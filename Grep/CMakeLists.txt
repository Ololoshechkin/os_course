cmake_minimum_required (VERSION 2.6)
project (mygrep)
set (CMAKE_CXX_STANDARD 17)
add_executable(mygrep my_grep.cpp trie.h trie.cpp fs_utils.h fs_utlis.cpp)
