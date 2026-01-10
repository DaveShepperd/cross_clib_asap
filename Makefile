DIRS  = base
DIRS += ctype
DIRS += setjmp
DIRS += stdio
DIRS += stdlib
DIRS +=string
LIBR = libr

.SILENT:

default:
	for xx in $(DIRS); do \
		(cd $$xx;make);\
		done;
	$(LIBR) `find . -name "*.ol"` > crtl.lib

clean:
	rm -f crtl.lib
	for xx in $(DIRS); do \
		(cd $$xx;make clean);\
		done;

