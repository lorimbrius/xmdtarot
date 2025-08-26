CFLAGS  ?= -O3
LDLIBS   = -lm -lXm -lXt -lX11
PREFIX   = /usr/local
INCLUDES = -I. -I$(PREFIX)/include
BINDIR   = games
MANDIR   = /share/man/man6

DECK_H   = deck_v1.h

all: xmdtarot man

xmdtarot: xmdtarot.o callbacks.o

xmdtarot.o: xmdtarot.c xmdtarot.h $(DECK_H)

callbacks.o: callbacks.c xmdtarot.h

man: xmdtarot.6
	gzip < xmdtarot.6 > xmdtarot.6.gz

install: all
	mkdir -p $(DESTDIR)$(PREFIX)$(BINDIR)
	mkdir -p $(DESTDIR)$(PREFIX)$(MANDIR)
	install -m 0755 -s xmdtarot $(DESTDIR)$(PREFIX)$(BINDIR)/xmdtarot
	install -m 0644 xmdtarot.6.gz $(DESTDIR)$(PREFIX)$(MANDIR)/xmdtarot.6.gz
	
deinstall:
	rm -f $(DESTDIR)$(PREFIX)$(BINDIR)/xmdtarot
	rm -f $(DESTDIR)$(PREFIX)$(MANDIR)/xmdtarot.6.gz

clean:
	rm -f xmdtarot xmdtarot.o callbacks.o xmdtarot.6.gz