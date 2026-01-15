CSS  = idoprnt.c
CSS += idonprnt.c
#CSS += i_fputn.c	#This in now in i_fputn_macas.mac
#CSS += i_sputn.c	#This is now in i_sputn_macas.mac
#CSS += iprintf.c	#This is now located in varargs.mac
#CSS += isprintf.c	#This is now located in varargs.mac
CSS += ultos.c
CSS += stdio.c
MSS  = i_fputn_macas.mac
MSS += i_sputn_macas.mac
MSS += varargs.mac

SSS   = $(patsubst %.c,%.s,$(CSS))
OBJS  = $(patsubst %.s,%.ol,$(SSS))
OBJS += $(patsubst %.mac,%.ol,$(MSS))

default: $(OBJS)

include ../Makefile.common
