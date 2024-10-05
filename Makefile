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
	@valgrind --leak-check=full ./normalbuild test_text.txt
#	@gcov freq_count.o $(SRC_OBJECTS)

test: testbuild
	@valgrind --leak-check=full ./testbuild 
	@gcov unit_tests.o $(SRC_OBJECTS)

simpletest: testbuild
	@./testbuild

# Make will avoid trying to search for files with the same name,
# instead just runs the command.
.PHONY: test simpletest run


# Build.
normalbuild: freq_count.o $(SRC_OBJECTS)
	$(C_COMPILER) $(C_LINK_OPTIONS) $^ -g -o $@ $(GCOV)
testbuild: unit_tests.o $(SRC_OBJECTS)
	$(C_COMPILER) $(C_LINK_OPTIONS) $^ -g -o $@ $(CUNIT_LINK) $(GCOV)

# Source files.
freq_count.o: src/freq_count.c
	$(C_COMPILER) $(C_OPTIONS) $^ -c

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
