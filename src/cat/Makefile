CC=gcc
CFLAGS=-Wall -Werror -Wextra -g

all : s21_cat

s21_cat : s21_cat.c
		$(CC) $(CFLAGS) s21_cat.c -o s21_cat

clang :
		clang-format -n ~/C3_SimpleBashUtils-0/src/cat/s21_cat.c

leaks :
		leaks -atExit -- ./s21_cat

sanitize : clean
		$(CC) $(CFLAGS) s21_cat.c -fsanitize=address -g -o s21_cat

clean :
		rm -rf *.o *.a *.dSYM s21_cat

rebuild : clean all