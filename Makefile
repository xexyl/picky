#!/usr/bin/env make
#
# picky - plain ASCII text file validation tool
#
# This Makefile is:
# Copyright (c) 2022-2024 by Landon Curt Noll.  All Rights Reserved.
#
# NOTE: The source code was once available under:
#
#   http://grail.eecs.csuohio.edu/~somos/picky.html
#
# It was written by Michael Somos.  We are not the author of this code.
#
# Permission to use, copy, modify, and distribute this software and
# its documentation for any purpose and without fee is hereby granted,
# provided that the above copyright, this permission notice and text
# this comment, and the disclaimer below appear in all of the following:
#
#       supporting documentation
#       source copies
#       source works derived from this source
#       binaries derived from this source or from derived source
#
# LANDON CURT NOLL DISCLAIMS ALL WARRANTIES WITH REGARD TO THIS SOFTWARE,
# INCLUDING ALL IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS. IN NO
# EVENT SHALL LANDON CURT NOLL BE LIABLE FOR ANY SPECIAL, INDIRECT OR
# CONSEQUENTIAL DAMAGES OR ANY DAMAGES WHATSOEVER RESULTING FROM LOSS OF
# USE, DATA OR PROFITS, WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR
# OTHER TORTIOUS ACTION, ARISING OUT OF OR IN CONNECTION WITH THE USE OR
# PERFORMANCE OF THIS SOFTWARE.
#
# chongo (Landon Curt Noll, http://www.isthe.com/chongo/index.html) /\oo/\
#
# Share and enjoy! :-)


# tools
#
CC= cc
CHMOD= chmod
CP= cp
INSTALL= install
MAN= man
RM= rm
SHELL= bash


# how to compile
#
CFLAGS= -O3 -g3


# where and what to install
#
PROG= picky
#
DESTDIR= /usr/local/bin
MAN1DIR= /usr/local/man/man1
#
TARGETS= ${PROG}


# default and first rule
#
all: ${TARGETS}


# rules, not file targets
#
.PHONY: all configure clean clobber install


# how to build
#
${PROG}.o: ${PROG}.c
	${RM} -f $@
	${CC} ${CFLAGS} ${PROG}.c -c

${PROG}: ${PROG}.o
	${RM} -f $@
	${CC} ${CFLAGS} ${PROG}.o -o $@


test.txt: ${PROG}.1 ${PROG}
	${RM} -f $@
	MANWIDTH=67 ${MAN} ./${PROG}.1 > $@

# utility rules
#
test: ${PROG} ${PROG}.1 ${PROG}.c test.txt
	./${PROG} -s Makefile -t ${PROG}.1 ${PROG}.c
	./${PROG} -t -b test.txt

clean:
	${RM} -f ${PROG}.o

clobber: clean
	${RM} -f ${TARGETS}

install: all
	${INSTALL} -m 0775 -d ${DESTDIR}
	${INSTALL} -m 0555 ${PROG} ${DESTDIR}
	${INSTALL} -m 0775 -d ${MAN1DIR}
	${INSTALL} -m 0444 ${PROG}.1 ${MAN1DIR}

uninstall:
	-${RM} -f -v ${DESTDIR}/${PROG}
	-${RM} -f -v ${MAN1DIR}/${PROG}.1
