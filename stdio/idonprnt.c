/*
**    IDONPRINT.C : integer, char, string only version of _doprint()
*/

#ifdef  DOCUMENTATION

Synopsis int idonprnt( va_list args );

Description idoprnt() performs all formatting operations for iprintf()
and related formatted output functions.
Author
Lyle V. Rains
This program is placed in the public domain.
Hacked up a little bit by Dave Shepperd, 2026-01-11

#endif

#include <idoprnt.h>

int _idonprnt( va_list argp )              /* Argument vector pointer      */
{
	int    c;
	char            numstr[NUMSIZE];        /* Numeric arg string buffer    */
	char            *arg_str;               /* Pointer, mostly to above     */
	char            prefix[3];              /* Numeric arg prefix buffer    */
	char            *pfx_str;               /* Pointer to above             */
	int             total_count;            /* Running count of chars sent  */
	long            value;
	ArgPrefix_t     argPrefix;
	doFPrintFunc    i_putn;                 /* String output routine        */
	char            *dest;                  /* A pointer to buffer          */
	int		    bufLen;                 /* Length of buffer in bytes    */
	const char      *format;                /* Format string                */

	i_putn = (doFPrintFunc)va_arg(argp,doFPrintFunc);
	dest = va_arg(argp,char *);
	bufLen = va_arg(argp,int);
	format = va_arg(argp,const char *);
	total_count = 0;
	--bufLen;         /* leave room for trailing null */
	while ( (arg_str = strchr(format, '%')) != NULL )
	{
		prefix[0] = 0;
		prefix[1] = 0;
		prefix[2] = 0;
		argPrefix.arg_len = arg_str - format;
		if ( argPrefix.arg_len > bufLen )
			argPrefix.arg_len = bufLen;
		if ( argPrefix.arg_len )
			PUTNSTR(format, &argPrefix.arg_len);
		bufLen -= argPrefix.arg_len;
		format = ++arg_str;
		/*
		** Handle special case "%%"
		*/
		if ( *format == '%' )
		{
		    if ( bufLen > 0 )
		    {
			argPrefix.arg_len = 1;
			PUTNSTR(format, &argPrefix.arg_len);
			++format;
		    }
		    continue;
		}
		/*
		** Process flags
		*/
        argPrefix.ljustify = argPrefix.addspace = argPrefix.addplus = argPrefix.pound = 0;
		for (;;)
		{
			switch ( (c = *format++) )
			{
			case '-' :                    /* %-  Left justify                 */
				++argPrefix.ljustify;
				continue;
			case ' ' :                    /* %_  Leading space on positive    */
				++argPrefix.addspace;
				continue;
			case '+' :                    /* %+  Leading '+' on positive      */
				++argPrefix.addplus;
				continue;
			case '#' :                    /* %#  Leading "0", "0x" or "0X"    */
				++argPrefix.pound;
				continue;
			default:
				break;
			}
			break;
		}
		/*
		** Process width field
		*/
		argPrefix.zerofill = FALSE;
		if ( c == '0' )
		{
			c = *format++;
			if ( !argPrefix.ljustify )
				argPrefix.zerofill = TRUE;
		}
		if ( c == '*' )                   /* %*d  Field width from arg        */
		{
			c = *format++;
			argPrefix.minwidth = va_arg(argp, int);
		}
		else
			ATOI(argPrefix.minwidth);
		/*
		** Process precision field
		*/
		argPrefix.precision = DEF_PREC;           /*      Default precision flag      */
		if ( c == '.' )                   /*      Precision specified         */
		{
			if ( (c = *format++) == '*' )   /* %n.* Precision from arg          */
			{
				c = *format++;
				argPrefix.precision = va_arg(argp, int);
			}
			else                          /* %n.n Precision from format str   */
				ATOI(argPrefix.precision);
		}
		/*
		** Process type modifier field
		*/
		argPrefix.is_long = INT_IS_LONG;
		if ( c == 'h' )
		{
			c = *format++;
			argPrefix.is_long = FALSE;
		}
		else if ( c == 'l' )
		{
			c = *format++;
			argPrefix.is_long = TRUE;
		}
		/*
		** Process conversion chars
		*/
        argPrefix.prec_cnt = argPrefix.pfx_len = 0;
		switch (c)
		{
		case 'n':
			*(va_arg(argp, int*)) = total_count;
			break;
		case 'c':
			numstr[0] = (char)(va_arg(argp, int));
			arg_str = numstr;
			argPrefix.arg_len = 1;
            argPrefix.zerofill = FALSE; 
			goto send_it;
		case 's':
			arg_str = va_arg(argp, char *);
			/*
			** (The precision field limits strings)
			*/
			if ( argPrefix.precision < (argPrefix.arg_len = strlen(arg_str)) &&  argPrefix.precision != DEF_PREC )
				argPrefix.arg_len = argPrefix.precision;
			argPrefix.zerofill = FALSE;
			goto send_it;
		case 'o':
            argPrefix.radix = 8;
			if ( argPrefix.pound )
				prefix[argPrefix.pfx_len++] = '0';
			goto send_num;
		case 'i':
			c = 'd';
		case 'd':
			if ( argPrefix.addplus )
				prefix[argPrefix.pfx_len++] = '+';
			else if ( argPrefix.addspace )
				prefix[argPrefix.pfx_len++] = ' ';
		case 'u':
			argPrefix.radix = 10;
			goto send_num;
		case 'x':
		case 'X':
			argPrefix.radix = 16;
			if ( argPrefix.pound )
			{
				prefix[argPrefix.pfx_len++] = '0';
				prefix[argPrefix.pfx_len++] = (char)c;
			}
		send_num:
			/*
			** Now we convert the numeric argument into a string
			*/
			value = (argPrefix.is_long) ? va_arg(argp, long) : (long)va_arg(argp, int);
			if ( c == 'd' )
			{
				if ( value < 0 )
				{
					prefix[0] = '-';
					argPrefix.pfx_len = 1;
					value = -value;
				}
			}
			else if ( !argPrefix.is_long )
				value &= USHORT_MASK;
			arg_str = ultos(value, numstr, argPrefix.radix);
			if ( c == 'X' )
			{
				char *ptr = arg_str;
				while ( *ptr )
				{
					if ( islower(*ptr) )
						*ptr = toupper(*ptr);
					++ptr;
				}
			}

			argPrefix.arg_len = strlen(arg_str);
			NON_NEG(argPrefix.prec_cnt, argPrefix.precision - argPrefix.arg_len);
		send_it:
			NON_NEG(argPrefix.fill_cnt, argPrefix.minwidth - (argPrefix.pfx_len + argPrefix.prec_cnt + argPrefix.arg_len));
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
			if ( !argPrefix.ljustify )
			{
				char *fill;
				/*
				** If we are filling with zeros, put out the prefix first
				** so we don't get stuff like "000-3" or "000xff"
				*/
				fill = " ";
				if ( argPrefix.zerofill )
				{
					fill = "0";
					if ( argPrefix.pfx_len > bufLen )
						argPrefix.pfx_len = bufLen;
					PUTNSTR(pfx_str, &argPrefix.pfx_len);
					bufLen -= argPrefix.pfx_len;
				}
				if ( argPrefix.fill_cnt > bufLen )
					argPrefix.fill_cnt = bufLen;
				PUTNCHR(fill, &argPrefix.fill_cnt);
				bufLen -= argPrefix.fill_cnt;
			}
			if ( argPrefix.pfx_len > bufLen )
				argPrefix.pfx_len -= bufLen;
			PUTNSTR(pfx_str, &argPrefix.pfx_len);     /* might do nothing (see note above) */
			bufLen -= argPrefix.pfx_len;
			if ( argPrefix.prec_cnt > bufLen )
				argPrefix.prec_cnt = bufLen;
			PUTNCHR("0", &argPrefix.prec_cnt);
			bufLen -= argPrefix.prec_cnt;
			if ( argPrefix.arg_len > bufLen )
				argPrefix.arg_len = bufLen;
			PUTNSTR(arg_str, &argPrefix.arg_len);
			bufLen -= argPrefix.arg_len;
			if ( argPrefix.fill_cnt > bufLen )
				argPrefix.fill_cnt = bufLen;
			PUTNCHR(" ", &argPrefix.fill_cnt);        /* might do nothing (see note above) */
			bufLen -= argPrefix.fill_cnt;
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
	argPrefix.arg_len = strlen(format);
	if ( argPrefix.arg_len > bufLen )
		argPrefix.arg_len = bufLen;
	if ( argPrefix.arg_len )
		PUTNSTR(format, &argPrefix.arg_len);
	argPrefix.arg_len = 1;
	PUTNCHR("\000", &argPrefix.arg_len);
	--total_count;
	return (total_count);
}
