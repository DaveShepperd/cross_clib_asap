/*		assert.h for cross-compiled embedded systems
 *	The outer macro (assert()) is based on Doug Gwyn's Public
 *	Domain implementation. The following comment is from his
 *	file:
 */

/*
	<assert.h> -- definitions for verifying program assertions

	public-domain implementation

	last edit:	12-Apr-1990	Gwyn@BRL.MIL

	complies with the following standards:
		ANSI X3.159-1989
		IEEE Std 1003.1-1988
		SVID Issue 3 (except for the extra blank in the NDEBUG case)
		X/Open Portability Guide Issue 3 (ditto)
 */

#undef	assert

#ifdef	NDEBUG

#define	assert(ignore)	((void)0)

#else	/* ! NDEBUG */

#ifdef	__STDC__

extern void	__assert(const char *, const char *, int);

#define	assert(ex)	((ex) ? (void)0 : __assert(#ex, __FILE__, __LINE__))

#else	/* ! __STDC__ */

extern void	__assert();

/* Reiser CPP behavior assumed: */
#define	assert(ex)	((ex) ? (void)0 : __assert("ex", __FILE__, __LINE__))

#endif	/* __STDC__ */

#endif	/* NDEBUG */

/*		Extra definitions for use where the reporting routine
 *	takes only a single parameter, of type (const char *)
 *	MEA, 10NOV93
 */
#ifndef NDEBUG
extern void panic(const char *);
#define __assert__(expr, file, lineno) \
panic("assertion failed: " #expr " " file ", Line "#lineno)
#define __assert(expr, file, lineno) __assert__(expr, file, lineno)
#endif


