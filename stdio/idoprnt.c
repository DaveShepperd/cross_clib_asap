/*
**    IDOPRINT.C : integer, char, string only version of _doprint()
*/

#ifdef  DOCUMENTATION

Synopsis

        int idoprnt(putstr, format, arglist)
          void            (*i_putc)();
          char            *format;
          va_list         arglist;

Description

        idoprnt() performs all formatting operations for iprintf()
        and related formatted output functions.
Author
        Lyle V. Rains
        This program is placed in the public domain.

#endif

#include <stdio.h>
#include <limits.h>
#include <ctype.h>
#include <string.h>
#include <stdarg.h>

extern char *ultos();

#define FALSE         0
#define TRUE          1
#define DEF_PREC      -1
#define USHORT_MASK   (USHRT_MAX)
#define INT_IS_LONG   (sizeof(int) > 2)
#define NUMSIZE   (sizeof(int) * CHAR_BIT)

#define ATOI(value) \
  for (value = 0; isdigit(c); c = *(format++)) \
    ( value *= 10, value += (c - '0') )
  
#define NON_NEG(i, expr) \
   ((((i) = (expr)) < 0) ? ((i)=0) : (i) )
    
#define PUTNSTR(s, i) i_putn((void *)&dest, (s), (i), &total_count, 0)

#define PUTNCHR(s, i) i_putn((void *)&dest, (s), (i), &total_count, 1)

