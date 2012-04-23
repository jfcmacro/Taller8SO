#include <sys/ipc.h>
#include <sys/types.h>
#include <sys/shm.h>
#include <sys/user.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>

int
main(int argc, char *argv[]) {

  int idSeg;
  int idSem;
  char *bufferSh;
  int i;
  int key;

  if (argc != 2) {
    fprintf(stderr, "Uso: %s \n", argv[0]);
    exit(1);
  }

  key = strtol(argv[1], NULL, 10);

  if ((idSeg = shmget(key, PAGE_SIZE, 0)) < 0) {
    fprintf(stderr, "Fallo al crear el segmento de memoria debido a: %d %s\n",
	    errno, strerror(errno));
    exit(1);
  }

  if (shmctl(idSeg, IPC_RMID, NULL) < 0) {
    fprintf(stderr, "Fallo al borrar el segmento de memoria debido a: %d %s\n",
	    errno, strerror(errno));
    exit(1);
  }

  if ((idSem = semget(key, 1, 0)) < 0) {
    fprintf(stderr, "No se pudo obtener el identificar del semaforo: %d %s\n",
	    errno, strerror(errno));
    exit(1);
  }

  if (semctl(idSem, 1, IPC_RMID, 0) < 0) {
    fprintf(stderr, "No se pudo borrar el semaforo: %d %s\n",
	    errno, strerror(errno));
    exit(1);
  }

  exit(0);
}
