# IOOPM-INL1

Phase 1, First assignment
Albin Lord, Karl Wästvind
Group: Clem


Uses a custom commit convention by Albin:

https://github.com/AshenCinders/CustomCommitConvention


# General Info

Build the program with `make normalbuild` and subsequently run it with `./normalbuild FILE_PATH_HERE`.

Build and run test with a single command `make test`.
This also shows memory usage and leaks with valgrind, and code coverage with gcov.

Clean all build- and coverage files with `make clean`

More options can be found by looking at the `Makefile`.

## Code coverage of testbuild

Using gcov

`common.c`: 100% coverage

`hash_table.c`: 100% coverage

`linked_list.c`: 100% coverage

Checking that all tests are ran:
`unit_tests.c`: 98.77% coverage
(error handling CUnit boilerplate)

Note that the code in `freq_count.c` cannot be tested with CUnit as it contains a main file.
Only `add_or_increment` is tested here (using a basic assert).


# Documentation

Some shared data-formats and macros can be found in common.h

## Hash table documentation

A ioopm_hash_table_t can be created with ioopm_hash_table_create, and must be ioopm_hash_table_destroy'd after use.

Each entry is an elem_t.
If the value type is elem.ptr_v then the user owns whatever data it points to. The same goes for keys.
Other elem_t subtypes gets deleted on destruction.

A hash table has a static bucket count of 257, so the performance will slowly degrade if large number of elements are inserted.

Keys are constant and can only be read.

Functions that can fail will return true on success and false on failure. If more data needs to be returned it is done via an argument-pointer.

Information on function-specific usage can be found in hash_table.h.

## Linked list documentation

A ioopm_list_t can be created with ioopm_linked_list_create, and must be ioopm_linked_list_destroy'd after use.

Each entry is an elem_t.
If the value type is elem.ptr_v then the user owns whatever data it points to.

Functions that can fail will return true on success and false on failure. If more data needs to be returned it is done via an argument-pointer.

ioopm_linked_list_prepend, ioopm_linked_list_append, and ioopm_linked_list_size run in constant order-of-growth.

Information on function-specific usage can be found in linked_list.h.


# Initial Profiling Results

Using gprof

**Running 1k-long-words.txt**:

Negligible time spent on all parts of the program.
Total time: 0.01s

**Running 10k-words.txt**:

Negligible time spent on all parts of the program.
Total time: 0.01s

**Running 16k-words.txt**:

Total time: 0.12s

~ca 80% of time spent in get_link_before_index

    This is largely because linked_list_get is used during conversion
    of a list to array for sorting (because the iterators do not yet exist as of writing).
    Since ioopm_linked_list_get has to walk thorough the list until it finds the correct index,
    and is called on each element in input list the resulting order-of-growth
    is a horrible O(n²) for converting the list to an array.

~ca 10% of time spent in find_previous_entry_for_key

    Since 16k-words.txt has a large number of unique words the
    list in each bucket in the hash table array will get very large.
    When searching for an entry we have to walk through the whole list
    until the value is found, thus losing a lot of performance.

~ca 10% of time spent in ioopm_equals_str

    This could be improved if a faster string comparson function is found.
    We can expect that reduing the number of checks from waling through lists
    also improves the total time spent here.


Regarding the lists in the hash table array, we can improve overall performace
by reducing the number of elements in each list by increasing the number of buckets.

This means we use the hash function more effectively and lessen the time spent bruteforcing seraching linearly.
(And adhering more to the point of using hash tables. That is, fast lookups.)


**Performance after changing number of buckets from 17 to 257**:

(using a prime number lowers chance of potential hashing patterns, making the spread of elements in the buckets more even).

Running 1k-long-words.txt:

Negligent time, same as before.

Running 10k-words.txt:

Negligent time, same as before.

Running 16k-words.txt

Total time: 0.08s
Yielding roughly a 50% performance increase.

~ca 95% of time spent in get_link_before_index

    It is reasonable to assume the vast majority of time is spent on the temporary list-to-array conversion code.

**Memory consumption before attempted performace improvements**:

Using valgrind

1k-long-words.txt
~ca 75 000 bytes allocated

10k-words.txt
~ca 93 000 bytes allocated


16k-words.txt
~ca 470 000 bytes allocated


**Memory consumption after attempted performance improvements**:

Using valgrind

1k-long-words.txt
~ca 86 000 bytes allocated

10k-words.txt
~ca 104 000 bytes allocated

16k-words.txt
~ca 485 000 bytes allocated


The proportional memory usage added when increasing the number of buckets is lower the more elements there are in the hash table.

Therefore hash tables usually resize themselves to keep a good balance between speed and memory usage.
