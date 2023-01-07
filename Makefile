# A somewhat simple Unix Makefile
CC= cc
CFLAGS= -O
DESTDIR= /usr/local/bin
INSTALL= install
MAN1DIR= /usr/local/man/man1
PICKY= picky
RM= rm

all: ${PICKY}
	@:

.PHONY: all

${PICKY}: ${PICKY}.c
	${CC} ${CFLAGS} -o ${PICKY} ${PICKY}.c

clean:
	${RM} -f ${PICKY}.o

clobber: clean
	${RM} -f ${PICKY}

test: ${PICKY} ${PICKY}.1 ${PICKY}.c
	./${PICKY} -s Makefile -t ${PICKY}.1 ${PICKY}.c

install: ${PICKY} ${PICKY}.1
	${INSTALL} -m 0775 -d ${DESTDIR}
	${INSTALL} ${PICKY} ${DESTDIR}
	${INSTALL} -m 0775 -d ${MAN1DIR}
	${INSTALL} ${PICKY}.1 ${MAN1DIR}
