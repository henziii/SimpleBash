#include "s21_grep.h"

int main(int argc, char **argv) {
  if (argc > 2) {
    grep(argc, argv);
  }
  return 0;
}

int grep(int argc, char **argv) {
  FILE *fp = NULL;
  size_t razmer;
  Flags flags = {0};
  variables help = {0, 0, 0, 0, 0, 0, 0, REG_NEWLINE};
  int a = 1;  // результат поиска регулярного выражения в текущей строке
  char *arr = NULL;
  char string[2048] = {0};
  regmatch_t structura[1];
  regex_t *patterns = flags_parsing(&flags, &help, argc, argv);
  if (help.err == 1) {
    free(arr);
    free(patterns);
    exit(1);
  }
  compilation_with_no_flags(&flags, &help, patterns, argv);
  help.count_of_files = file_counter(argc);
  while (optind < argc) {
    if ((fp = fopen(argv[optind], "r")) == NULL) {
      if (!flags.s) {
        fprintf(stderr, "./s21_grep: %s: No such file or directory\n",
                argv[optind]);
        optind++;
      } else {
        optind++;
      }
    } else {
      while (getline(&arr, &razmer, fp) != -1) {
        help.number_of_lines++;
        a = 1;
        flag_o_and_flag_n(&flags, &help, arr, patterns, argv);
        output_with_flag_o(&flags, &help, structura, &a, string, arr, patterns,
                           razmer, argv);
        if (flags.o && !flags.c && !flags.l && !flags.v) continue;
        if ((flags.v && a != 0) || (!flags.v && a == 0)) {
          help.number_of_coincidences++;
          help.check++;
        }
        if (flags.l) {
          if (flags.c) {
            help.count = 0;
            continue;
          } else {
            if (help.check != 0) {
              printf("%s\n", argv[optind]);
              break;
            }
          }
        }
        if ((help.count_of_files == 1) ||
            (help.count_of_files != 1 && flags.h)) {
          if (((flags.v && help.count == 0 && !flags.c && !flags.l) ||
               (!flags.v && help.count != 0 && !flags.c && !flags.l)) &&
              !flags.n) {
            printf("%s", arr);
            if (arr[strlen(arr) - 1] != '\n') {
              printf("\n");
            }
          } else if (((flags.v && !flags.c && !flags.l && a != 0) ||
                      (!flags.v && !flags.c && !flags.l && a == 0)) &&
                     flags.n) {
            printf("%d:%s", help.number_of_lines, arr);
            if (arr[strlen(arr) - 1] != '\n') {
              printf("\n");
            }
          }
        } else if (help.count_of_files != 1 && !flags.h) {
          if (((flags.v && !flags.c && !flags.l && a != 0) ||
               (!flags.v && !flags.c && !flags.l && a == 0)) &&
              !flags.n) {
            printf("%s:%s", argv[optind], arr);
            if (arr[strlen(arr) - 1] != '\n') {
              printf("\n");
            }
          } else if (((flags.v && !flags.c && !flags.l && a != 0) ||
                      (!flags.v && !flags.c && !flags.l && a == 0)) &&
                     flags.n) {
            printf("%s:%d:%s", argv[optind], help.number_of_lines, arr);
            if (arr[strlen(arr) - 1] != '\n') {
              printf("\n");
            }
          }
        }
        help.count = 0;
      }
      if (help.count_of_files == 1) {
        if (flags.c && !flags.l) {
          printf("%d\n", help.number_of_coincidences);
        }
        if (flags.c && flags.l && help.number_of_coincidences > 0) {
          printf("%d\n%s\n", 1, argv[optind]);
        } else if (flags.c && flags.l && help.number_of_coincidences == 0) {
          printf("%d\n", 0);
        }
      } else {
        if (flags.c && !flags.l) {
          if (flags.h) {
            printf("%d\n", help.number_of_coincidences);
          } else {
            printf("%s:%d\n", argv[optind], help.number_of_coincidences);
          }
        }
        if (flags.c && flags.l && !flags.h) {
          if (help.number_of_coincidences > 0) {
            printf("%s:%d\n%s\n", argv[optind], 1, argv[optind]);
          } else if (help.number_of_coincidences == 0) {
            printf("%s:0\n", argv[optind]);
          }
        } else if (flags.c && flags.l && flags.h) {
          if (help.number_of_coincidences > 0) {
            printf("%d\n%s\n", 1, argv[optind]);
          } else if (help.number_of_coincidences == 0) {
            printf("0\n");
          }
        }
      }
      optind++;
      help.number_of_coincidences = 0;
      help.check = 0;
      help.number_of_lines = 0;
      fclose(fp);
    }
  }
  if (help.l > 0) {
    for (int i = 0; i < help.l; i++) {
      regfree(&patterns[i]);
    }
  }
  free(arr);
  free(patterns);
  return 0;
}

