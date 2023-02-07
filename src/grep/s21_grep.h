#ifndef SRC_GREP_S21_GREP_H_
#define SRC_GREP_S21_GREP_H_

#include <getopt.h>
#include <regex.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

typedef struct flags {
  int e;
  int i;
  int v;
  int c;
  int l;
  int n;
  int h;
  int s;
  int f;
  int o;
} Flags;

typedef struct variables_grep {
  int number_of_coincidences;  // счетчик совпавших строк
  int count_of_files;          // счетчик файлов
  int l;                       // счетчик регулярок
  int number_of_lines;         // счетчик строк в файле
  int check;                   // для флага l
  int count;
  int err;
  int fl;
} variables;

regex_t *flags_parsing(Flags *flags, variables *help, int argc, char **argv);
void flag_o_and_flag_n(Flags *flags, variables *help, char *arr,
                       regex_t *patterns, char **argv);
void compilation_with_no_flags(Flags *flags, variables *help, regex_t *patterns,
                               char **argv);
int file_counter(int argc);
void compilation_with_f(variables *help, regex_t **patterns, char *arr);
int grep(int argc, char **argv);
void output_with_flag_o(Flags *flags, variables *help, regmatch_t *structura,
                        int *a, char *string, char *arr, regex_t *patterns,
                        size_t razmer, char **argv);

#endif  // SRC_GREP_S21_GREP_H_