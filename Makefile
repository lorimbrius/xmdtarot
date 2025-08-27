CFLAGS  ?= -O3
LDLIBS   = -lm -lXm -lXt -lX11
PREFIX   = /usr/local
INCLUDES = -I. -I$(PREFIX)/include
BINDIR   = games
MANDIR   = /share/man/man6

DECKV    ?= 1

all: xmdtarot man

xmdtarot: xmdtarot.o callbacks.o

xmdtarot.o: xmdtarot.c xmdtarot.h deck_v$(DECKV).h
	@echo "Using deck: $(DECKV)"
	cc $(CFLAGS) $(INCLUDES) -DDECKV=$(DECKV) -c xmdtarot.c -o xmdtarot.o

callbacks.o: callbacks.c xmdtarot.h

deck_v2.h: deck_v2_src/discocardsall.ps
	cd deck_v2_src && ./split_cards.sh discocardsall.ps

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
	rm -f xmdtarot xmdtarot.o callbacks.o xmdtarot.6.gz deck_v2.h

mostlyclean:
	rm -f xmdtarot xmdtarot.o callbacks.o xmdtarot.6.gz