regex_t *flags_parsing(Flags *flags, variables *help, int argc, char **argv) {
  int opt;
  char arr[258];
  regex_t *patterns = (regex_t *)malloc(sizeof(regex_t));
  while ((opt = getopt_long(argc, argv, "e:ivclnhsf:o", NULL, NULL)) != -1) {
    switch (opt) {
      case 'e':
        flags->e = 1;
        if (strcmp(optarg, "\0") != 0) {
          regcomp(&patterns[help->l], optarg, help->fl);
          help->l++;
          patterns =
              (regex_t *)realloc(patterns, (help->l + 1) * sizeof(regex_t));
        } else {
          help->err = 1;
        }
        break;
      case 'i':
        flags->i = 1;
        help->fl = REG_ICASE;
        break;
      case 'v':
        flags->v = 1;
        break;
      case 'c':
        flags->c = 1;
        break;
      case 'l':
        flags->l = 1;
        break;
      case 'n':
        flags->n = 1;
        break;
      case 'h':
        flags->h = 1;
        break;
      case 's':
        flags->s = 1;
        break;
      case 'f':
        flags->f = 1;
        compilation_with_f(help, &patterns, arr);
        break;
      case 'o':
        flags->o = 1;
        break;
      case '?':
        help->err = 1;
        break;
    }
  }
  return patterns;
}

void flag_o_and_flag_n(Flags *flags, variables *help, char *arr,
                       regex_t *patterns, char **argv) {
  // обработка ситуации когда флаги о и n вместе
  // печать номера строки где нашли совпадение
  int a = 1;
  for (int i = 0; i < help->l; i++) {
    a = regexec(&patterns[i], arr, 0, NULL, 0);
    if (a == 0) {
      if (((flags->o && flags->n && !flags->c && !flags->l && !flags->v) &&
           (help->count_of_files == 1)) ||
          ((flags->o && flags->n && !flags->c && !flags->l && !flags->v &&
            flags->h) &&
           (help->count_of_files > 1))) {
        printf("%d:", help->number_of_lines);
        break;
      } else if ((flags->o && flags->n && !flags->c && !flags->l && !flags->v &&
                  !flags->h) &&
                 (help->count_of_files > 1)) {
        printf("%s:%d:", argv[optind], help->number_of_lines);
        break;
      } else {
        break;
      }
    }
  }
}

void compilation_with_no_flags(Flags *flags, variables *help, regex_t *patterns,
                               char **argv) {
  if (!flags->e && !flags->f) {
    regcomp(patterns, argv[optind], help->fl);
    help->l++;
    optind++;
  }
}

int file_counter(int argc) {
  int current_file = optind;
  int count_of_files = 0;
  while (current_file != argc) {
    count_of_files++;
    current_file++;
  }
  return count_of_files;
}

void compilation_with_f(variables *help, regex_t **patterns, char *arr) {
  FILE *tp = NULL;
  if ((tp = fopen(optarg, "r")) == NULL) {
    exit(1);
  } else {
    while (fgets(arr, 258, tp) != NULL) {
      for (int i = 0; i < (int)strlen(arr); i++) {
        if (strcmp(arr, "\n") != 0) {
          if (arr[i] == '\n') {
            arr[i] = '\0';
          }
        }
        regcomp(&(*patterns)[help->l], arr, help->fl);
        help->l++;
        *patterns =
            (regex_t *)realloc(*patterns, (help->l + 1) * sizeof(regex_t));
      }
    }
  }
  fclose(tp);
}

void output_with_flag_o(Flags *flags, variables *help, regmatch_t *structura,
                        int *a, char *string, char *arr, regex_t *patterns,
                        size_t razmer, char **argv) {
  for (int i = 0; i < help->l; i++) {  //цикл по патернам
    *a = regexec(&patterns[i], arr, 0, NULL,
                 0);  //передаем результат поиска патерна в мейн
    if (*a == 0) {
      help->count++;
      if (flags->o && !flags->c && !flags->l && !flags->v) {
        while (regexec(&patterns[i], arr, 1, structura, 0) ==
               0) {  //цикл с добавлением rmso и rmeo
          char *buf = (char *)calloc(  // создаем буфер под патерн
              (structura[0].rm_eo - structura[0].rm_so) + 2, sizeof(char));
          strncpy(buf,
                  arr + structura[0].rm_so,  //копируем патерн из строки в буфер
                  (structura[0].rm_eo - structura[0].rm_so));
          if (help->count_of_files > 1 && !flags->h && !flags->n) {
            printf("%s:", argv[optind]);
          }
          buf[strlen(buf)] = 0;
          // печать патерна
          if (buf[strlen(buf) - 1] == '\n') {
            printf("%s", buf);
          } else {
            printf("%s", buf);
            printf("\n");
          }
          free(buf);
          for (int k = structura[0].rm_eo, j = 0; k <= (int)strlen(arr);
               k++, j++) {
            string[j] = arr[k];  //записываем обрезок в буфер string[]
          }
          memset(arr, 0, razmer - 1);  //зануляем строку
          strcpy(arr, string);  //копируем обрезок в строку
          memset(string, 0, strlen(string));  //зануляем буфер
        }
      } else {
        break;
      }
    }
  }
}