/*
 * Copyright (c) 1986 The Regents of the University of California.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms are permitted
 * provided that the above copyright notice and this paragraph are
 * duplicated in all such forms and that any documentation,
 * advertising materials, and other materials related to such
 * distribution and use acknowledge that the software was developed
 * by the University of California, Berkeley.  The name of the
 * University may not be used to endorse or promote products derived
 * from this software without specific prior written permission.
 * THIS SOFTWARE IS PROVIDED ``AS IS'' AND WITHOUT ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
 * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE.
 *
 *	@(#)psreg.h	7.3 (Berkeley) 05/09/89
 */

#ifndef _PSREG_
#define _PSREG_
/*
 * PS300 definitions.
 */
#ifndef KERNEL
#include <sys/ioctl.h>
#else
#include "ioctl.h"
#endif

struct  pslookup {
        int     pl_len;                 /* length of name string */
        char    *pl_name;               /* address of name string */
        u_long  pl_addr;                /* symbol's address */
};

/*
 * Maximum string which may be supplied for lookup.
 */
#define PS_MAXNAMELEN   256             /* must be <= PSMAXDMA */

/*
 * When doing physical i/o, one may specify the address
 * and whether or not refresh sync should is required with
 * a null iovec descriptor (iov_len = 0).
 */
#define PSIO_SYNC       1               /* do physical write w/ refresh sync */

/*
 * Ioctl requests.
 */
#define PSIOGETERROR    _IOR('p', 0, int)                 /* get last error */
#define PSIOLOOKUP      _IOWR('p', 1, struct pslookup)    /* do name lookup */
#define PSIORWLOGICAL   _IOWR('p', 2, int)                /* set i/o mode */

/*
 * Error codes returned by PSIOGETERROR are either
 * returned by the PS300 or, from the list below,
 * generated by the device driver.
 */
#define PSERROR_DIOTIMO         1       /* timeout during dioread/diowrite */
#define PSERROR_INVALBC         2       /* invalid byte count for read/write */
#define PSERROR_BADADDR         3       /* invalid address for read/write */
#define PSERROR_BADCMD          4       /* invalid command in ikstart */
#define PSERROR_NAMETIMO        5       /* timeout during nameaddr dioread */
#define PSERROR_CMDTIMO         6       /* operation timed out */
#endif
