
extern int errno;
extern void perror();

#define EDOM (33)
#define ERANGE (34)
/* The numbers below weren't _exactly_ picked out of a hat, but rather
*  are the common ones for *nix compatible systems...
*/
#ifndef EIO
#define EIO (5)	/* Nebulous Input/Output error */
#endif
#ifndef ENOMEM
#define ENOMEM (12)	/* [m,re,c]alloc failed */
#endif
#ifndef ENOENT
#define ENOENT (2)	/* File Not found */
#endif
#ifndef E2BIG
#define E2BIG (7)	/* Too many args, or arg list too long */
#endif
#ifndef EINVAL 
#define EINVAL (22)	/* Bogus argument */
#endif

