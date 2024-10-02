# Put @ in front of command to not print it out when running.

# Only type "make" to run the program.
main: run

# Aliases.
C_COMPILER     = gcc
C_OPTIONS      = -Wall -pedantic -g -fprofile-arcs -ftest-coverage
C_LINK_OPTIONS = -lm 
CUNIT_LINK     = -lcunit
GCOV           = -lgcov --coverage

SRC_OBJECTS = common.o hash_table.o linked_list.o

# Compile and run immediately.
run: normalbuild
	@./normalbuild

test: testbuild
	@valgrind --leak-check=full ./testbuild 
	@gcov unit_tests.o $(SRC_OBJECTS)

simpletest: testbuild
	@./testbuild

# Make will avoid trying to search for files with the same name,
# instead just runs the command.
.PHONY: test simpletest run


# Build.
# TODO include the main file on normal build when it is created
normalbuild: $(SRC_OBJECTS)
	$(C_COMPILER) $(C_LINK_OPTIONS) $^ -g -o $@
testbuild: unit_tests.o $(SRC_OBJECTS)
	$(C_COMPILER) $(C_LINK_OPTIONS) $^ -g -o $@ $(CUNIT_LINK) $(GCOV)

# Source files.
common.o: src/common.c
	$(C_COMPILER) $(C_OPTIONS) $^ -c
hash_table.o: src/hash_table.c
	$(C_COMPILER) $(C_OPTIONS) $^ -c
linked_list.o: src/linked_list.c
	$(C_COMPILER) $(C_OPTIONS) $^ -c

# Test files.
unit_tests.o: test/unit_tests.c
	$(C_COMPILER) $(C_OPTIONS) $^ -c

# Remove temp build files.
clean:
	@rm -f *.o testbuild normalbuild a.out *.gcda *.gcno *.gcov
	@echo "Cleaned files"
