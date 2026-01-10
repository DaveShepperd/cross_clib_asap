#ifndef _STDLIB_H_
#define _STDLIB_H_

#ifndef vms
#define	EXIT_FAILURE	(1)
#define	EXIT_SUCCESS	(0)
#endif
/* following is as good a definition as any for systems that don't handle
*  multi-byte characters at all...
*/
#ifdef __FULL_HOSTED__
#define	MB_CUR_MAX	(1)
#endif
#ifndef NULL
#define	NULL		(0)
#endif
#define	RAND_MAX	((1L<<31)-3)	/* see stdlib/random.c for explanation*/

typedef struct {
		int	quot;
		int	rem;
}	div_t;

typedef struct {
		long	quot;
		long	rem;
}	ldiv_t;

#ifndef __size_t_DEF__
#	define	__size_t_DEF__	1
#ifdef 	_SYSTEM_size_t_HEADER_
#	include	_SYSTEM_size_t_HEADER_
#else
#include <size_t.h>
#endif
#endif

void	abort	( void );
int	abs	( int i );
int	atexit	( void ( *exit_handler )( void ) );
double	atof	( const char *str );
int	atoi	( const char *str );
long	atol	( const char *str );
void	*bsearch (
		const void *key, const void *ordered_array,
		size_t table_size, size_t item_size,
		int ( *compare )( const void *key, const void *ordered_element )
	);
void	*calloc	( size_t cnt, size_t size );
div_t	div	( int numer, int denom );
void	exit	( int status );
void	free	( void *alloced_object );
char	*getenv	( const char *name );
long	labs	( long i );
ldiv_t	ldiv	( long numer, long denom );
void	*malloc	( size_t size );
#ifdef __FULL_HOSTED__
int	mblen	( const char *mbstr, size_t limit );
size_t	mbstowcs( wchar_t *wcs, const char *mbs, size_t limit );
int	mbtowc	( wchar_t *wcp, const char *mbs, size_t limit );
#endif
void	qsort (
		const void *array_to_be_sorted,
		size_t table_size, size_t item_size,
		int ( *compare )( const void *item1, const void *item2 )
	);
int	rand	( void );
void	*realloc( void *old_object, size_t new_size );
void	srand	( unsigned seed );
double	strtod	( const char *str, char **tail );
long	strtol	( const char *str, char **tail, int radix );
long	strtoul	( const char *str, char **tail, int radix );
int	system	( const char *command );
#ifdef __FULL_HOSTED__
size_t	wcstombs( char *mbs, const wchar_t *wcs, size_t limit );
int	wctomb	( char *mbs, wchar_t wchar );
#endif

#endif	/* _STDLIB_H_ */
