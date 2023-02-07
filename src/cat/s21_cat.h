#ifndef SRC_CAT_S21_CAT_H_
#define SRC_CAT_S21_CAT_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct flags {
  int b;
  int e;
  int n;
  int s;
  int t;
  int v;
  int E;
  int T;
  int error;
};

void set_flags(struct flags *flags, char **argv, int *tmp);
void parser(struct flags *flags, char c);
void GNU_flags_parser(struct flags *flags, char **argv, int i);
void reader(struct flags flags, int argc, char **argv, int tmp);

#endif  // SRC_CAT_S21_CAT_H_