#ifndef __STDDEF_DEFINED
#ifndef NULL
#define NULL (0)
#endif

typedef long ptrdiff_t;

#ifndef __size_t_DEF__
#	define	__size_t_DEF__	1
#ifdef 	_SYSTEM_size_t_HEADER_
#	include	_SYSTEM_size_t_HEADER_
#else
#include <size_t.h>
#endif
#endif

#endif /* __STDDEF_DEFINED */
