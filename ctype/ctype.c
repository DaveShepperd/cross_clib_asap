/* The following code is fast and correct for ASCII, but takes advantage
* of the assignment of the "islower" bit to the offset between upper and
* lower case in ASCII. It probably won't be near as neat in EBCDIC :-)
* The "isupper" bit is similarly one-half the offset, so a machine that
* could do shift-and-add could have just as fast a tolower. On the 68000,
* we accept an extra add-to-self, which is still cheaper than a branch
* Also note that compilers that default to signed characters will give,
* uh, _interesting_ results when these are naively used (no cast to unsigned
* char) with characters that have the high bit set. It was that or screw-up
* the handling of the (legal) input EOF.
*/

#include <ctype.h>

#if (_L != 32) || (_U != 16)
#error not using this 'C' file with correct ctype.h
#endif

#define DX (_D|_X)
#define UX (_U|_X)
#define LX (_L|_X)

#ifndef JUST_SEVEN_CHAR_BITS
#define ENTRIES (257)
#else
#define ENTRIES (129)
#endif

unsigned char _ctype_[ENTRIES] = {
	0,			/* EOF */
	_K, _K, _K, _K, _K, _K, _K, _K, /* NUL SOH STX ETX EOT ENQ ACK BEL */
	_K, _W, _W, _W, _W, _W, _K, _K, /* BS  HT  LF  VT  FF  CR  SO  SI  */
	_K, _K, _K, _K, _K, _K, _K, _K, /* DLE DC1 DC2 DC3 DC4 NAK SYN ETB */
	_K, _K, _K, _K, _K, _K, _K, _K, /* CAN EM  SUB ESC FS  GS  RS  US  */
	_S, _P, _P, _P, _P, _P, _P, _P, /* spc !   "   #   $   %   &   '   */
	_P, _P, _P, _P, _P, _P, _P, _P, /* (   )   *   +   ,   -   .   /   */
	DX, DX, DX, DX, DX, DX, DX, DX, /* 0   1   2   3   4   5   6   7   */
	DX, DX, _P, _P, _P, _P, _P, _P, /* 8   9   ;   :   <   =   >   ?   */
	_P, UX, UX, UX, UX, UX, UX, _U, /* @   A   B   C   D   E   F   G   */
	_U, _U, _U, _U, _U, _U, _U, _U, /* H   I   J   K   L   M   N   O   */
	_U, _U, _U, _U, _U, _U, _U, _U, /* P   Q   R   S   T   U   V   W   */
	_U, _U, _U, _P, _P, _P, _P, _P, /* X   Y   Z   [   \   ]   ^   _   */
	_P, LX, LX, LX, LX, LX, LX, _L, /* '   a   b   c   d   e   f   g   */
	_L, _L, _L, _L, _L, _L, _L, _L, /* h   i   j   k   l   m   n   o   */
	_L, _L, _L, _L, _L, _L, _L, _L, /* p   q   r   s   t   u   v   w   */
	_L, _L, _L, _P, _P, _P, _P, _K  /* x   y   z   {   |   }   ~   del */
#ifndef JUST_SEVEN_CHAR_BITS
	,0,  0,  0,  0,  0,  0,  0,  0, /* <eight-bit chars> 80-87	   */
	 0,  0,  0,  0,  0,  0,  0,  0, /* <eight-bit chars> 89-8F	   */
	 0,  0,  0,  0,  0,  0,  0,  0, /* <eight-bit chars> 90-97	   */
	 0,  0,  0,  0,  0,  0,  0,  0, /* <eight-bit chars> 98-9F	   */
	 0,  0,  0,  0,  0,  0,  0,  0, /* <eight-bit chars> A0-A7	   */
	 0,  0,  0,  0,  0,  0,  0,  0, /* <eight-bit chars> A8-AF	   */
	 0,  0,  0,  0,  0,  0,  0,  0, /* <eight-bit chars> B0-B7	   */
	 0,  0,  0,  0,  0,  0,  0,  0, /* <eight-bit chars> B8-BF	   */
	 0,  0,  0,  0,  0,  0,  0,  0, /* <eight-bit chars> C0-C7	   */
	 0,  0,  0,  0,  0,  0,  0,  0, /* <eight-bit chars> C8-CF	   */
	 0,  0,  0,  0,  0,  0,  0,  0, /* <eight-bit chars> D0-D7	   */
	 0,  0,  0,  0,  0,  0,  0,  0, /* <eight-bit chars> D8-DF	   */
	 0,  0,  0,  0,  0,  0,  0,  0, /* <eight-bit chars> E0-E7	   */
	 0,  0,  0,  0,  0,  0,  0,  0, /* <eight-bit chars> E8-EF	   */
	 0,  0,  0,  0,  0,  0,  0,  0, /* <eight-bit chars> F0-F7	   */
	 0,  0,  0,  0,  0,  0,  0,  0  /* <eight-bit chars> F8-FF	   */
#endif
};

int
toupper(inch)
int inch;
{
    return inch - (_ctype_[inch+1]&_L);
}

int
tolower(inch)
int inch;
{
    return inch + ((_ctype_[inch+1]&_U)<<1);
}
