/*	This file contains two implementations of the "Minimal Standard"
*	random number generator discussed in CACM vol 31 #10 (October 1988)
*	in the article by Stephen K. Park and Keith W. Miller:
*	"Random Number Generators: good ones are hard to find"
*
*	It is a linear congruential generator of the form:
*		seed = (seed*a)%m
*	where (in this case) a = 16807 (= 7^5) and m=(2^31-1)
*
*	In this particular file, we have jiggered the handling of "seed"
*	so that the Ansi requirements of "behaving as if the target
*	environment calls srand(1) at startup" can be handled by the
*	initialization of seed to 0. This also has the (benificial?) effect
*	of ensuring that 0 is included in the range of outputs, although
*	a seed value of 0 would cause lock-up. The down-side is that RAND_MAX
*	becomes 2^31-3. This is because:
*		m = 2^31-1, hence:
*		seed < 2^31-1 	( or seed <= 2^31-2) hence:
*		(seed-1) < 2^31-2 ( or (seed-1) <= 2^31-3 )
*
*/

#ifdef DEBUG
#include <stdio.h>
static unsigned long ref_seed;
#endif

static unsigned long seed;

#define A (16807)		/* 7**5 */
#define M (2147483647)		/* 2**31-1 */

#ifdef DEBUG

/*	This is the implementation described in the article.
*	It includes a clever method for avoiding overflow but requires at
*	least one (possibly two, for a dumb compiler) divides.
*		This is a transliteration from the Pascal source, and runs
*	correctly on a 80386 machine when compiled with GCC 1.37
*/
#define Q (M/A)			/* should end up 127773 */
#define R (M%A)			/* should end up 2836 */

int ref_rand()
{

    unsigned long hi,lo;
    long test;

    hi = ref_seed / Q;
    lo = ref_seed % Q;

    test = (A * lo) - (R * hi);
    if ( test > 0 ) ref_seed = test;
    else ref_seed = test + M;
}
#endif

void
rand()
{
    unsigned long hprod,lprod;

    /* first do two 16x16 multiplies */
    hprod = ((seed>>16)&0xFFFF) * 16807;
    lprod = (seed&0xFFFF)*16807;
#if DEBUG > 1
    fprintf(stderr,"seed = %X\thprod = %X\tlprod = %X\n",seed,hprod,lprod);
#endif
    /* combine the products (suitably shifted) to form 48-bit product,
    *  with bottom 32 bits in lprod and top 16 bits in hprod.
    */
    hprod += ((lprod>>16) & 0xFFFF);
    lprod = (lprod&0xFFFF)|((hprod&0xFFFF)<<16);
    hprod >>= 16;
#if DEBUG > 1
    fprintf(stderr,"sum to\thprod = %X\tlprod = %X\n",hprod,lprod);
#endif
    /* now subtract the top 17 bits from the bottom 31 bits to implement
    *  a deferred "end-around carry".
    */
    hprod = hprod + hprod + ((lprod>>31)&1);
    lprod += hprod;
#if DEBUG > 1
    fprintf(stderr,"e-o-c\thprod = %X\tlprod = %X\n",hprod,lprod);
#endif
    /* final "and" gives modulo(2^31-1) */
    seed = lprod & 0x7FFFFFFF;
}

#if DEBUG > 1
#define LOOPLIM (6)
#else
#define LOOPLIM (10002)
#endif

#if 0
main()
{
    int i;
    seed = 1;
#ifdef DEBUG
    ref_seed = seed;
#endif
    for ( i = 1 ; i < LOOPLIM ; ++i ) {
	if ( (i > 9995) || (i < 10)) {
#ifdef DEBUG
	    printf("ref_seed[%d] = %u\t",i,ref_seed);
#endif
	    printf("seed[%d] = %u\n",i,seed);
	}
#ifdef DEBUG
	ref_rand();
#endif
	rand();
    }
}
#endif
