/*
** IPRINTF.C  -  source for iprintf() and ifprintf() formatted output
**               functions.  This module uses the ivprintf() and 
**               ivfprintf) macros in iprint.h.
**
** author:       Lyle V. Rains
**               This program is placed in the public domain.
*/



#include <stdio.h>
#include <stdarg.h>
#include <string.h>

int ifprintf( FILE *stream, const char *format, ... )
{
  va_list args;
  va_start (args, format);
  return idoprnt(i_fputn, stream, format, args);
}

int ivfprintf( FILE *stream, const char *format, va_list args )
{
  return idoprnt(i_fputn, stream, format, args);
}

int iprintf(const char *format, ...) 
{
  va_list args;
  va_start (args, format);
  return idoprnt(i_fputn, stdout, format, args);
}

int ivprintf( const char *format, va_list args )
{
  return idoprnt(i_fputn, stdout, format, args);
}

