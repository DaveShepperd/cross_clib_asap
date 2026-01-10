#include <stdio.h>
#include <errno.h>
#include <string.h>

void perror(prefix)
char *prefix;
{
    int old_errno = errno;

    if ( prefix ) {
	fputs(prefix,stderr);
	fputs(": ",stderr);
    }
    fputs(strerror(old_errno),stderr);
    fputs("\n",stderr);
}
