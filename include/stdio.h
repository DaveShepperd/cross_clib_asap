#ifndef __STDIO_DEFINED
#define __STDIO_DEFINED

/*      stdio.h replacement for Asap Development System. This file
*       contains (only) enough of the definitions for Standard I/O
*       to allow not-particulary-demanding programs to use the two
*       uarts on the ADS for i/o. Other features will appear as
*       needed/written MEA 1DEC1989
*/

typedef int FILE;
extern FILE ctlchan;

extern FILE *stdin;
extern FILE *stdout;
extern FILE *stderr;

#ifndef NULL
#define NULL            ((void *)0)
#endif
#define EOF             (-1)

extern int  fputc(int ch, FILE *fp);
extern char *fgets(char *buf, int len, FILE *fp);
extern long ftell(FILE *fp);
extern int  fputs(const char *str, FILE *fp);
extern int  fflush(FILE *fp);
extern int  ifprintf(FILE *fp, const char *buf, ...);

#if 0
extern int  ivfprintf(FILE *fp, const char *buf, va_list args);
#endif

#define putc fputc
#define putchar(x) fputc(x,stdout)
#define puts(x) do { fputs(x,stdout); fputs("\n",stdout); } while (0))
#define printf iprintf
#define fprintf ifprintf
#define vfprintf ivfprintf

#endif /* __STDIO_DEFINED */
