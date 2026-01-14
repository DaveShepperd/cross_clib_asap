/*
** IPRINTF.C  -  source for iprintf() and ifprintf() formatted output
**               functions.  This module uses the ivprintf() and 
**               ivfprintf) macros in iprint.h.
**
** author:       Lyle V. Rains
**               This program is placed in the public domain.
*/

/* This functionality has been moved to varargs.mac for the ASAP */

#if 0
#include <stdio.h>
#include <varargs.h>
#include <string.h>

int ifprintf( FILE *stream, const char *format, ... )
{
  va_list args;
  va_start (args, format);
  return idoprnt(i_fputn, stream, format, args);
}

#if 0
int ivfprintf( FILE *stream, const char *format, va_list args )
{
  return idoprnt(i_fputn, stream, format, args);
}
#endif

int iprintf(const char *format, ...) 
{
  va_list args;
  va_start (args, format);
  return idoprnt(i_fputn, stdout, format, args);
}

#if 0
int ivprintf( const char *format, va_list args )
{
  return idoprnt(i_fputn, stdout, format, args);
}
#endif

#endif
