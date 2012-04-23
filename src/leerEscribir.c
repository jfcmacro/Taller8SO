/* leerEscribir.c */
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>

#define TAMANO  1024

int
leerEscribir(fd_set* set, int in, int out) {
  static char *buffer = NULL;
  int nCLeidos;

  if (!buffer) {
    buffer = (char *) malloc(TAMANO);
    if (!buffer) {
      return -1;
    }
  }

  if (FD_ISSET(in, set)) {

    do {

      nCLeidos = read(in, buffer, TAMANO);
      if (nCLeidos > 0) {
	if (write(out, buffer, nCLeidos) < 0) {
	  return -1;
	}
      }
      else
	return -1;

    } while (nCLeidos == TAMANO);
  }
  return 0;
}

int
validarError(fd_set* set, int in) {
  char buffer[1];

  if (FD_ISSET(in, set)) {
    if (read(in, buffer, 1) < 0) {
      return -1;
    }
  }
  return 0;
}
