.POSIX:

SOURCES ::= test

CXXFLAGS ::= -std=c++17 -Wall -pedantic -MMD -g

test: ${SOURCES:=.o}
	$(CXX) -o $@ ${SOURCES:=.o}

-include ${SOURCES:=.d}

clean:
	rm -fv ${SOURCES:=.o} ${SOURCES:=.d}
