CFLAGS = -Wall -Wpedantic -Wextra -Wfatal-errors \
		 -g -std=gnu11 -O0 -fprofile-arcs -ftest-coverage -fopenmp -pthread
LDLIBS = -fopenmp -latomic

build:
	cc $(CFLAGS) $(LDLIBS) -o bin/lispy src/main.c src/parser.c  src/mpc.c

run:
	bin/lispy