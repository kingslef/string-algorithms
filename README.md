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
*   ** tests/ **  Directory for unit tests.
       * ** test_main.c ** Unit tests.
