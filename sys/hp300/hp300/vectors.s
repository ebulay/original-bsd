| Copyright (c) 1988 University of Utah
| Copyright (c) 1990, 1993
|	The Regents of the University of California.  All rights reserved.
|
| %sccs.include.redist.gas%
|
|	@(#)vectors.s	8.2 (Berkeley) 01/21/94
|
#ifdef HPFPLIB
/*
 * XXX the HP FP library mishandles "normal" F-line faults causing
 * the kernel to crash, hence we detect it ourselves rather than just
 * vectoring to "_fline".  We also always catch unsupported data type
 * faults ourselves for no particular reason.
 */
#define	_fpbsun		_bsun
#define	_fpinex		_inex
#define	_fpdz		_dz
#define	_fpunfl		_unfl
#define	_fpoperr	_operr
#define	_fpovfl		_ovfl
#define	_fpsnan		_snan
#else
#define	_fpbsun		_fpfault
#define	_fpinex		_fpfault
#define	_fpdz		_fpfault
#define	_fpunfl		_fpfault
#define	_fpoperr	_fpfault
#define	_fpovfl		_fpfault
#define	_fpsnan		_fpfault
#endif

	.text
	.globl	_buserr,_addrerr
	.globl	_illinst,_zerodiv,_chkinst,_trapvinst,_privinst,_trace
	.globl	_badtrap
	.globl	_spurintr,_lev1intr,_lev2intr,_lev3intr
	.globl	_lev4intr,_lev5intr,_lev6intr,_lev7intr
	.globl	_trap0,_trap1,_trap2,_trap15
	.globl	_fpfline, _fpunsupp, _fpfault
	.globl	_fpbsun, _fpinex, _fpdz, _fpunfl, _fpoperr, _fpovfl, _fpsnan
	.globl	_trap12

Lvectab:
	.long	0x4ef80400	/* 0: jmp 0x400:w (unused reset SSP) */
	.long	0		/* 1: NOT USED (reset PC) */
	.long	_buserr		/* 2: bus error */
	.long	_addrerr	/* 3: address error */
	.long	_illinst	/* 4: illegal instruction */
	.long	_zerodiv	/* 5: zero divide */
	.long	_chkinst	/* 6: CHK instruction */
	.long	_trapvinst	/* 7: TRAPV instruction */
	.long	_privinst	/* 8: privilege violation */
	.long	_trace		/* 9: trace */
	.long	_illinst	/* 10: line 1010 emulator */
	.long	_fpfline	/* 11: line 1111 emulator */
	.long	_badtrap	/* 12: unassigned, reserved */
	.long	_coperr		/* 13: coprocessor protocol violation */
	.long	_fmterr		/* 14: format error */
	.long	_badtrap	/* 15: uninitialized interrupt vector */
	.long	_badtrap	/* 16: unassigned, reserved */
	.long	_badtrap	/* 17: unassigned, reserved */
	.long	_badtrap	/* 18: unassigned, reserved */
	.long	_badtrap	/* 19: unassigned, reserved */
	.long	_badtrap	/* 20: unassigned, reserved */
	.long	_badtrap	/* 21: unassigned, reserved */
	.long	_badtrap	/* 22: unassigned, reserved */
	.long	_badtrap	/* 23: unassigned, reserved */
	.long	_spurintr	/* 24: spurious interrupt */
	.long	_lev1intr	/* 25: level 1 interrupt autovector */
	.long	_lev2intr	/* 26: level 2 interrupt autovector */
	.long	_lev3intr	/* 27: level 3 interrupt autovector */
	.long	_lev4intr	/* 28: level 4 interrupt autovector */
	.long	_lev5intr	/* 29: level 5 interrupt autovector */
	.long	_lev6intr	/* 30: level 6 interrupt autovector */
	.long	_lev7intr	/* 31: level 7 interrupt autovector */
	.long	_trap0		/* 32: syscalls */
	.long	_trap1		/* 33: sigreturn syscall or breakpoint */
	.long	_trap2		/* 34: breakpoint or sigreturn syscall */
	.long	_illinst	/* 35: TRAP instruction vector */
	.long	_illinst	/* 36: TRAP instruction vector */
	.long	_illinst	/* 37: TRAP instruction vector */
	.long	_illinst	/* 38: TRAP instruction vector */
	.long	_illinst	/* 39: TRAP instruction vector */
	.long	_illinst	/* 40: TRAP instruction vector */
	.long	_illinst	/* 41: TRAP instruction vector */
	.long	_illinst	/* 42: TRAP instruction vector */
	.long	_illinst	/* 43: TRAP instruction vector */
	.long	_trap12		/* 44: TRAP instruction vector */
	.long	_illinst	/* 45: TRAP instruction vector */
	.long	_illinst	/* 46: TRAP instruction vector */
	.long	_trap15		/* 47: TRAP instruction vector */
 	.long	_fpbsun		/* 48: FPCP branch/set on unordered cond */
 	.long	_fpinex		/* 49: FPCP inexact result */
 	.long	_fpdz		/* 50: FPCP divide by zero */
 	.long	_fpunfl		/* 51: FPCP underflow */
 	.long	_fpoperr	/* 52: FPCP operand error */
 	.long	_fpovfl		/* 53: FPCP overflow */
 	.long	_fpsnan		/* 54: FPCP signalling NAN */

	.long	_fpunsupp	/* 55: FPCP unimplemented data type */
	.long	_badtrap	/* 56: unassigned, reserved */
	.long	_badtrap	/* 57: unassigned, reserved */
	.long	_badtrap	/* 58: unassigned, reserved */
	.long	_badtrap	/* 59: unassigned, reserved */
	.long	_badtrap	/* 60: unassigned, reserved */
	.long	_badtrap	/* 61: unassigned, reserved */
	.long	_badtrap	/* 62: unassigned, reserved */
	.long	_badtrap	/* 63: unassigned, reserved */
#define BADTRAP16	.long	_badtrap,_badtrap,_badtrap,_badtrap,\
				_badtrap,_badtrap,_badtrap,_badtrap,\
				_badtrap,_badtrap,_badtrap,_badtrap,\
				_badtrap,_badtrap,_badtrap,_badtrap
	BADTRAP16		/* 64-255: user interrupt vectors */
	BADTRAP16		/* 64-255: user interrupt vectors */
	BADTRAP16		/* 64-255: user interrupt vectors */
	BADTRAP16		/* 64-255: user interrupt vectors */
	BADTRAP16		/* 64-255: user interrupt vectors */
	BADTRAP16		/* 64-255: user interrupt vectors */
	BADTRAP16		/* 64-255: user interrupt vectors */
	BADTRAP16		/* 64-255: user interrupt vectors */
	BADTRAP16		/* 64-255: user interrupt vectors */
	BADTRAP16		/* 64-255: user interrupt vectors */
	BADTRAP16		/* 64-255: user interrupt vectors */
	BADTRAP16		/* 64-255: user interrupt vectors */
