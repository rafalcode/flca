CC=gcc
DBGCFLAGS=-g -Wall# -pg # note the gprof option
CFLAGS=-O2
EXES=base1w

# lnarr, simple line arrays from a single file
base1w: base1w.c
	${CC} ${DBGCFLAGS} -o $@ $^


.PHONY: clean

clean:
	rm -f ${EXES} vgcore.* core*
