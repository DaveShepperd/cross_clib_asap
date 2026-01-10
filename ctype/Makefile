CSS  = #ctype.c	# use the .mac version
MSS  = ctype.mac

SSS   = $(patsubst %.c,%.s,$(CSS))
OBJS  = $(patsubst %.s,%.ol,$(SSS))
OBJS += $(patsubst %.mac,%.ol,$(MSS))

default: $(OBJS)

include ../Makefile.common

