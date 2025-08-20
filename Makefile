CFLAGS ?= -O3
LDLIBS  = -lm -lXm -lXt -lX11
PREFIX  = /usr/local
BINDIR  = games
MANDIR  = /share/man/man6

all: xmdtarot

xmdtarot: xmdtarot.o callbacks.o

install: all
	mkdir -p $(DESTDIR)$(PREFIX)$(BINDIR)
	mkdir -p $(DESTDIR)$(PREFIX)$(MANDIR)
	install -m 0755 -s xmdtarot $(DESTDIR)$(PREFIX)$(BINDIR)/xmdtarot
	
deinstall:
	rm -f $(DESTDIR)$(PREFIX)$(BINDIR)/xmdtarot

clean:
	rm -f xmdtarot xmdtarot.o callbacks.o