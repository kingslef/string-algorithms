string-algorithms
=================

This is a project for T-106.5400 String Algorithms course in spring 2014. The
main idea was to implement some of the well-known exact string pattern matching
algorithms and study if we could pick the fastest algorithm by sampling the text
and the pattern, i.e. choose run only part of the pattern through part of the
text.

Directory Structure
-------------------

String searching algorithms are implemented inside **src/algorithms/** and they
are compiled to **libstrpat.a**. Application using that is implemented in
**src/fep.c**.

*   **build/** This is where the project is most likely built.
*   **src/** Location of the main source code.
   *   **fep.c** Fixed expression pattern searcher, main function for the project.
   *   **algorithms/** Implementation of the string algorithms.
      *   **trivial.c** Trivial string searching algorithm.
      *   **kmp.c** Knuth-Morris-Pratt string searching algorithm.
      *   **bm.c** Boyer-Moore string searching algorithm.
      *   **rk.c** Rabin-Karp string searching algorithm.
      *   **trivial_mem.c** Trivial algorithm using glibc functions.
*   **tests/**  Directory for unit tests.
   * **test_main.c** Unit tests.

Building
--------

Building the project requires CMake 2.6 and a GNU/Linux machine.

To build, execute:
``` sh
~/string-algorithms> mkdir build && cd build
~/string-algorithms/build> cmake .. && make
```

Now you should be able to run the tests with:
``` sh
~/string-algorithms/build> ./test_main # or alternatively valgrind ./test_main
```

... and to execute the program itself with:
``` sh
~/string-algorithms/build> ./fep <pattern> <file>
```

Example:
``` sh
~/string-algorithms/build> ./fep pat Makefile
```

About the Chosen Algorithms
---------------------------

Knuth-Morris-Pratt and Boyer-Moore were quite easy to pick because they are
arguably the most well-known. For example, GNU Grep seems to use Boyer-Moore
variant for fixed string matching <cite>[1]</cite> and Knuth-Morris-Pratt is
usually the first algorithm encountered in any string algorithm course.

Rabin-Karp doesn't offer anything since it mostly shines when searching for
multiple patterns, but since it was easy to implement I chose it also.

I wanted to implement shift-and but I didn't quite have time for it.

To be able to compare running times of BM and KMP, I implemented basic trivial
algorithm which loops the text through and looks for the pattern.

Finally, after digging through the internet on string searching algorithms, I
found a suggestion that a simple fixed string searcher could be implemented with
memmem(3) <cite>[2]</cite>. Since I already read the text with mmap(2) and I
didn't care about portability (memmem is an GNU extension), I wanted to see how
it would manage against the other algorithms. I was quite suprised to see that
when digging through 1467600 byte text searching for nonexisting 393 character
pattern, memmem only took 0.21 ms when for example KMP took 16 ms.

[1]: http://git.savannah.gnu.org/cgit/grep.git/tree/README
[2]: http://lists.freebsd.org/pipermail/freebsd-current/2010-August/019353.html

Known Issues
------------

RK overflows if the pattern is large, i.e. after 32 ^ pattern_len doesn't fit
into unsigned long. This could have been improved by using better constants, but
since I don't really have understanding of those, I left it like this. Due to
this, only KMP, BM and trivial algorithms are taken into account in the
sampling. Times of other algorithms are reported but in RKs case, it might be
wrong.

BM good suffix rule was very tricky to implement. My implementation can be
improved a lot (by storing widest borders, fixing my strrnstr etc.). It became
so complex that when I tried to improve it, it broke. There is still possibly
some bugs with it.

Sampling
--------

Sampling is quite straight-forward. Take a piece of the pattern and run it
through only part of the text. I chose to use beginning of the text and the
pattern because without any analyzing, it is quite indifferent what part is
used. Then KMP, BM, and trivial are run with the same text and pattern.

Only difficulty was choosing the right values for sampling lengths. If pattern
length would have been small, trivial would have gained most benefit from it,
since it doesn't need to precompute anything. Also, by having too large text
length, sampling could have taken too much time compared to actual matching.

If the text is quite small (<10000 bytes in my testing), it doesn't matter which
algorithm is used so sampling is not beneficial in that case. Only when using
longer texts (>200000), actual sampling can be made.

