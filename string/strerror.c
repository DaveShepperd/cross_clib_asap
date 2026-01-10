/*	strerror - returns a char pointer to the translation of the error
*	'code'. Note that it is _not_ safe to squirrel away the returned
*	pointer because the contents may change on subsequent calls to
*	strerror. In the routine below, this only occurs with unknown
*	error codes, but the standard specifies it as general behavior.
*	    Obviously, this could be extended to multiple languages and
*	customized error messages (terse to verbose), but this is a start.
*/

#include "errno.h"

/* if you do not have limits.h, or it for some reason does not include CHAR_BIT,
*  substitute the appropriate value for the max number of digits in the hex
*  representation of an int.
*/

#include <limits.h>
#ifndef CHAR_BIT
#define MAXDIGS 8
#endif

#ifndef MAXDIGS
#define MAXDIGS ((sizeof(int)*CHAR_BIT)/4 + 1)
#endif

#define NOCODE "Untranslatable error code 0x "

/* WARNING, the following calculation will fail on broken compilers which
*  believe that sizeof("Quoted string") == sizeof(char *). If you have one
*  of these, substitute hand-calculated number.
*/
static char nocode[sizeof(NOCODE) + MAXDIGS];

/* #define const to whitespace if your compiler does not support it */

struct txt {
    const int code;
    const char * const text;
};

