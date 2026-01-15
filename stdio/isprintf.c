/*
** ISPRINTF.C -  source for isprintf() and ivsprintf() formatted output
**               functions.
**
** author:       Lyle V. Rains
**               This program is placed in the public domain.
*/

#if 0	/* This has been moved to i_sputn_macas.mac */
#include <stdio.h>
#include <stdarg.h>
#include <string.h>

void i_sputn( void **iDest, const char *string, int *count, int *total_count, int repeat)
{
  char *dest = *(char **)iDest;
  int i = *count;

  *total_count += i;
  if (repeat)
    (void) memset((void *)dest, *string, i);
  else
    (void) memcpy((void *)dest, string, i);
  *dest += i;
  *count = 0;
  return;
}
#endif

/* These functions have been moved to varargs.mac for the ASAP */
#if 0
int isprintf( char *buf, const char *format, ... )
{
  int n;
  va_list args;
  va_start (args, format);
  buf[n = idoprnt(i_sputn, (void *)buf, format, args)] = '\0';
  return (n);
}
    
int ivsprintf( char *buf, const char *format, va_list args )
{
  int n;
  buf[n = idoprnt(i_sputn, (void *)buf, format, args)] = '\0';
  return (n);
}

int isnprintf( char *buf, int bufLen, const char *format, ... )
{
  int n;
  va_list args;
  va_start (args, format);
  buf[n = idonprnt(i_sputn, (void *)buf, bufLen, format, args)] = '\0';
  return n;
}

int ivsnprintf( char *buf, int bufLen, const char *format, va_list args )
{
  int n;
  buf[n = idonprnt(i_sputn, (void *)buf, bufLen, format, args)] = '\0';
  return (n);
}
#endif
