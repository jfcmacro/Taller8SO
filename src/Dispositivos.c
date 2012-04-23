// Dispositivos.c
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

char*
inicializarBuffer(int nBloque) {
  char *buffer;

  buffer = malloc(1024);

  if (buffer)
    sprintf(buffer, "Numero Bloque: %d", nBloque);

  return buffer;
}

int
main(int argc, char *argv[]) {

  int fdDispositivo;
  char *buffer;
  int i;
  int nCarEscritos;

  fdDispositivo = open("/dev/fd0", O_RDWR);

  if (fdDispositivo < 0) {
    fprintf(stderr, "Error al abrir el dispositivo: %d %s\n",
	    errno, strerror(errno));
    exit(1);
  }

  for (i = 0;i < 1440; i++) {
    if ((buffer = inicializarBuffer(i))) {
      fprintf(stdout, "Escribiendo buffer: %d\n", i);
      if ((nCarEscritos = write(fdDispositivo, buffer, 1024)) < 0) {
	fprintf(stderr, "No se pudo escribir en el dispositivo: %d %s\n",
		errno, strerror(errno));
	exit(1);
      }
      free(buffer);
    }
  }

  exit(0);
}
