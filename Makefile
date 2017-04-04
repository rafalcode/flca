CC=gcc
DBGCFLAGS=-g -Wall# -pg # note the gprof option
CFLAGS=-O2
EXES=base1w basemw basemwf pwfl

# very basic, just accepts one word. on command line.
base1w: base1w.c
	${CC} ${DBGCFLAGS} -o $@ $^

# multi word
basemw: basemw.c
	${CC} ${DBGCFLAGS} -o $@ $^

# multi word file
basemwf: basemwf.c
	${CC} ${DBGCFLAGS} -o $@ $^

# multi word file
pwfl: pwfl.c
	${CC} ${DBGCFLAGS} -o $@ $^

.PHONY: clean

clean:
	rm -f ${EXES} vgcore.* core*
