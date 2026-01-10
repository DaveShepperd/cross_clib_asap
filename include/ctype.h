#define _L (32)
#define _U (16)
#define _D (4)
#define _X (8)
#define _P (2)
#define _S (1)
#define _W (64)
#define _K (128)

extern unsigned char _ctype_[257];

#define isalnum(x) (_ctype_[(x)+1] & (_L|_U|_D))
#define isalpha(x) (_ctype_[(x)+1] & (_L|_U))
#define iscntrl(x) (_ctype_[(x)+1] & (_K|_W))
#define isdigit(x) (_ctype_[(x)+1] & (_D))
#define isgraph(x) (_ctype_[(x)+1] & (_D|_L|_P|_U))
#define islower(x) (_ctype_[(x)+1] & (_L))
#define isprint(x) (_ctype_[(x)+1] & (_D|_L|_P|_S|_U))
#define ispunct(x) (_ctype_[(x)+1] & (_P))
#define isspace(x) (_ctype_[(x)+1] & (_S|_W))
#define isupper(x) (_ctype_[(x)+1] & (_U))
#define isxdigit(x) (_ctype_[(x)+1] & (_D|_X))
extern int toupper();
extern int tolower();
