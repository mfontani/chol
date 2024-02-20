.PHONY: run_tests
run_tests: test
	prove -v /bin/bash :: -c 'valgrind -s --tool=memcheck --leak-check=full --show-leak-kinds=all --error-exitcode=99 ./test'

C_FILES=$(wildcard */*.c)
C_FILES+=main.c
C_FILES+=../../tap/tap.c
H_FILES=$(wildcard */*.h) ../../tap/tap.h ../dyn_arr.h

test: $(C_FILES) $(H_FILES)
	$(CC) $(CFLAGS) -ggdb3 -o test $(C_FILES)