int
idoprnt(
  doFPrintFunc    i_putn,                 /* String output routine        */	
  FILE            *dest,                  /* A file pointer               */
  const char      *format,                /* Format string                */
  va_list         argp                    /* Argument vector pointer      */
  )
{
  register int    c;
  char            numstr[NUMSIZE];        /* Numeric arg string buffer    */
  char            *arg_str;               /* Pointer, mostly to above     */
  char            prefix[3];              /* Numeric arg prefix buffer    */
  char            *pfx_str;               /* Pointer to above             */
  int             precision;              /* Min length for numbers;      */
                                          /* Max length for strings;      */
                                          /* Ignored for characters       */
  int             minwidth;               /* Argument width               */
  long            value;
  int             radix;                  /* Conversion radix             */
  int             ljustify;               /* TRUE for left-justification  */
  int             is_long;
  int             pound;                  /* Special prefix format        */
  int             addspace;               /* Add space left of positive   */
  int             addplus;                /* Add plus left of positive    */
  int             zerofill;               /* '0' or ' ' for fill flag     */
  int             arg_len;                /* Argument field length        */
  int             pfx_len;                /* Prefix field length          */
  int             fill_cnt;               /* Count of fill characters     */
  int             prec_cnt;               /* Count of extra precision 0's */
  int             total_count;            /* Running count of chars sent  */

  total_count = 0;
  while ((arg_str = strchr(format, '%')) != NULL)
  {
    arg_len = arg_str - format;
    PUTNSTR(format, &arg_len);
    format = ++arg_str;
    /*
    ** Handle special case "%%"
    */
    if (*format == '%')
    {
      arg_len = 1;
      PUTNSTR(format, &arg_len);
      ++format;
      continue;
    }
    /*
    ** Process flags
    */
    ljustify = addspace = addplus = pound = 0;
    for (;;)
    {
      switch (c = *format++)
      {
      case '-' :                    /* %-  Left justify                 */
        ++ljustify; continue;
      case ' ' :                    /* %_  Leading space on positive    */
        ++addspace; continue;
      case '+' :                    /* %+  Leading '+' on positive      */
        ++addplus; continue;
      case '#' :                    /* %#  Leading "0", "0x" or "0X"    */
        ++pound; continue;
      }
      break;
    }
    /*
    ** Process width field
    */
    zerofill = FALSE;
    if (c == '0')
    {
      c = *format++;
      if (!ljustify) zerofill = TRUE;
    }
    if (c == '*')                   /* %*d  Field width from arg        */
    {
      c = *format++;
      minwidth = va_arg(argp, int);
    }
    else
      ATOI(minwidth);
    /*
    ** Process precision field
    */
    precision = DEF_PREC;           /*      Default precision flag      */
    if (c == '.')                   /*      Precision specified         */
    {
      if ((c = *format++) == '*')   /* %n.* Precision from arg          */
      {
        c = *format++;
        precision = va_arg(argp, int);
      }
      else                          /* %n.n Precision from format str   */
        ATOI(precision);
    }
    /*
    ** Process type modifier field
    */
    is_long = INT_IS_LONG;
    if (c == 'h') {
      c = *format++;
      is_long = FALSE;
    }
    else if (c == 'l')
    {
      c = *format++;
      is_long = TRUE;
    }
    /*
    ** Process conversion chars
    */
    prec_cnt = pfx_len = 0;
    switch (c) {
    case 'n':
      *(va_arg(argp, int*)) = total_count;
      break;
    case 'c':
      numstr[0] = (char)(va_arg(argp, int));
      arg_str = numstr;
      arg_len = 1;
      zerofill = FALSE;
      goto send_it;
    case 's':
      arg_str = va_arg(argp, char *);
      /*
      ** (The precision field limits strings)
      */
      if ( precision < (arg_len = strlen(arg_str))  &&  precision != DEF_PREC )
        arg_len = precision;
      zerofill = FALSE;
      goto send_it;
    case 'o':
      radix = 8;
      if (pound)
        prefix[pfx_len++] = '0';
      goto send_num;
    case 'i':
      c = 'd';
    case 'd':
      if (addplus)
        prefix[pfx_len++] = '+';
      else if (addspace)
        prefix[pfx_len++] = ' ';
    case 'u':
      radix = 10;
      goto send_num;
    case 'x':
    case 'X':
      radix = 16;
      if (pound)
      {
        prefix[pfx_len++] = '0';
        prefix[pfx_len++] = (char)c;
      }
send_num:
      /*
      ** Now we convert the numeric argument into a string
      */
      value = (is_long) ? va_arg(argp, long) : (long)va_arg(argp, int);
      if (c == 'd')
      {
        if (value < 0)
        {
          prefix[0] = '-';
          pfx_len = 1;
          value = -value;
        }
      }
      else if (!is_long)
        value &= USHORT_MASK;
      arg_str = ultos(value, numstr, radix);
      if (c == 'X') {
	char *ptr = arg_str;
	while ( *ptr ) {
	  if ( islower(*ptr) ) *ptr = toupper(*ptr);
	  ++ptr;
	}
      }
  
      arg_len = strlen(arg_str);
      NON_NEG(prec_cnt, precision - arg_len);
send_it:
      NON_NEG(fill_cnt, minwidth - (pfx_len + prec_cnt + arg_len));
      pfx_str = prefix;
      /*
      ** Need to pad out the field to minimum width and minimum precision.
      **
      ** NOTE: as a side effect of PUTNCHR and PUTNSTR macros, the field
      ** length variable is zeroed after execution.  Therefore, on a field
      ** which can appear in more than one place (i.e., fill and prefix),
      ** we don't have to check in the 2nd call, because the character
      ** count is zero, and the 2nd call will have no effect.
      */
      if (!ljustify)
      {
        char *fill;
        /*
        ** If we are filling with zeros, put out the prefix first
        ** so we don't get stuff like "000-3" or "000xff"
        */
        fill = " ";
        if (zerofill)
        {
          fill = "0";
          PUTNSTR(pfx_str, &pfx_len);
        }
        PUTNCHR(fill, &fill_cnt);
      }
      PUTNSTR(pfx_str, &pfx_len);     /* might do nothing (see note above) */
      PUTNCHR("0", &prec_cnt);
      PUTNSTR(arg_str, &arg_len);
      PUTNCHR(" ", &fill_cnt);        /* might do nothing (see note above) */
      break;

    default:
      /*
      ** Illegal format - ignore the '%' and continue main loop;
      */
      format = arg_str;
      break;
    }
  } /* end of main loop */
  /*
  ** Output tail of format string
  */
  arg_len = strlen(format);
  PUTNSTR(format, &arg_len);

  return (total_count);
}

