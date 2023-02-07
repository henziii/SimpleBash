#include "s21_cat.h"

int main(int argc, char **argv) {
  if (argc > 1) {
    struct flags flags = {0};
    int tmp = 1;
    set_flags(&flags, argv, &tmp);
    reader(flags, argc, argv, tmp);
  }
  return 0;
}

void set_flags(struct flags *flags, char **argv, int *tmp) {
  for (int i = 1; argv[i][0] == '-'; i++) {
    if (argv[i][1] == '-') {
      GNU_flags_parser(flags, argv, i);
    } else {
      for (int j = 1; argv[i][j] != '\0'; j++) {
        parser(flags, argv[i][j]);
      }
    }
    (*tmp)++;
  }
}

void parser(struct flags *flags, char c) {
  switch (c) {
    case 'b':
      flags->b = 1;
      break;

    case 'e':
      flags->e = 1;
      flags->v = 1;
      flags->E = 1;
      break;

    case 'n':
      flags->n = 1;
      break;

    case 's':
      flags->s = 1;
      break;

    case 't':
      flags->t = 1;
      flags->v = 1;
      flags->T = 1;
      break;

    case 'v':
      flags->v = 1;
      break;

    case 'E':
      flags->E = 1;
      break;

    case 'T':
      flags->T = 1;
      break;

    default:
      flags->error = 1;
      break;
  }
}

void GNU_flags_parser(struct flags *flags, char **argv, int i) {
  if (strcmp(argv[i], "--number-nonblank") == 0) {
    flags->b = 1;
  } else if (strcmp(argv[i], "--number") == 0) {
    flags->n = 1;
  } else if (strcmp(argv[i], "--squeeze-blank") == 0) {
    flags->s = 1;
  } else {
    flags->error = 1;
  }
}

void reader(struct flags flags, int argc, char **argv, int tmp) {
  FILE *fp = NULL;
  char prev_char;
  int curr_ch;
  if (flags.error) {
    exit(1);  // нужный нам флаг не обнаружен
  }
  for (int i = tmp; i < argc; i++) {
    if ((fp = fopen(argv[i], "r")) == NULL) {
      // printf("File not found\n");
      exit(1);
    }
    int str_counter = 1;
    int line = 0;
    for (prev_char = '\n'; (curr_ch = getc(fp)) != EOF; prev_char = curr_ch) {
      if (prev_char == '\n') {
        if (flags.s) {
          if (curr_ch == '\n') {
            if (line) {
              continue;
            }
            line = 1;
          } else {
            line = 0;
          }
        }
        if (flags.b) {
          flags.n = 0;
          if (curr_ch != '\n') {
            printf("%6d\t", str_counter++);
          }
        }
        if (flags.n) {
          printf("%6d\t", str_counter++);
        }
      }
      if (flags.E) {
        if (curr_ch == '\n') {
          printf("$");
        }
      }
      if (flags.T) {
        if (curr_ch == 9) {
          printf("^");
          curr_ch = 'I';
        }
      }
      if (flags.v) {
        if (curr_ch >= 0 && curr_ch <= 31 && curr_ch != 9 && curr_ch != 10) {
          curr_ch = curr_ch + 64;
          printf("^");
        } else if (curr_ch == 127) {
          printf("^");
          curr_ch = '?';
        } else if (curr_ch >= 128 && curr_ch <= 159) {
          curr_ch = curr_ch - 64;
        } else if (curr_ch >= 160 && curr_ch <= 255) {
          curr_ch = curr_ch - 128;
        }
      }
      putchar(curr_ch);
    }
    fclose(fp);
  }
}
