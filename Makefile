SOURCES ::= test generate

CC ::= clang++
CXX ::= clang++
CXXFLAGS ::= -std=c++14 -Wall -pedantic -MMD -g

test: ${SOURCES:=.o}

-include ${SOURCES:=.d}
