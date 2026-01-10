CSS  =
MSS  = crt0.mac
MSS += gnu_div.mac
MSS += gnu_mod.mac
MSS += gnu_mul.mac
MSS += isqrt.mac
MSS += mul_evolve.mac
MSS += ultodec.mac

SSS   = $(patsubst %.c,%.s,$(CSS))
OBJS  = $(patsubst %.s,%.ol,$(SSS))
OBJS += $(patsubst %.mac,%.ol,$(MSS))

default: $(OBJS)

include ../Makefile.common

