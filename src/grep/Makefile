CC=gcc
CFLAGS=-Wall -Werror -Wextra -g

all : s21_grep

s21_grep : s21_grep.c
		$(CC) $(CFLAGS) s21_grep.c -o s21_grep

clang :
		clang-format -n ~/C3_SimpleBashUtils-0/src/grep/s21_grep.c

leaks :
		leaks -atExit -- ./s21_grep -h -s -o a TestFile3

sanitize : clean
		$(CC) $(CFLAGS) s21_grep.c -fsanitize=address -g -o s21_grep

clean :
		rm -rf *.o *.a *.out *.dSYM s21_grep

rebuild : clean all