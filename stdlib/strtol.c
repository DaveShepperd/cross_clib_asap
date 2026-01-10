/* #define STAND_ALONE 1 */
#define _SLOW_MULTIPLY_
#define _DECIMAL_SPECIAL_CASE_
#define _ASCII_CHARS_
/* #define _EBCDIC_CHARS_ */

/*	This file contains strtol() and strtoul(), as well as "wrappers"
*	that implement atoi() and atol() and a static routine that is
*	the "core" of the conversions. It also contains a "stand alone"
*	main() for testing.
*	It is assumed that the target machine is binary and one of:
*	two's-complement, one's-complement, or sign-magnitude, as this
*	is pretty much forced by the definition of C. It runs a bit faster
*	if the character code is either ASCII or EBCDIC, and assumes if the
*	latter that chars are default unsigned (I cannot imagine a conforming
*	implementation with signed, 8-bit chars and EBCDIC). The places where
*	character set matters are within "#ifdef _UNKNOWN_CHARS_". The character
*	set has to be explicitly spec'd (or default to slow "UNKNOWN") because
*	the pre-processor is not supposed to know the values of the _target_
*	character set.
*
*	I hope it is _not_ assumed that:
*	   word length is 2^n
*	   two's complement
*	   ascii
*	so let me (albaugh@dms.uucp) know if it breaks on your machine.
*	You should also be aware that some compilers lie in limits.h
*	(e.g. MicroSoft, DEC), so before you flame me, check it out.
*	   This code is copyright by Michael Albaugh 1989. Anyone may do
*	anything to it with the sole exceptions of removing this notice or
*	attaching further restrictions.
*/

/* QUESTION: should we keep eating digits after an overflow, so endptr will
*  be correct?  I assume so, but if not:
* #define BARF_EARLY break;
*/
#define BARF_EARLY

#define MAX_RADIX 36

#include <ctype.h>
#include <limits.h>

#include <errno.h>

#ifdef STAND_ALONE
#include <stdio.h>
/* I would #include <stdlib.h> here but it is broken on my (vms) system */
#define EXIT_SUCCESS 1
extern long strtol();
extern unsigned long strtoul();

extern int strncmp();
#else
#define my_strtol strtol
#define my_strtoul strtoul
#endif

#if (LONG_MIN + LONG_MAX) != 0
/* a scrupulous two's complement implementation has one more negative number
*  than positive. We need to take this into account in a few places
*/
#if (LONG_MIN + LONG_MAX + 1) == 0
#define TRUE_TWOS 1
#else
/* I have _no_ idea what this kind of arithmetic this is
*/
#error unknown number system
#endif
#else
/* one's complement, sign-magnitude, or sloppy compilers */
#define TRUE_TWOS 0
#endif

#define _UNKNOWN_CHARS_
#ifdef _ASCII_CHARS_
#undef _UNKNOWN_CHARS_
#endif

#ifdef _EBCDIC_CHARS_
#undef _UNKNOWN_CHARS_
#endif