int
idonprnt(
  doFPrintFunc    i_putn,                 /* String output routine        */
  char            *dest,                  /* A pointer to buffer          */
  int		  bufLen,	          /* Length of buffer in bytes    */
  const char      *format,                /* Format string                */
  va_list         argp                    /* Argument vector pointer      */
  )
{
  register int    c;
  char            numstr[NUMSIZE];        /* Numeric arg string buffer    */
  char            *arg_str;               /* Pointer, mostly to above     */
  char            prefix[3];              /* Numeric arg prefix buffer    */
  char            *pfx_str;               /* Pointer to above             */
  int             precision;              /* Min length for numbers;      */
                                          /* Max length for strings;      */
                                          /* Ignored for characters       */
  int             minwidth;               /* Argument width               */
  long            value;
  int             radix;                  /* Conversion radix             */
  int             ljustify;               /* TRUE for left-justification  */
  int             is_long;
  int             pound;                  /* Special prefix format        */
  int             addspace;               /* Add space left of positive   */
  int             addplus;                /* Add plus left of positive    */
  int             zerofill;               /* '0' or ' ' for fill flag     */
  int             arg_len;                /* Argument field length        */
  int             pfx_len;                /* Prefix field length          */
  int             fill_cnt;               /* Count of fill characters     */
  int             prec_cnt;               /* Count of extra precision 0's */
  int             total_count;            /* Running count of chars sent  */

  total_count = 0;
  --bufLen;			/* leave room for trailing null */
  while ((arg_str = strchr(format, '%')) != NULL)
  {
    arg_len = arg_str - format;
    if ( arg_len > bufLen )
        arg_len = bufLen;
    PUTNSTR(format, &arg_len);
    bufLen -= arg_len;
    format = ++arg_str;
    /*
    ** Handle special case "%%"
    */
    if (*format == '%')
    {
		if ( bufLen > 0 )
		{
		  arg_len = 1;
		  PUTNSTR(format, &arg_len);
		  ++format;
		}
		continue;
    }
    /*
    ** Process flags
    */
    ljustify = addspace = addplus = pound = 0;
    for (;;)
    {
      switch (c = *format++)
      {
      case '-' :                    /* %-  Left justify                 */
        ++ljustify; continue;
      case ' ' :                    /* %_  Leading space on positive    */
        ++addspace; continue;
      case '+' :                    /* %+  Leading '+' on positive      */
        ++addplus; continue;
      case '#' :                    /* %#  Leading "0", "0x" or "0X"    */
        ++pound; continue;
      }
      break;
    }
    /*
    ** Process width field
    */
    zerofill = FALSE;
    if (c == '0')
    {
      c = *format++;
      if (!ljustify) zerofill = TRUE;
    }
    if (c == '*')                   /* %*d  Field width from arg        */
    {
      c = *format++;
      minwidth = va_arg(argp, int);
    }
    else
      ATOI(minwidth);
    /*
    ** Process precision field
    */
    precision = DEF_PREC;           /*      Default precision flag      */
    if (c == '.')                   /*      Precision specified         */
    {
      if ((c = *format++) == '*')   /* %n.* Precision from arg          */
      {
        c = *format++;
        precision = va_arg(argp, int);
      }
      else                          /* %n.n Precision from format str   */
        ATOI(precision);
    }
    /*
    ** Process type modifier field
    */
    is_long = INT_IS_LONG;
    if (c == 'h') {
      c = *format++;
      is_long = FALSE;
    }
    else if (c == 'l')
    {
      c = *format++;
      is_long = TRUE;
    }
    /*
    ** Process conversion chars
    */
    prec_cnt = pfx_len = 0;
    switch (c) {
    case 'n':
      *(va_arg(argp, int*)) = total_count;
      break;
    case 'c':
      numstr[0] = (char)(va_arg(argp, int));
      arg_str = numstr;
      arg_len = 1;
      zerofill = FALSE;
      goto send_it;
    case 's':
      arg_str = va_arg(argp, char *);
      /*
      ** (The precision field limits strings)
      */
      if ( precision < (arg_len = strlen(arg_str))  &&  precision != DEF_PREC )
        arg_len = precision;
      zerofill = FALSE;
      goto send_it;
    case 'o':
      radix = 8;
      if (pound)
        prefix[pfx_len++] = '0';
      goto send_num;
    case 'i':
      c = 'd';
    case 'd':
      if (addplus)
        prefix[pfx_len++] = '+';
      else if (addspace)
        prefix[pfx_len++] = ' ';
    case 'u':
      radix = 10;
      goto send_num;
    case 'x':
    case 'X':
      radix = 16;
      if (pound)
      {
        prefix[pfx_len++] = '0';
        prefix[pfx_len++] = (char)c;
      }
send_num:
      /*
      ** Now we convert the numeric argument into a string
      */
      value = (is_long) ? va_arg(argp, long) : (long)va_arg(argp, int);
      if (c == 'd')
      {
        if (value < 0)
        {
          prefix[0] = '-';
          pfx_len = 1;
          value = -value;
        }
      }
      else if (!is_long)
        value &= USHORT_MASK;
      arg_str = ultos(value, numstr, radix);
      if (c == 'X') {
	char *ptr = arg_str;
	while ( *ptr ) {
	  if ( islower(*ptr) ) *ptr = toupper(*ptr);
	  ++ptr;
	}
      }
  
      arg_len = strlen(arg_str);
      NON_NEG(prec_cnt, precision - arg_len);
send_it:
      NON_NEG(fill_cnt, minwidth - (pfx_len + prec_cnt + arg_len));
      pfx_str = prefix;
      /*
      ** Need to pad out the field to minimum width and minimum precision.
      **
      ** NOTE: as a side effect of PUTNCHR and PUTNSTR macros, the field
      ** length variable is zeroed after execution.  Therefore, on a field
      ** which can appear in more than one place (i.e., fill and prefix),
      ** we don't have to check in the 2nd call, because the character
      ** count is zero, and the 2nd call will have no effect.
      */
      if (!ljustify)
      {
        char *fill;
        /*
        ** If we are filling with zeros, put out the prefix first
        ** so we don't get stuff like "000-3" or "000xff"
        */
        fill = " ";
        if (zerofill)
        {
          fill = "0";
	  if ( pfx_len > bufLen )
	      pfx_len = bufLen;
	  PUTNSTR(pfx_str, &pfx_len);
	  bufLen -= pfx_len;
        }
	if ( fill_cnt > bufLen )
		fill_cnt = bufLen;
	PUTNCHR(fill, &fill_cnt);
	bufLen -= fill_cnt;
      }
      if ( pfx_len > bufLen )
	  pfx_len -= bufLen;
      PUTNSTR(pfx_str, &pfx_len);     /* might do nothing (see note above) */
      bufLen -= pfx_len;
      if ( prec_cnt > bufLen )
	  prec_cnt = bufLen;
      PUTNCHR("0", &prec_cnt);
      bufLen -= prec_cnt;
      if ( arg_len > bufLen )
	  arg_len = bufLen;
      PUTNSTR(arg_str, &arg_len);
      bufLen -= arg_len;
      if ( fill_cnt > bufLen )
	  fill_cnt = bufLen;
      PUTNCHR(" ", &fill_cnt);        /* might do nothing (see note above) */
      bufLen -= fill_cnt;
      break;

    default:
      /*
      ** Illegal format - ignore the '%' and continue main loop;
      */
      format = arg_str;
      break;
    }
  } /* end of main loop */
  /*
  ** Output tail of format string
  */
  arg_len = strlen(format);
  if ( arg_len > bufLen )
	  arg_len = bufLen;
  PUTNSTR(format, &arg_len);
  arg_len = 1;
  PUTNCHR("\000",&arg_len);
  --total_count;
  bufLen -= arg_len;
  return (total_count);
}
