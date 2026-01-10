/*	Simple string functions. These are the short relatively quick
*	functions from string.h. Some others (strpbrk, strspn, strcspn, strstr,
*	strtok)	are in complex_strings.c. strdup is in a separate file, as
*	it is not part of the standard.
*	Locale-specific routines (strcoll, strxfrm, strerror) may never be
*	written	by me.
*	   This code is copyright by Michael Albaugh 1989. Anyone may do
*	anything to it with the sole exceptions of removing this notice or
*	attaching further restrictions.
*/

#include <stddef.h>	/* need size_t */

/*	strcat() concatenates (nul-terminated) string src to end of dst.
*	No check for overflow is made.
*/

char *strcat (char *dst, const char *src)
{
    char *pd;

    for ( pd = dst ; *pd ; ++pd ) {;}	/* find NUL at end of dst */
    while ( (*pd++ = *src++) != 0 ) {;}	/* copy src including NUL */
    return dst;
}

/*	strncat() concatenates at most 'cnt' chars of string src to end of dst.
*	No check for overflow is made (other than the presumption that the
*	caller has allowed enogh room for 'cnt' more chars in 'dst')
*/

char *strncat (char *dst, const char *src, size_t cnt)
{
    char *pd;
    if ( cnt ) {
	for ( pd = dst ; *pd ; ++pd ) {;}	/* find NUL at end of dst */
	while ( (*pd++ = *src++) != 0 ) {
	    if (--cnt == 0) {*pd = '\0';break;}	/* bail out if cnt exceeded */
	}
    }
    return dst;
}

/*	strcmp() compares the (nul-terminated) strings at first and second,
*	and returns an integer greater than zero if (first > second), less
*	than zero if (first < second) or 0 if the two are equal. The version
*	below uses unsigned characters, although I am not sure the standard
*	requires it.
*/
int strcmp (const char *first, const char *second)
{
    unsigned char *s1, *s2;
    int c;

    s1 = (unsigned char *)first;
    s2 = (unsigned char *)second;
    while ( (c = *s1++) && !(c -= *s2++) ) {;}
    return c ? c : -(*s2);
}

/*	strncmp() compares at most the first 'cnt' characters of the
*	(nul-terminated) strings at first and second, and returns an
*	integer greater than zero if (first > second), less than zero
*	if (first < second) or 0 if the two are equal. The version below
*	uses unsigned characters, although I am not sure the standard
*	requires it.
*/
int strncmp (const char *first, const char *second, size_t cnt)
{
    const unsigned char *s1, *s2;
    int c;

    if ( cnt ) {
	s1 = (const unsigned char *)first;
	s2 = (const unsigned char *)second;
    	while ( (c = *s1++) && !(c -= *s2++) && --cnt)
       {
	       ;
       }
    }
    return (cnt == 0) ? 0 : ( c ? c : -(*s2));
}

/*	strcpy() copies (nul-terminated) string src to dst.
*	No check for overflow is made.
*/

char *strcpy (char *dst, const char *src)
{
    register char *pd;

    pd = dst;
   /* copy src including NUL */
    while ( (*pd++ = *src++) != 0 )
   {
	   ;
   }
    return dst;
}

/*	strncpy() copies at most 'cnt' chars of string src to dst.
*	No check for overflow is made (other than the presumption that the
*	caller has allowed enogh room for 'cnt' chars in 'dst')
*/

char *strncpy (char *dst, const char *src, size_t cnt)
{
    char *pd;
    if ( cnt ) {
	pd = dst;
	while ( (*pd++ = *src++) != '\0' ) {
	    if (--cnt == 0)
		   return dst;	/* bail out if cnt exceeded */
	}
	/* now fill remainder if any with NULs. Yes, the pANS says we do,
	*  although strncat() doesn't
	*/
	if ( --cnt )
	       memset(pd,0,cnt);
    }
    return dst;
}

/*	strlen() returns the number of chars in the (nul-terminated) string
*	src (not counting the nul).
*/

size_t strlen (const char *src)
{
    const char *ps;
   /* find NUL at end of src */
    for ( ps = src ; *ps ; ++ps )
   {
	   ;
   }
    return ps - src;
}

/*	strchr() returns a pointer to the first occurence of 'chr' in string
*	at 'src'. If no instance of 'chr', returns the null pointer.
*/
char *strchr (const char *src, int chr)
{
    int c;
    for ( ; ( c = *src ) != chr ; ++src )
	   if ( c == '\0' ) return 0;
    return (char *)src;
}

/*	strrchr() returns a pointer to the last occurence of 'chr' in string
*	at 'src'. If no instance of 'chr', returns the null pointer.
*/
char *strrchr (const char *src, int chr)
{
    int c;
    const char *last = NULL;
    do {
	if ( (c = *src) == chr )
	       last = src;
	++src;
    } while ( c != '\0' );
    return (char *)last;
}
