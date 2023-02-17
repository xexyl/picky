#!/usr/bin/env make
#
# picky - plain ASCII text file validation tool
#
# This Makefile is:
# Copyright (c) 2022,2023 by Landon Curt Noll.  All Rights Reserved.
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


SHELL= bash

CC= cc
CFLAGS= -O3 -g3
RM= rm
CP= cp
CHMOD= chmod
INSTALL= install
DESTDIR= /usr/local/bin
MAN1DIR= /usr/local/man/man1
RM= rm
CP= cp
CHMOD= chmod
MAN= man

DESTDIR= /usr/local/bin

TARGETS= picky

all: ${TARGETS} picky.txt

# rules, not file targets
#
.PHONY: all configure clean clobber install

picky: picky.c
	${CC} ${CFLAGS} picky.c -o $@

picky.txt: picky.1
	${RM} -f $@
	MANWIDTH=67 ${MAN} picky.1 > $@

clean:
	${RM} -f picky.o

clobber: clean
	${RM} -f ${TARGETS}

install: all
	${INSTALL} -m 0775 -d ${DESTDIR}
	${INSTALL} ${TARGETS} ${DESTDIR}
	${INSTALL} -m 0775 -d ${MAN1DIR}
	${INSTALL} picky.1 ${MAN1DIR}
