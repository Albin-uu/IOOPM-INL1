# IOOPM-INL1

Phase 1, First assignment
Albin Lord, Karl Wästvind
Group: Clem


Uses a custom commit convention by Albin:

https://github.com/AshenCinders/CustomCommitConvention


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
