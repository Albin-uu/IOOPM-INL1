# Put @ in front of command to not print it out when running.

# Only type "make" to run the program.
main: run

# Aliases.
C_COMPILER     = gcc
C_OPTIONS      = -Wall -pedantic -g
C_LINK_OPTIONS = -lm 
CUNIT_LINK     = -lcunit

# Compile and run immediately.
run: normalbuild
	@./normalbuild

test: testbuild
	@valgrind --leak-check=full ./testbuild 

testnovalgrind: testbuild
	@./testbuild

# Make will avoid trying to search for files with the same name,
# instead just runs the command.
.PHONY: test testnovalgrind run

# Build.
# TODO include the main file on normal build when it is created
normalbuild: hash_table.o linked_list.o
	$(C_COMPILER) $(C_LINK_OPTIONS) $^ -g -o $@
testbuild: unit_tests.o hash_table.o linked_list.o
	$(C_COMPILER) $(C_LINK_OPTIONS) $^ -g -o $@ $(CUNIT_LINK) 

# Source files.
hash_table.o: src/hash_table.c
	$(C_COMPILER) $(C_OPTIONS) $^ -c
linked_list.o: src/linked_list.c
	$(C_COMPILER) $(C_OPTIONS) $^ -c

# Test files.
unit_tests.o: test/unit_tests.c
	$(C_COMPILER) $(C_OPTIONS) $^ -c

# Remove temp build files.
clean:
	@rm -f *.o testbuild normalbuild a.out
	@echo "Cleaned files"
