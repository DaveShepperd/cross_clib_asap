/****************************************************************************/
/*									    */
/*		    L o n g j u m p   H e a d e r   F i l e		    */
/*		    ---------------------------------------		    */
/*									    */
/*	Long jumps are implemented as follows:  			    */
/*									    */
/*		1).	Routine "setjmp" is called to setup a special 	    */
/*			buffer for return.  The return address, stack 	    */
/*			pointer frame pointer and all "non-temp" regs	    */
/*			are saved.  This allows	the calling program	    */	
/*			do the proper number of "pops".			    */
/*									    */
/*		2).	At some later time, the procedure "longjmp" is	    */
/*			called.  The programmer sees a return from the	    */
/*			previous "setjmp" as the result.		    */
/*									    */
/*	Calling sequence:						    */
/*									    */
/*		#include	<setjmp.h> (definitions)		    */
/*		jmp_buf	 env;	(define a buffer for saved stuff)	    */
/*									    */
/*		setjmp(env);						    */
/*	a:								    */
/*									    */
/*		longjmp(env,val);					    */
/*									    */
/*	Setjmp returns an int of 0 on first call, and "val" on the 	    */
/*	subsequent "longjmp" call.  The longjmp call causes execution to    */
/*	resume at "a:" above.						    */
/*									    */
/****************************************************************************/
#ifndef _SETJMP_H_
#define _SETJMP_H_

/* if compiling for ASAP and not over-ridden, we need to save 19 regs (%10-.SP) */
#ifndef __SETJMP_NREGS__
#ifdef __GNUC__
#ifdef __asap__
#define __SETJMP_NREGS__ (19)
#endif
#endif
#endif

/* If still no definition, use the old 68K default of 13 */
#ifndef __SETJMP_NREGS__
#define __SETJMP_NREGS__ (13)
#endif

typedef	int	jmp_buf[__SETJMP_NREGS__];
extern int setjmp(),longjmp();
#endif	/* _SETJMP_H_ */

