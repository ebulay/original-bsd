/*
 * Copyright (c) University of British Columbia, 1984
 * Copyright (c) 1990 The Regents of the University of California.
 * All rights reserved.
 *
 * This code is derived from software contributed to Berkeley by
 * the Laboratory for Computation Vision and the Computer Science Department
 * of the University of British Columbia.
 *
 * %sccs.include.redist.c%
 *
 *	@(#)hd_subr.c	7.4 (Berkeley) 08/30/90
 */

#include "param.h"
#include "systm.h"
#include "mbuf.h"
#include "domain.h"
#include "socket.h"
#include "protosw.h"
#include "errno.h"
#include "time.h"
#include "kernel.h"

#include "../net/if.h"

#include "hdlc.h"
#include "hd_var.h"
#include "x25.h"

hd_init ()
{

	hdintrq.ifq_maxlen = IFQ_MAXLEN;
}

hd_ctlinput (prc, addr)
caddr_t addr;
{
	register struct x25config *xcp = (struct x25config *)addr;
	register struct ifnet *ifp;
	register struct hdcb *hdp;
	register struct ifaddr *ifa;

	if (xcp->xc_addr.x25_family != AF_CCITT)
		return (EAFNOSUPPORT);
	if (xcp->xc_lptype != HDLCPROTO_LAPB)
		return (EPROTONOSUPPORT);
	for (ifa = ifa_ifwithaddr ((struct sockaddr *)xcp); ifa; ifa = ifa->ifa_next)
		if (ifa->ifa_addr->sa_family == AF_CCITT)
			break;
	if (ifa == 0 || (ifp = ifa->ifa_ifp) == 0)
		panic ("hd_ctlinput");
	for (hdp = hdcbhead; hdp; hdp = hdp->hd_next)
		if (hdp->hd_ifp == ifp)
			break;

	if (hdp == 0) {		/* new interface */
		register int error;
		register struct mbuf *m;

		m = m_getclr (M_DONTWAIT, MT_PCB);
		if (m == 0)
			return (ENOBUFS);
		if (error = pk_ctlinput (PRC_LINKDOWN, xcp)) {
			m_freem (m);
			return (error);
		}

		hdp = mtod (m, struct hdcb *);
		hdp->hd_ifp = ifp;
		hdp->hd_xcp = xcp;
		hdp->hd_next = hdcbhead;
		hdcbhead = hdp;
		hdp->hd_state = INIT;
	}

	switch (prc) {
	case PRC_IFUP:
		if (xcp->xc_lwsize == 0 ||
			xcp->xc_lwsize > MAX_WINDOW_SIZE)
				xcp->xc_lwsize = MAX_WINDOW_SIZE;
		if (hdp->hd_state == INIT)
			SET_TIMER (hdp);
		break;

	case PRC_IFDOWN:
		if (hdp->hd_state == ABM)
			hd_message (hdp, "Operator shutdown: link closed");
		(void) pk_ctlinput (PRC_LINKDOWN, xcp);
		hd_writeinternal (hdp, DISC, POLLON);
		hdp->hd_state = DISC_SENT;
		SET_TIMER (hdp);
	}
	return (0);
}

hd_initvars (hdp)
register struct hdcb *hdp;
{
	register struct mbuf *m;
	register int i;

	/* Clear Transmit queue. */
	while ((m = hd_remove (&hdp->hd_txq)) != NULL)
		m_freem (m);

	/* Clear Retransmit queue. */
	i = hdp->hd_lastrxnr;
	while (i != hdp->hd_retxqi) {
		m_freem (hdp->hd_retxq[i]);
		i = (i + 1) % MODULUS;
	}
	hdp->hd_retxqi = 0;

	hdp->hd_vs = hdp->hd_vr = 0;
	hdp->hd_lasttxnr = hdp->hd_lastrxnr = 0;
	hdp->hd_rrtimer = 0;
	KILL_TIMER(hdp);
	hdp->hd_retxcnt = 0;
	hdp->hd_condition = 0;
}

hd_decode (hdp, frame)
register struct hdcb *hdp;
struct Hdlc_frame *frame;
{
	register int frametype = ILLEGAL;
	register struct Hdlc_iframe *iframe = (struct Hdlc_iframe *) frame;
	register struct Hdlc_sframe *sframe = (struct Hdlc_sframe *) frame;
	register struct Hdlc_uframe *uframe = (struct Hdlc_uframe *) frame;

	if (iframe -> hdlc_0 == 0) {
		frametype = IFRAME;
		hdp->hd_iframes_in++;
	}