#ifdef _UNKNOWN_CHARS_
static const char alphas[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
extern char *strchr();
#endif

#define UNSIGNED (4)
#define OVERFLOW (2)
#define NEGATIVE (1)

/*	x_str_conv() is the "guts" of strtol() and strtoul().
*	It is called with the original parameters plus an "unsigned"
*	indicator.
*/
static int
x_str_conv(start, endptr, base, is_unsigned)
char *start,**endptr;
int base, is_unsigned;
{
    register char *wp;
    register int digit;
    unsigned long accum;
    unsigned pre_over;
    int flags = 0;
    char *first_digit;
#ifdef _UNKNOWN_CHARS_
    char *cp;
#endif

    /* check for valid base. */
    if ( (base < 0) || (base > MAX_RADIX) || (base == 1 ) ) {
	/* QUESTION: although not spec'd, should we produce an error here */
	base = 0;
    }

/* skip whitespace and definitely establish base */
    wp = start;
    while ( (digit = *wp) && isspace(digit) ) {++wp;}
    flags = 0;
    if ( digit == '-' ) {
	flags = NEGATIVE;	/* use lsb for -/+ indication */
	digit = *++wp;
    }
    else if ( digit == '+' ) {
	digit = *++wp;
    }
    if ( (base == 0) && ( digit == '0' ) ) {
	/* base could be octal or hex */
	if ( (wp[1] == 'x') || (wp[1] == 'X') ) base = 16;
	else base = 8;
    }
    if ( base == 0 ) base = 10;		/* all else fails, use decimal */

    accum = 0;
    if ( (base == 16) && (digit == '0') && ((wp[1] == 'x') || (wp[1] == 'X'))) {
	wp += 2;	/* skip over "0x" */
    }

    first_digit = wp;		/* remember first place that could be a digit */

#ifdef _DECIMAL_SPECIAL_CASE_
    if ( base == 10 ) {
	/* code for base=10. This is separate because it is so common
	*  that multiply-avoidance and compile-time constant-folding can
	*  be a big win. If code-space is a bigger concern, you should
	*  not #define _DECIMAL_SPECIAL_CASE_
	*/
	pre_over = ULONG_MAX/10;
	while ( 1 ) {
	    digit = *wp++;
	    if ( !isdigit(digit) ) break;
	    if ( accum > pre_over ) {
		flags |= OVERFLOW;
		BARF_EARLY
	    }
	    else {
		accum *= 10;
		digit -= '0';
		/* we could still be close to the edge... */
		if ( (unsigned long)(ULONG_MAX -accum) < (unsigned long)digit) {
		    flags |= OVERFLOW;
		    BARF_EARLY
		}
		else accum += digit;
	    } /* end if ( "unable to multiply" ) */
	} /* end while (1) */
    }	/* end if base == 10 */
    else
#endif
    {
	/* generic code, used for base != 10 */

#ifdef _SLOW_MULTIPLY_
	/* on some machines, such as the 68K and many RISCs, multiply is
	*  slow compared to shifting. If so, the following optimization
	*  could help.
	*/
	int shift_cnt;			/* also used as "flag" for << vs * */

	shift_cnt = 0;			/* default to no shift */
	if ( (base & (base-1)) == 0 ) {
	    /* base has only one bit set, replace multiply with shift */
	    for ( digit = 1 ; base != digit ; ++shift_cnt, digit <<= 1 ) {;}
	    pre_over = ULONG_MAX >> shift_cnt;
	} else pre_over = ULONG_MAX / base;
#else
	pre_over = ULONG_MAX / base;
#endif

	while ( 1 ) {
	    digit = *wp++;
	    if ( isdigit(digit) ) digit -= '0';
	    else if ( (base > 10 ) && isalpha(digit) ) {
		if ( islower(digit) ) digit = toupper(digit);
#ifdef _UNKNOWN_CHARS_
		/* If character set is completely unknown, this is the
		*  best technique I have seen.
		*/
		if ( (cp = strchr(alphas,digit)) == 0) digit = MAX_RADIX;
		digit = (cp - alphas) + 10;
#else
#ifdef _EBCDIC_CHARS_
		/* If character set is ebcdic, we need to worry about
		*  non-contiguous alphas.
		*/
		if ( digit > 'R' ) digit -= 'S'-'R'-1;
		if ( digit > 'I' ) digit -= 'J'-'I'-1;
#endif							/* _EBCDIC_CHARS_ */
		digit -= 'A'-10;
#endif							/* _UNKNOWN_CHARS_ */
	    }
	    else digit = MAX_RADIX;	 /* default to always bogus value */
	    if ( digit >= base ) break;	/* found un-digit */
	    if ( accum > pre_over ) {
		flags |= OVERFLOW;
		BARF_EARLY
	    }
	    else {
#ifdef _SLOW_MULTIPLY_
		if ( shift_cnt != 0 ) accum <<= shift_cnt;
		else
#endif
		accum *= base;
		/* we could still be close to the edge... */
		if ( (unsigned long)(ULONG_MAX -accum) < (unsigned long)digit) {
		    flags |= OVERFLOW;
		    BARF_EARLY
		}
		else accum += digit;
	    } /* end if ( "unable to multiply" ) */
	} /* end while (1) */
    }	/* end of "else" that applies for all "generic" radices */

    if ( --wp == first_digit ) wp = start; /* "backspace" over leading cruft */
    if ( endptr ) *endptr = wp;
    if ( is_unsigned ) {
	if ( flags & OVERFLOW ) {
	    accum = ULONG_MAX;
	    errno = ERANGE;
	} else if ( flags & NEGATIVE ) {
	    /* YES! even for unsigned!! */
	    accum = -accum;
	}
    } else {
	/* signed numbers require a bit of care */
	if ( flags & OVERFLOW ) {
	    errno = ERANGE;
	    if ( flags & NEGATIVE ) accum = LONG_MIN;
	    else accum = LONG_MAX;
	}
	else if ( flags & NEGATIVE ) {
	    if ( accum > (unsigned long)-(LONG_MIN)) { 
		errno = ERANGE;
		accum = LONG_MIN;
	    } else accum = -accum;
	}
	else if ( accum > LONG_MAX ) {
	    accum = LONG_MAX;
	    errno = ERANGE;
	}
    }
    return accum;
}

#ifdef STAND_ALONE
static
#endif
long
my_strtol(start, endptr, base)
char *start,**endptr;
int base;
{
    return x_str_conv(start, endptr, base, 0);
}

#ifdef STAND_ALONE
static
#endif

unsigned long
my_strtoul(start, endptr, base)
char *start,**endptr;
int base;
{
    return x_str_conv(start, endptr, base, 1);
}

#ifndef STAND_ALONE
long
atol(string)
char *string;
{
    return x_str_conv(string, (char **)0, 10, 0);
}

int
atoi(string)
char *string;
{
    long val;
    val = x_str_conv(string, (char **)0, 10, 0);
    if ( val < INT_MIN ) val = INT_MIN;
    if ( val > INT_MAX ) val = INT_MAX;
    return (int)val;
}
#endif

#ifdef STAND_ALONE

void
str_rept(start,end)
char *start,*end;
{
    int tok;
    fprintf(stderr,"\n%s\n",start);
    while ( ((tok = *start) != 0) && (start++ != end) ) {
	if ( tok == '\t' ) fputc('\t',stderr);
	else fputc(' ',stderr);
    }
    fputc('^',stderr);
} 

int
main(argc,argv)
int argc;
char **argv;
{
    char *wp,*ep;
    long val;
    unsigned long uval;
    int errcode,radix;
    radix = 0;
    while ( --argc ) {
	wp = *++argv;
	errno = errcode = 0;
	val = my_strtol(wp,&ep,radix);
	errcode = errno;
	if ( ep == wp ) {
	    if ( !strncmp(wp,"rad=",4) ) {
		radix = strtol(wp+4,0,10);
		continue;
	    }
	    else fprintf(stderr,"%s is not a number\n",wp);
	}
	else if ( errcode == 0 ){
	    str_rept(wp,ep);
	    fprintf(stderr,"-- has value %ld (0x%lX)\n",val,val);
	} else {
	    str_rept(wp,ep);
	    fprintf(stderr,"-- overflows, with value %ld (0x%lX)\n",val,val);
	}
	errno = errcode = 0;
	uval = my_strtoul(wp,&ep,radix);
	errcode = errno;
	if ( ep == wp ) {
	    fprintf(stderr,"is not an unsigned number\n",wp);
	}
	else if ( errcode == 0 ){
	    str_rept(wp,ep);
	    fprintf(stderr,"-- has value %lu (0x%lX)\n",uval,uval);
	} else {
	    str_rept(wp,ep);
	    fprintf(stderr,"-- overflows, with value %lu (0x%lX)\n",uval,uval);
	}
	fputs("host strtol says:",stderr);
	errno = errcode = 0;
	val = strtol(wp,&ep,radix);
	errcode = errno;
	if ( ep == wp ) {
	    fprintf(stderr,"%s is not a number\n",wp);
	}
	else if ( errcode == 0 ){
	    str_rept(wp,ep);
	    fprintf(stderr,"-- has value %ld (0x%lX)\n",val,val);
	} else {
	    str_rept(wp,ep);
	    fprintf(stderr,"-- overflows, with value %ld (0x%lX)\n",val,val);
	}
	fputs("host strtoul says:",stderr);
	errno = errcode = 0;
	uval = strtoul(wp,&ep,radix);
	errcode = errno;
	if ( ep == wp ) {
	    fprintf(stderr,"is not an unsigned number\n",wp);
	}
	else if ( errcode == 0 ){
	    str_rept(wp,ep);
	    fprintf(stderr,"-- has value %lu (0x%lX)\n",uval,uval);
	} else {
	    str_rept(wp,ep);
	    fprintf(stderr,"-- overflows, with value %lu (0x%lX)\n",uval,uval);
	}
    }
    return EXIT_SUCCESS;
}
#endif
