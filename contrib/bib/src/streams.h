/*
 *	@(#)streams.h	2.3	05/27/93
 */
long int nextrecord(), recsize(), nextline();

# define  maxstr            256
# define  pos(x)            fseek(stream,x,0)

