/* usage.c */
#include <stdio.h>

void
usage(const char* progname) {
  fprintf(stderr, "Uso: %s socketname\n");
  exit(1);
}
