#include <limits.h>	/* need UCHAR_MAX */
#include <stddef.h>	/* need size_t */

/* This file contains simple C source for the Ansi defined functions
*  memchr, memcmp, memcpy, memmove, and memset. For definitions of
*  individual functions, see comments below at each function.
*     This code is copyright by Michael Albaugh 1989. Anyone may do
*  anything to it with the sole exceptions of removing this notice or
*  attaching further restrictions.
*/

void *
memchr ( srch, chr, len)
const void *srch;
int chr;
size_t len;
/* locates first occurence of char 'chr' within first 'len' chars of array srch.
* returns pointer to char or NULL if not found.
*/
{
    register unsigned char cmp,*cp;

    /* I believe the following line will resolve the ambiguity in the
    *  spec about assignments of negative values to smaller unsigned
    *  integral types. However, it _may_ be legal to have a UCHAR_MAX that
    *  does _not_ equal 2**(CHAR_BIT)-1.
    */
    cmp = chr & UCHAR_MAX;		/* lose the bits that *cp will */
    cp = (unsigned char *)srch;
    while ( len && (*cp != cmp) ) { --len ; ++cp;}
    if ( len ) return (void *)cp;
    else return 0;
}

int
memcmp ( s1, s2, len)
const void *s1;
const void *s2;
size_t len;
/* compares first 'len' chars of arrays s1 and s2.
*  returns integer <, ==, or < 0 depending on first differing character.
*/
{

    unsigned char *t1,*t2;

    t1 = (unsigned char *)s1;
    t2 = (unsigned char *)s2;
    while ( len && (*t1++ == *t2++) ) --len ;

    if ( !len ) return len;
    return (*--t1) - (*--t2);
}

void *
memcpy ( dst, src, len)
void *dst;
const void *src;
size_t len;
/* copy 'len' chars from array pointed to by 'src' to array pointed to by 'dst'
*  returns dst.
*  No guarantee of "correct" behavior for overlapping src,dst (see memmove)
*/
{
    unsigned char *dp,*srcp;

    dp = (char *)dst;
    srcp = (char *)src;
    while ( len-- ) *dp++ = *srcp++;
    return dst;
}

void *
memmove (dst, src, len)
void *dst;
const void *src;
size_t len;
/* copy 'len' chars from array pointed to by 'src' to array pointed to by 'dst'
* returns dst.
* Checks for possible overlap and "does the right thing"
*/
{
    unsigned char *dp,*srcp;

    dp = (char *) dst;
    srcp = (char *) src;
    /* note that on some machines which nonetheless meet the standard,
    *  the following comparison is undefined. On such machines memmove
    *  cannot be written in C anyway.
    */
    if ( dp < srcp ) {
	while ( len-- ) *dp++ = *srcp++;
    } else {
	dp += len;
	srcp += len;
	while ( len-- ) *--dp = *--srcp;
    }
    return dst;
}

void *
memset ( dst, chr, len)
void *dst;
char chr;
size_t len;
/* set first 'len' characters of array pointed to by 'dst' to value 'chr'
* returns dst
*/
{
    unsigned char *dp;

    dp = (unsigned char *)dst;
    while ( len-- ) *dp++ = chr;
    return dst;
}
