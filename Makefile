# A somewhat simple Unix Makefile
CFLAGS=-O
PROG=picky
INSTALL= install
DESTDIR= /usr/local/bin
MAN1DIR= /usr/local/man/man1
all: $(PROG)
$(PROG): $(PROG).c
	$(CC) $(CFLAGS) -o $(PROG) $(PROG).c
clean:
	rm -f $(PROG).o
clobber: clean
	rm -f $(PROG)
test: $(PROG) $(PROG).1 $(PROG).c
	./$(PROG) -s Makefile -t $(PROG).1 $(PROG).c
install: $(PROG) $(PROG).1
	${INSTALL} -m 0775 -d ${DESTDIR}
	${INSTALL} $(PROG) ${DESTDIR}
	${INSTALL} -m 0775 -d ${MAN1DIR}
	${INSTALL} $(PROG).1 ${MAN1DIR}
