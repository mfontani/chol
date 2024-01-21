.PHONY: run_tests
run_tests: test
	@# prove -v ./test
	prove -v /bin/bash :: -c 'valgrind -s --tool=memcheck --leak-check=full --error-exitcode=99 ./test'

C_FILES=$(wildcard */*.c)
C_FILES+=main.c
H_FILES=$(wildcard */*.h)

test: $(C_FILES) $(H_FILES)
	$(CC) $(CFLAGS) -o test $(C_FILES)
