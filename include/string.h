#ifndef _STRING_H_
#define _STRING_H_

#ifndef NULL
	#define NULL ((void *)0)
#endif

#ifndef __size_t_DEF__
	#define	__size_t_DEF__	1
	#ifdef 	_SYSTEM_size_t_HEADER_
		#include	_SYSTEM_size_t_HEADER_
	#else
		#include <size_t.h>
	#endif
#endif
#include <stdio.h>
#include <varargs.h>

/* In string/mem_func.mac */
extern	void* memchr(const void *ptr, int chr, int size);
extern	int   memcmp(const void *ptr1, void *ptr2, int size);
extern	void* memcpy(void *dst, const void *src, int size);
extern	void* memmove(void *dst, const void *src, int size);
extern	void* memset(void *dst, int num, int size);

/* In string/simple_str.c */
extern	  char* strcat(char *dst, const char *src);
extern	  char* strncat(char *dst, const char *src, size_t len);
extern	  int strcmp(const char *pt1, const char *ptr2);
extern	  int strncmp(const char *ptr1, const char *prt2, size_t len);
extern	  char* strcpy(char *dst, const char *src);
extern	  char* strncpy(char *dst, const char *src, size_t len);
extern	  size_t strlen(const char *src);
extern	  char* strchr(const char *src, int chr);
extern	  char* strrchr(const char *src, int chr);

/* In string/complex_str.c */
extern	  char* strpbrk(const char *ptr1, const char *ptr2);
extern	  size_t strspn(const char *ptr1, const char *ptr2);
extern	  size_t strcspn(const char *ptr1, const char *ptr2);
extern	  char* strstr(const char *ptr1, const char *ptr2);
extern	  char* strtok(char *ptr1, const char *ptr2);

/* In string/strerror.c */
extern	  char* strerror(int err);

#ifdef __FULL_HOSTED__
extern	  char* strcasestr(const char *, const char *);
extern	int strcoll(const char *ptr1, const char *ptr2);
extern	  size_t strxfrm();
#endif

extern int isprintf(char *buf, const char *fmt, ...);
extern int isnprintf(char *buf, int len, const char *fmt, ...);

#if 0
extern int ivsprintf(char *buf, const char *fmt, va_list args);
extern int ivsnprintf(char *buf, int bufLen, const char *fmt, va_list args);
extern int ivprintf(const char *buf, va_list args);
extern int ivfprintf(FILE *fp, const char *buf, va_list args);
	#define vsprintf ivsprintf
	#define vsnprintf ivsnprintf
	#define vfprintf ivfprintf
#endif

#define sprintf isprintf
#define snprintf isnprintf
#define nsprintf isnprintf
#define fprintf ifprintf

extern void i_fputn(
	void **dest,     /* pointer to pointer to FILE or buffer. */
	const char *src, /* pointer to string to add to dest */
	int *len,        /* pointer to int having number of characters to add to dest */
	int *total_count, /* pointer to total count added so far. Will be incremented by len */
	int repeat       /* bool: if set repeat the same char len times */
	);

typedef void (*doFPrintFunc)(
	void **dest,     /* pointer to pointer to FILE or buffer. */
	const char *src, /* pointer to string to add to dest */
	int *len,        /* pointer to int having number of characters to add to dest */
	int *total_count, /* pointer to total count added so far. Will be incremented by len */
	int repeat       /* bool: if set repeat the same char len times */
	);

#endif	/* _STRING_H_ */
