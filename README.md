string-algorithms
=================

Implementation of string matching algorithms.

Directory Structure
-------------------

String searching algorithms are implemented in side ** src/algorithms/ ** and it
is compiled in to libstrpat.so. Application using that is implemented in **
src/fep.c **.

*   ** build/ ** This is where the project is most likely built.
*   ** src/ ** Location of the main source code.
        *   ** fep.c ** Fixed expression pattern searcher -> Main function for the project.
        *   ** algorithms/ ** Implementation of the string algorithms.
            *   ** trivial.c ** Trivial string searching algorithm.
            *   ** kmp.c ** Knuth-Morris-Pratt string searching algorithm.
            *   ** bm.c ** Boyer-Moore string searching algorithm.
            *   ** rk.c ** Rabin-Karp string searching algorithm.
            *   ** trivial_mem.c ** Trivial algorithm using glibc functions.
*   ** tests/ **  Directory for unit tests.
       * ** test_main.c ** Unit tests.

About the Chosen Algorithms
---------------------------

Knuth-Morris-Pratt and Boyer-Moore were quite easy to pick because they are
arguably the most well-known. For example, GNU Grep seems to use Boyer-Moore
variant for fixed string matching[1] and Knuth-Morris-Pratt is usually the first
algorithm encountered in any string algorithm course.

Rabin-Karp doesn't offer anything since it mostly shines when searching for
multiple patterns, but since it was easy to implement I chose it also.

To be able to compare running times of BM and KMP, I implemented basic trivial
algorithm which loops the text through and looks for the pattern.

Finally, after digging through the internet on string searching algorithms, I
found a suggestion that a simple fixed string searcher could be implemented with
memmem(3)[2]. Since I already read the text with mmap(2) and I didn't care about
portability (memmem is an GNU extension), I wanted to see how it would manage
against the other algorithms. I was quite suprised to see that when digging
through 244600 byte text searching for nonexisting 393 character pattern, memmem
only took 0.30 ms when for example KMP took 61 ms.

[1]:http://git.savannah.gnu.org/cgit/grep.git/tree/README
[2]:http://lists.freebsd.org/pipermail/freebsd-current/2010-August/019353.html
