CSS  = idoprnt.c
CSS += i_fputn.c
CSS += iprintf.c
CSS += isprintf.c
CSS += ultos.c
MSS  =

SSS  = $(patsubst %.c,%.s,$(CSS))
OBJS = $(patsubst %.s,%.ol,$(SSS))

default: $(OBJS)

include ../Makefile.common