static const struct txt err_txt[] = {
    /* only the first two are required by pANS */
    { EDOM , "EDOM - input out of range" },
    { ERANGE , "ERANGE - result out of range" },
#ifdef EPERM
    { EPERM, "EPERM - No permission for operation (Not owner?)"},
#endif
#ifdef ENOENT
    { ENOENT, "ENOENT - Can't find file or directory" },
#endif
#ifdef ESRCH
    { ESRCH, "ESRCH - no such process" },
#endif
#ifdef EINTR
    { EINTR, "EINTR - System call was interrupted"},
#endif
#ifdef EIO
    { EIO, "EIO - I/O error"},
#endif
#ifdef ENXIO
    { ENXIO, "ENXIO - non-existant device or address"},
#endif
#ifdef E2BIG
    { E2BIG, "E2BIG - Too many arguments in list"},
#endif
#ifdef ENOEXEC
    { ENOEXEC, "ENOEXEC - Format error in exec() call"},
#endif
#ifdef EBADF
    { EBADF, "EBADF - File number invalid"},
#endif
#ifdef ECHILD
    { ECHILD, "ECHILD - No children (sub-processes)"},
#endif
#ifdef EAGAIN
    { EAGAIN, "EAGAIN - process table full"},
#endif
#ifdef ENOMEM
    { ENOMEM, "ENOMEM - Unable to allocate memory"},
#endif
#ifdef EACCES
    { EACCES, "EACCES - Access denied (permission?)"},
#endif
#ifdef EFAULT
    { EFAULT, "EFAULT - Bad address or memory error"},
#endif
#ifdef ENOTBLK
    { ENOTBLK, "ENOTBLK - Not a block-structured device"},
#endif
#ifdef EBUSY
    { EBUSY, "EBUSY - mount device busy"},
#endif
#ifdef EEXIST
    { EEXIST, "EEXIST - attempt to create already existing file"},
#endif
#ifdef EXDEV
    { EXDEV, "EXDEV - attempt to link across devices"},
#endif
#ifdef ENODEV
    { ENODEV, "ENODEV - Device does not exist"},
#endif
#ifdef ENOTDIR
    { ENOTDIR, "ENOTDIR - Not a directory"},
#endif
#ifdef EISDIR
    { EISDIR, "EISDIR - Directory, not a file"},
#endif
#ifdef EINVAL
    { EINVAL, "EINVAL - invalid argument"},
#endif
#ifdef ENFILE
    { ENFILE, "ENFILE - too many files in (os) file table"},
#endif
#ifdef EMFILE
    { EMFILE, "EMFILE - too many files open (for process)"},
#endif
#ifdef ENOTTY
    { ENOTTY, "ENOTTY - Not a 'typewriter' (terminal)"},
#endif
#ifdef ETXTBSY
    { ETXTBSY, "ETXTBSY - 'text' (executable) file busy"},
#endif
#ifdef EFBIG
    { EFBIG, "EFBIG - File too large"},
#endif
#ifdef ENOSPC
    { ENOSPC, "ENOSPC - No space remaining on device (volume)"},
#endif
#ifdef ESPIPE
    { ESPIPE, "ESPIPE - illegal seek (on pipe?)"},
#endif
#ifdef EROFS
    { EROFS, "EROFS - (attempt to write on?) read-only file system"},
#endif
#ifdef EMLINK
    { EMLINK, "EMLINK - (attempt to create?) too many links"},
#endif
#ifdef EPIPE
    { EPIPE, "EPIPE - pipe broken (other process died?)"},
#endif
#ifdef EWOULDBLOCK
    { EWOULDBLOCK, "EWOULDBLOCK - I/O operation would block"},
#endif
#ifdef EINPROGRESS
    { EINPROGRESS, "EINPROGRESS - Oper. now in progress"},
#endif
#ifdef EALREADY
    { EALREADY, "EALREADY - oper. already in progress"},
#endif
#ifdef ENOTSOCK
    { ENOTSOCK, "ENOTSOCK - Not a socket"},
#endif
#ifdef EDESTADDRREQ
    { EDESTADDRREQ, "EDESTADDRREQ - Requires destination address"},
#endif
#ifdef EMSGSIZE
    { EMSGSIZE, "EMSGSIZE - Message is too long"},
#endif
#ifdef EPROTOTYPE
    { EPROTOTYPE, "EPROTOTYPE - Wrong protocol type for socket"},
#endif
#ifdef ENOPROTOOPT
    { ENOPROTOOPT, "ENOPROTOOPT - Protocol not available"},
#endif
#ifdef EPROTONOSUPPORT
    { EPROTONOSUPPORT, "EPROTONOSUPPORT - Protocol not supported"},
#endif
#ifdef ESOCKTNOSUPPORT
    { ESOCKTNOSUPPORT, "ESOCKTNOSUPPORT - Socket type not supported"},
#endif
#ifdef EOPNOTSUPP
    { EOPNOTSUPP, "EOPNOTSUPP - Operation not supported on socket"},
#endif
#ifdef EPFNOSUPPORT
    { EPFNOSUPPORT, "EPFNOSUPPORT - Protocol Family not supported"},
#endif
#ifdef EAFNOSUPPORT
    { EAFNOSUPPORT, "EAFNOSUPPORT - Address Family not supported"},
#endif
#ifdef EADDRINUSE
    { EADDRINUSE, "EADDRINUSE - Address already in use"},
#endif
#ifdef EADDRNOTAVAIL
    { EADDRNOTAVAIL, "EADDRNOTAVAIL - Address not available for assignment"},
#endif
#ifdef ENETDOWN
    { ENETDOWN, "ENETDOWN - Network down"},
#endif
#ifdef ENETUNREACH
    { ENETUNREACH, "ENETUNREACH - Network unreachable"},
#endif
#ifdef ENETRESET
    { ENETRESET, "ENETRESET - Network connection dropped by reset"},
#endif
#ifdef ECONNABORTED
    { ECONNABORTED, "ECONNABORTED - Connection aborted by software"},
#endif
#ifdef ECONNRESET
    { ECONNRESET, "ECONNRESET - Peer reset connection"},
#endif
#ifdef ENOBUFS
    { ENOBUFS, "ENOBUFS - No buffers (memory) available"},
#endif
#ifdef EISCONN
    { EISCONN, "EISCONN - Socket already connected"},
#endif
#ifdef ENOTCONN
    { ENOTCONN, "ENOTCONN - Socket not connected"},
#endif
#ifdef ESHUTDOWN
    { ESHUTDOWN, "ESHUTDOWN - Attempt to send after socket shutdown"},
#endif
#ifdef ETOOMANYREFS
    { ETOOMANYREFS, "ETOOMANYREFS - Too many references: Can't splice"},
#endif
#ifdef ETIMEDOUT
    { ETIMEDOUT, "ETIMEDOUT - Connection timed out"},
#endif
#ifdef ECONNREFUSED
    { ECONNREFUSED, "ECONNREFUSED - Connection refused"},
#endif
#ifdef ELOOP
    { ELOOP, "ELOOP - Too many levels of symbolic links (self-reference?)"},
#endif
#ifdef ENAMETOOLONG
    { ENAMETOOLONG, "ENAMETOOLONG - File name too long"},
#endif
#ifdef EHOSTDOWN
    { EHOSTDOWN, "EHOSTDOWN - Host is down"},
#endif
#ifdef EHOSTUNREACH
    { EHOSTUNREACH, "EHOSTUNREACH - Host is un-reachable (no route)"},
#endif
#ifdef ENOTEMPTY
    { ENOTEMPTY, "ENOTEMPTY - (Attempt to delete?) non-empty directory"},
#endif
#ifdef EPROCLIM
    { EPROCLIM, "EPROCLIM - Too many processes"},
#endif
#ifdef EUSERS
    { EUSERS, "EUSERS - Too many users"},
#endif
#ifdef EDQUOT
    { EDQUOT, "EDQUOT - Disk Quota exceeded"},
#endif
};

char *
strerror(code)
int code;
{

    register const struct txt *tptr;
    char *wp;
    int cnt;

    tptr = &err_txt[0];

    for ( cnt = sizeof(err_txt)/sizeof(struct txt) ; --cnt >= 0 ; ++tptr ) {
	if ( tptr->code == code ) return (char *)tptr->text;
    }

    /* fell out without recognizing error code, build a message */

    strcpy(nocode,NOCODE);
    wp = &nocode[strlen(nocode)];
    for ( cnt = MAXDIGS ; --cnt >= 0 ; ) {
	wp[cnt] = "0123456789ABCDEF"[code&0xF];
	code >>= 4;
    }
    wp[MAXDIGS] = '\0';
    return nocode;
}
