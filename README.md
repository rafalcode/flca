# flca

File Listing Capture. The big assumption here is of one word per line.

Will start off innocently, just captured a filename on the argument list and captures them in an array of strings.

## base1w.c
Just one word, given on the argline

## basemwf.c
Multiples words

## basemwf.c
Parting from basemw.c helped thigns quite alot
and getting it to run though files wasn't a huge problem
However, the situation of files ending with a newline and and EOF
(essentially the last line is an EOF) did create a leak. Solving that took longer than expected.
