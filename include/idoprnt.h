#ifndef _IDOPRNT_H_
#define _IDOPRNT_H_

#include <stdio.h>
#include <limits.h>
#include <ctype.h>
#include <string.h>
#include <stdarg.h>
#include <stdint.h>
#include <varargs.h>

extern char *ultos( uint32_t value, char *string, int radix);

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

#define PUTNSTR(strPtr, cntPtr) if ( *(cntPtr) ) i_putn((void *)&dest, (strPtr), (cntPtr), &total_count, 0)

#define PUTNCHR(strPtr, cntPtr) if ( *(cntPtr) ) i_putn((void *)&dest, (strPtr), (cntPtr), &total_count, 1)

typedef struct
{
	int             precision;              /* Min length for numbers;      */
	/* Max length for strings;      */
	/* Ignored for characters       */
	int             minwidth;               /* Argument width               */
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
} ArgPrefix_t;

extern int _idoprnt( va_list argp );	/* Used by printf and fprintf */
extern int _idonprnt( va_list argp );	/* Used by sprintf and snprintf */

#endif	/* _IDOPRNT_H_ */
