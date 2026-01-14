CSS  = idoprnt.c
#CSS += i_fputn.c
CSS += iprintf.c
CSS += isprintf.c
CSS += ultos.c
CSS += stdio.c
MSS  = i_fputn_macas.mac

SSS   = $(patsubst %.c,%.s,$(CSS))
OBJS  = $(patsubst %.s,%.ol,$(SSS))
OBJS += $(patsubst %.mac,%.ol,$(MSS))

default: $(OBJS)

include ../Makefile.common
