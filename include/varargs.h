#ifndef _VARARGS_H_
#define _VARARGS_H_

typedef char *va_list;

#define __va_rounded_size(TYPE)  \
  (((sizeof (TYPE) + sizeof (int) - 1) / sizeof (int)) * sizeof (int))

#define va_end(AP)

/* Amount of space required in an argument list for an arg of type TYPE.
   TYPE may alternatively be an expression whose type is used.  */

#define va_arg(AP, TYPE)						\
 (AP += __va_rounded_size (TYPE),					\
  *((TYPE *) (AP - __va_rounded_size (TYPE))))

#if 0

/*****************************************************
   gca doesn't put the first 5 arguments on the stack
   so the va_start() thing doesn't work. Have to do it
   using shims written in assembler. See varargs.mac.
******************************************************/

#define va_start(AP, LASTARG) 						\
 (AP = ((char *) &(LASTARG) + __va_rounded_size (LASTARG)))
#endif

#endif /* _VARARGS_H_ */
