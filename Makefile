main: inlupp1

C_COMPILER     = gcc
C_OPTIONS      = -Wall -pedantic -g
C_LINK_OPTIONS = -lm 
CUNIT_LINK     = -lcunit

%.o:  %.c
	$(C_COMPILER) $(C_OPTIONS) $^ -c

inlupp1: src/hash_table.o
	$(C_COMPILER) $(C_LINK_OPTIONS) $^ -o $@

#src/hash_table.o: src/hash_table.c
#	$(C_COMPILER) $(C_OPTIONS) $^ -c

inlupp1.final: file1.o file2.o file3.o
# TODO: add e.g. optimisation flags, remove unnecessary linking, etc.

clean:
	rm -f *.o inlupp1