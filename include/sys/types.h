#ifndef _TYPES_H_
#define _TYPES_H_

#ifndef	_TIME_T
#	define	_TIME_T
	typedef	long		time_t;		/* <time> type */
#endif	/* _TIME_T */
#ifndef	_CLOCK_T
#	define	_CLOCK_T
	typedef	long	clock_t;	/* value returned by clock() */
#endif	/* _CLOCK_T */

#ifndef	_PTRDIFF_T
#	define	_PTRDIFF_T
	typedef	int	ptrdiff_t;	/* difference between two pointeres */
#endif	/* _PTRDIFF_T */

#ifndef	_FPOS_T
#	define	_FPOS_T
	typedef long	fpos_t;	/* file positioning */
#endif	/* _FPOS_T */

#ifndef	_SIG_ATOMIC_T
#	define	_SIG_ATOMIC_T
	typedef	int	sig_atomic_t;	/* atomic entity for signal handling */
#endif	/* _SIG_ATOMIC_T */

#endif	/* _TYPES_H_ */
