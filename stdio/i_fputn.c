/*
** i_fputn.c  -  O/S shim for use with by iprintf() and ifprintf().
**               functions.  This module uses the ivprintf() and 
**               ivfprintf) macros in string.h.
**
** author:       Lyle V. Rains
**               This program is placed in the public domain.
*/


/* This function is expected to be superceeded by a platform specific re-write */

#include <stdio.h>
#include <stdarg.h>
#include <string.h>

FILE *stdin=NULL;
FILE *stdout=(FILE *)1;
FILE *stderr=(FILE *)3;
int errno;

void i_fputn(
  void          **stream,
  const char    *src,
  int           *len,
  int           *total_count,
  int           repeat
  )
{
  int i = *len;

  *total_count += i;
  if (repeat)
    while (0 < i--)
      fputc(*src, *(FILE **)stream);	/* Replace this with actual code to do the I/O */
  else
    while (0 < i--)
      fputc(*src++, *(FILE **)stream);	/* Replace this with actual code to do the I/O */
  *len = 0;
}

