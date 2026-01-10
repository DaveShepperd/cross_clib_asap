/*  ULTOS.C  -  Unsigned long to string conversion routine
**
**  For use with the printf() family of routines.
**
**  Author:  Lyle V. Rains
**  This program is placed in the public domain by the author
*/

#include <limits.h>
/* you need CHAR_BIT for the following: */

#define MAXLEN ((CHAR_BIT * sizeof(long))+1)
/* (33?) max no of chars in binary number conversion */

static const char digits[] = "0123456789abcdefghijklmnopqrstuvwxyz";

char *
ultos( value, string, radix )
  unsigned long value;
  char *string;
  int radix;
{
  char tmp[MAXLEN];
  register char *c1;

  if ( radix == 10 ) {
    __ultodec(value,string);
    return string;
  }
  if (2 <= radix && radix <= 36)
  {
    {
      
      *(c1 = &tmp[MAXLEN-1]) = '\0';
      if ( (radix & (radix-1)) ) {
	/* non-binary radix */
	do
	{
	  /* 
	  ** Calculate the digits in reverse order
	  */
	  *--c1 = digits[value % radix];
	}
	while (value /= radix);
      }	else {
	/* binary radix */
	int shift;
	for ( shift = 1 ; radix > (1 << shift) ; ++shift) {;}
	radix -= 1;
	do {
	  *--c1 = digits[value & radix];
	}
	while ( value >>= shift );
      }
    }
    /*
    ** At this point, we have the converted string in tmp.
    ** with c1 pointing at the most significant digit of the string.
    */ 
    strcpy(string,c1);
  }
  return (string);
}
