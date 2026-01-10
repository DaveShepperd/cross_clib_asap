/*	"Complex" string functions. These are the functions from string.h
*	that are typically slower than O(len) + a small constant (strpbrk,
*	strspn, strcspn, strstr, strtok). The short, relatively quick
*	functions from string.h (strcat, strncat, strchr, strrchr, strcmp,
*	strncmp, strcpy, strncpy, strlen) are in simple_strings.c. strdup
*	is in a separate file, as it is not part of the standard.
*	Locale-specific routines (strcoll, strxfrm, strerror) may never be
*	written	by me.
*	   This code is copyright by Michael Albaugh 1989. Anyone may do
*	anything to it with the sole exceptions of removing this notice or
*	attaching further restrictions.
*/

#include <string.h>		/* among other things, gets size_t */

/*	strpbrk() returns a pointer to the first character in 'srch' that
*	is a member of 'brk'. If no member of 'brk' found, returns NULL
*/

char *strpbrk(const char *srch, const char *brk)
{
    const char *tbp;
    int s,b;

    for ( ; (s = *srch) != '\0' ; ++srch ) {
	for ( tbp = brk ; (b = *tbp) != '\0' ; ++tbp ) {
	    if ( b == s)
		   return (char *)srch;
	}
    }
    return NULL;
}
	
/*	strspn() returns the index of the first character in 'srch' that
*	is not a member of 'spn'. If all chars are members of 'spn', returns
*	index of terminating NUL (strlen(srch))
*/

size_t strspn(const char *srch, const char *spn)
{
    const char *tbp,*tsp;
    int s,b;

    for ( tsp = srch ; (s = *tsp) != '\0' ; ++tsp ) {
	for ( tbp = spn ; (b = *tbp) != s ; ++tbp ) {
	    if ( b == '\0') goto break_both;	/* not found in span, done */
	}
    }
break_both:
    return tsp-srch;
}

/*	strcspn() returns the index of the first character in 'srch' that
*	is a member of 'spn'. If no member of 'spn' found, returns index
*	of terminating NUL (strlen(srch))
*/
size_t strcspn(const char *srch, const char *spn)
{
    const char *tsp,*tbp;
    int s,b;

    for ( tsp = srch ; (s = *srch) != '\0' ; ++srch ) {
	for ( tbp = spn ; (b = *tbp) != '\0' ; ++tbp ) {
	    if ( b == s) goto break_both;
	}
    }
break_both:
    return tsp-srch;
}

/*	strstr() returns a pointer to the first occurence of 'substr' in
*	'srch'. If no such occurence, returns NULL. This is a fairly naive
*	implementation, based on the assumed likelyhood that both srch and
*	substr are too short to benefit from more complex algorithms (e.g.
*	Boyer-Moore).
*/
char *strstr(const char *srch, const char *substr)
{
    const char *sst;
    int c;
    int chr;

/*	We are taking a wild guess that trying to match a zero-length
*	substring results in a succesful match versus the beginning of 'srch'
*/
    if ( (chr = *substr) == '\0' )
	   return (char *)srch;

    /* repeat the following two phases until success or failure */
    while (1) {
	/* first find leftmost instance of first char of substring. */
	for ( ;  (c = *srch) != chr ; ++srch ) {
	    if ( c == '\0' )
		   return NULL;	/* past end of srch, failed */
	}

	/* now check for remainder of substring */
	for ( sst = substr ; (c = *(++sst)) != '\0' && ( c == *(++srch)) ;)
	{
		;
	}

	/* WARNING! Subtle code follows:
	*  Back up by length we checked. If we left the for loop because we
	*  reached the end of substr, srch has not been incremented, so we
	*  need to back up one less than the length of substr to find the
	*  starting point of the substring. On the other hand, if we had a
	*  mis-match, we want to advance one character in srch anyway.
	*  In either case the desired adjustment is the same.
	*/
	srch -= ((sst - substr)-1);
	if ( c == '\0' )
	       return (char *)srch;
    }
    return NULL;
}

/*	strtok() is a wierd state-ful (hence non-reentrant) routine with
*	no known redeeming social importance. It is included here because
*	it is part of the pANS, but you won't catch _me_ using it :-).
*	Ok, what it does is poke '\0's into a string in place of "delimiters".
*	If called with a non-null 'target', it starts scanning that string
*	skipping over characters in the set 'delim'. Once it finds a non-nul
*	character not in delim, it is commited to returning a pointer to that
*	character (call it 'start'). It then scans until it finds a non-nul
*	character that _is_ in 'delim', replaces it with a NUL, saves a pointer
*	one past this NUL in its secret place, and returns 'start'.
*	If called with a null ((char *)0) 'target', it picks up where it left
*	off. You can change the value of 'delim' between calls, but you mess
*	with *'target' (the string, not the pointer) at your peril. If it ever
*	runs off the end of the string, it saves a pointer to that end (the
*	NUL) so subsequent calls (with 'target' == 0) fail.
*		If it can't find 'start', it returns NULL.
*/
char *strtok(char *target, const char *delim)
{
    static char *magic;
  
    if ( target != 0 ) magic = target;
    else target = magic;

    if ( *target == '\0' ) return 0;
    target = &target[strspn(target,delim)];
    if ( *target == '\0' ) {
	/* failure to find 'start', remember and return */
	magic = target;
	return NULL;
    }

    /* found beginning of token, now look for end */
    if ( *(magic = target + strcspn(target,delim)) != '\0') *magic++ = '\0';
    return target;
}
