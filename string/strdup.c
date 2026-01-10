/*	strdup() returns a pointer to a copy of the string 'src'. The copy
*	is created in memory gotten from malloc().
*/
#include <stdlib.h>

char *
strdup (src)
char *src;
{
    char * dst;
    int len;

    len =  strlen(src)+1;
    if ( (dst = malloc(len)) != 0 ) memcpy(dst,src,len);
    return dst;
}
