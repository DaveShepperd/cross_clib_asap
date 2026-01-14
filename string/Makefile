CSS  = complex_str.c
#CSS += mem_func.c # Use the .mac version
CSS += simple_strs.c
CSS += strdup.c
CSS += strerror.c
MSS  = mem_func.mac
MSS += simple_strs_macas.mac

SSS   = $(patsubst %.c,%.s,$(CSS))
OBJS  = $(patsubst %.s,%.ol,$(SSS))
OBJS += $(patsubst %.mac,%.ol,$(MSS))

default: $(OBJS)

include ../Makefile.common
