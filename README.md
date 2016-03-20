string-algorithms
=================

**Moved to https://gitlab.com/aatos/string-algorithms**

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

But basically the sampling is choosing between trivial algorithm and BM, and
that could also be possible by analyzing the pattern length and text length.

Example output
--------------

### Test ###

``` sh
~/string-algorithms/build> ./test_main
trivial: no matches
trivial: no matches
trivial: no matches
trivial: match at 0
trivial: match at 3
trivial: match at 8, 16
trivial: match at 28
trivial: match at 6
trivial: match at 16
kmp: no matches
kmp: no matches
kmp: no matches
kmp: match at 0
kmp: match at 3
kmp: match at 8, 16
kmp: match at 28
kmp: match at 6
kmp: match at 16
bm: no matches
bm: no matches
bm: no matches
bm: match at 0
bm: match at 3
bm: match at 8, 16
bm: match at 28
bm: match at 6
bm: match at 16
rk: no matches
rk: match at 0
rk: match at 3
rk: match at 8, 16
rk: match at 28
rk: match at 6
rk: match at 16
trivial_mem: no matches
trivial_mem: no matches
trivial_mem: no matches
trivial_mem: match at 0
trivial_mem: match at 3
trivial_mem: match at 8, 16
trivial_mem: match at 28
trivial_mem: match at 6
trivial_mem: match at 16
Tests run: 25
All tests passed
```

### Running the sampler through longer file ###

For testing larger files, I used books in [Project
Gutenberg](http://www.gutenberg.org/wiki/Main_Page), but since they are encoded
with iso8859 or utf8 and this application only supports ascii, they have to be converted.

``` sh
~/string-algorithms/build> wget -qO - http://www.gutenberg.org/files/45641/45641-8.txt | iconv -f iso8859-1 -t ASCII//TRANSLIT > text.txt
~/string-algorithms/build> ./fep "Project Gutenberg" text.txt
Sampling..
kmp: match at 4, 246
bm: match at 4, 246
trivial: match at 4, 246
Sampling done.
kmp: match at 4, 246, 226360, 227168, 227576, 227661, 227981, 228320, 228556, 228849, 228928, 229473, 229645, 229736, 230614, 230730, 230807, 230865, 231193, 231304, 231658, 231854, 231957, 232230, 232416, 232547, 233109, 233250, 233565, 233736, 233910, 234051, 234207, 234317, 234606, 234739, 234977, 235198, 235558, 235641, 235808, 235890, 235999, 236171, 236229, 239208, 239364, 239601, 239693, 239801, 239825, 240244, 240296, 240502, 240572, 240782, 240836, 241148, 241881, 241931, 243364, 243644, 243739, 243970, 244381, 244443
bm: match at 4, 246, 226360, 227168, 227576, 227661, 227981, 228320, 228556, 228849, 228928, 229473, 229645, 229736, 230614, 230730, 230807, 230865, 231193, 231304, 231658, 231854, 231957, 232230, 232416, 232547, 233109, 233250, 233565, 233736, 233910, 234051, 234207, 234317, 234606, 234739, 234977, 235198, 235558, 235641, 235808, 235890, 235999, 236171, 236229, 239208, 239364, 239601, 239693, 239801, 239825, 240244, 240296, 240502, 240572, 240782, 240836, 241148, 241881, 241931, 243364, 243644, 243739, 243970, 244381, 244443
trivial: match at 4, 246, 226360, 227168, 227576, 227661, 227981, 228320, 228556, 228849, 228928, 229473, 229645, 229736, 230614, 230730, 230807, 230865, 231193, 231304, 231658, 231854, 231957, 232230, 232416, 232547, 233109, 233250, 233565, 233736, 233910, 234051, 234207, 234317, 234606, 234739, 234977, 235198, 235558, 235641, 235808, 235890, 235999, 236171, 236229, 239208, 239364, 239601, 239693, 239801, 239825, 240244, 240296, 240502, 240572, 240782, 240836, 241148, 241881, 241931, 243364, 243644, 243739, 243970, 244381, 244443
rk: too long pattern!
trivial_mem: match at 4, 246, 226360, 227168, 227576, 227661, 227981, 228320, 228556, 228849, 228928, 229473, 229645, 229736, 230614, 230730, 230807, 230865, 231193, 231304, 231658, 231854, 231957, 232230, 232416, 232547, 233109, 233250, 233565, 233736, 233910, 234051, 234207, 234317, 234606, 234739, 234977, 235198, 235558, 235641, 235808, 235890, 235999, 236171, 236229, 239208, 239364, 239601, 239693, 239801, 239825, 240244, 240296, 240502, 240572, 240782, 240836, 241148, 241881, 241931, 243364, 243644, 243739, 243970, 244381, 244443

Run times:
kmp         3.15 ms
        (+2.87 ms to sampling)

bm          0.28 ms
        (+0.00 ms to sampling)

trivial     1.86 ms
        (+1.57 ms to sampling)

rk          0.02 ms
        (-0.27 ms to sampling)

trivial_mem  0.53 ms
        (+0.24 ms to sampling)


Sampling chose the best algorithm
 Still faster than second fastest => Worth it

Text length was 244600 and pattern length was 17 characters
```

Still sampling seems to fail, so sample sizes would require some fine tuning:
``` sh
~/string-algorithms/build> ./fep "Information about the Project Gutenberg" text.txt
Sampling..
kmp: no matches
bm: no matches
trivial: no matches
Sampling done.
kmp: match at 240760
bm: match at 240760
trivial: match at 240760
rk: too long pattern!
trivial_mem: match at 240760

Run times:
kmp         2.98 ms
        (+1.09 ms to sampling)

bm          0.24 ms
        (-1.64 ms to sampling)

trivial     1.89 ms
        (+0.00 ms to sampling)

rk          0.01 ms
        (-1.87 ms to sampling)

trivial_mem  0.07 ms
        (-1.82 ms to sampling)

** Algorithm chosen by sampling (bm) compared to the fastest algorithm (trivial) was:
   +1.64 ms + 0.09 ms (time took by sampling per algorithm) = +1.73 ms slower

Text length was 244600 and pattern length was 39 characters
```
