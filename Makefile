# Put @ in front of command to not print it out when running.

# Only type "make" to run the program.
main: run

# Aliases.
C_COMPILER     = gcc
C_OPTIONS      = -Wall -pedantic -g
C_LINK_OPTIONS = -lm 
CUNIT_LINK     = -lcunit

#%.o:  %.c
#	$(C_COMPILER) $(C_OPTIONS) $^ -c

# Compile and run immediately.
run: normalbuild
	@./normalbuild

test: testbuild
	@valgrind --leak-check=full ./testbuild 

testnovalgrind: testbuild
	@./testbuild

# Don't create files with target names, just run the command.
.PHONY: test testnovalgrind run

# Build.
normalbuild: hash_table.o
	$(C_COMPILER) $(C_LINK_OPTIONS) $^ -g -o $@
testbuild: hash_table.o unit_tests.o
	$(C_COMPILER) $(C_LINK_OPTIONS) $? -g -o $@ $(CUNIT_LINK) 

# Source files.
hash_table.o: src/hash_table.c
	$(C_COMPILER) $(C_OPTIONS) $^ -c

# Test files.
unit_tests.o: test/unit_tests.c
	$(C_COMPILER) $(C_OPTIONS) $^ -c

#inlupp1.final: file1.o file2.o file3.o
# TODO: add e.g. optimisation flags, remove unnecessary linking, etc.

clean:
	@rm -f *.o testbuild normalbuild a.out
	@echo "Cleaned files"
