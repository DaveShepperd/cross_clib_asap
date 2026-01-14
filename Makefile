DIRS  = base
DIRS += ctype
DIRS += setjmp
DIRS += stdio
DIRS += stdlib
DIRS +=string
LIBR = libr
ECHO = /usr/bin/echo -e

.SILENT:

default:
	$(ECHO) "\tVisiting all subdirs ..."
	for xx in $(DIRS); do \
		(cd $$xx;make);\
		done;
	$(ECHO) "\tCreate crtl.lib ..."
	$(LIBR) `find . -name "*.ol"` > crtl.lib

clean:
	rm -f crtl.lib
	for xx in $(DIRS); do \
		(cd $$xx;make clean);\
		done;