	else if (sframe -> hdlc_01 == 1) {
		/* Supervisory format. */
		switch (sframe -> s2) {
		case 0: 
			frametype = RR;
			hdp->hd_rrs_in++;
			break;

		case 1: 
			frametype = RNR;
			hdp->hd_rnrs_in++;
			break;

		case 2: 
			frametype = REJ;
			hdp->hd_rejs_in++;
		}
	}
	else if (uframe -> hdlc_11 == 3) {
		/* Unnumbered format. */
		switch (uframe -> m3) {
		case 0: 
			frametype = DM;
			break;

		case 1: 
			frametype = SABM;
			break;

		case 2: 
			frametype = DISC;
			break;

		case 3: 
			frametype = UA;
			break;

		case 4: 
			frametype = FRMR;
			hdp->hd_frmrs_in++;
		}
	}
	return (frametype);
}

/* 
 *  This routine is called when the HDLC layer internally  generates a
 *  command or  response  for  the remote machine ( eg. RR, UA etc. ). 
 *  Only supervisory or unnumbered frames are processed.
 */

hd_writeinternal (hdp, frametype, pf)
register struct hdcb *hdp;
register int frametype, pf;
{
	register struct mbuf *buf;
	struct Hdlc_frame *frame;
	register struct Hdlc_sframe *sframe;
	register struct Hdlc_uframe *uframe;

	MGET (buf, M_DONTWAIT, MT_HEADER);
	if (buf == 0)
		return;
	frame = mtod (buf, struct Hdlc_frame *);
	sframe = mtod (buf, struct Hdlc_sframe *);
	uframe = mtod (buf, struct Hdlc_uframe *);

	/* Assume a response - address structure for DTE */
	frame -> address = ADDRESS_A;
	buf -> m_len = 2;
	buf -> m_act = buf -> m_next = NULL;

	switch (frametype) {
	case RR: 
		frame -> control = RR_CONTROL;
		hdp->hd_rrs_out++;
		break;

	case RNR: 
		frame -> control = RNR_CONTROL;
		hdp->hd_rnrs_out++;
		break;

	case REJ: 
		frame -> control = REJ_CONTROL;
		hdp->hd_rejs_out++;
		break;

	case SABM: 
		frame -> control = SABM_CONTROL;
		frame -> address = ADDRESS_B;
		break;

	case DISC: 
		frame -> control = DISC_CONTROL;
		frame -> address = ADDRESS_B;
		break;

	case DM: 
		frame -> control = DM_CONTROL;
		break;

	case UA: 
		frame -> control = UA_CONTROL;
		break;

	case FRMR: 
		frame -> control = FRMR_CONTROL;
		bcopy ((caddr_t)&hd_frmr, (caddr_t)frame -> info, 3);
		buf -> m_len = 5;
		hdp->hd_frmrs_out++;

	}

	if (sframe -> hdlc_01 == 1) {
		/* Supervisory format - RR, REJ, or RNR. */
		sframe -> nr = hdp->hd_vr;
		sframe -> pf = pf;
		hdp->hd_lasttxnr = hdp->hd_vr;
		hdp->hd_rrtimer = 0;
	}
	else
		uframe -> pf = pf;

	hd_trace (hdp, TX, frame);
	(*hdp->hd_output)(hdp, buf);
}

struct mbuf *
hd_remove (q)
struct hdtxq *q;
{
	register struct mbuf *m;

	m = q -> head;
	if (m) {
		if ((q -> head = m -> m_act) == NULL)
			q -> tail = NULL;
		m -> m_act = 0;
	}
	return (m);
}

hd_append (q, m)
register struct hdtxq *q;
register struct mbuf *m;
{

	m -> m_act = NULL;
	if (q -> tail == NULL)
		q -> head = m;
	else
		q -> tail -> m_act = m;
	q -> tail = m;
}

hd_flush (ifp)
struct ifnet *ifp;
{
	register struct mbuf *m;
	register int s;

	while (1) {
		s = spl6 ();		/* XXX SHOULDN'T THIS BE splimp? */
		IF_DEQUEUE (&ifp->if_snd, m);
		splx (s);
		if (m == 0)
			break;
		m_freem (m);
	}
}

hd_message (hdp, msg)
struct hdcb *hdp;
char *msg;
{
	char *format_ntn ();

	if (hdcbhead -> hd_next)
		printf ("HDLC(%s): %s\n", format_ntn (hdp->hd_xcp), msg);
	else
		printf ("HDLC: %s\n", msg);
}

#ifdef HDLCDEBUG
hd_status (hdp)
struct hdcb *hdp;
{
	printf ("HDLC STATUS:\n V(S)=%d, V(R)=%d, retxqi=%d,\n",
		hdp->hd_vs, hdp->hd_vr, hdp->hd_retxqi);

	printf ("Last_rx_nr=%d, Last_tx_nr=%d,\n Condition=%d, Xx=%d\n",
		hdp->hd_lastrxnr, hdp->hd_lasttxnr, hdp->hd_condition, hdp->hd_xx);
}
#endif
