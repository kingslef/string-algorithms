cmake_minimum_required(VERSION 2.6)
project(string-algorithms)

include_directories(src)

set(CMAKE_C_FLAGS "-std=c99 -pedantic-errors -Wall -Wextra")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wformat=2 -Wswitch-default")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wcast-align -Wpointer-arith")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wnested-externs")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wshadow -Wwrite-strings")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wstrict-aliasing=2 -Wunused-result")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wstrict-prototypes -Winline -Wundef")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wcast-qual")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wno-char-subscripts")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -g")

# Enable clock_gettime
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -D_POSIX_C_SOURCE=199309L")
# Enable memmem(3)
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -D_GNU_SOURCE")

add_library(strpat src/algorithms/trivial.c src/algorithms/trivial_mem.c
  src/algorithms/kmp.c src/algorithms/bm.c src/algorithms/rk.c)

if(UNIX)
  target_link_libraries(strpat m rt)
endif(UNIX)

add_executable(test_main tests/test_main.c)
target_link_libraries(test_main strpat)

add_executable(fep src/fep.c)
target_link_libraries(fep strpat rt)
