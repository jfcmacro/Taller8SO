#include <sys/ipc.h>
#include <sys/types.h>
#include <sys/shm.h>
#include <sys/sem.h>
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
  struct sembuf semaforos[1];

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

  if ((bufferSh = (char *) shmat(idSeg, 0, 0)) == (void *) 0) {
    fprintf(stderr, "No pudo ser asignado el segmento de memoria: %d %s\n",
	    errno, strerror(errno));
    exit(1);
  }

  if ((idSem = semget(key, 1, 0)) < 0) {
    fprintf(stderr, "No se pudo crear un semaforo: %d %s\n",
	    errno, strerror(errno));
    exit(1);
  }

  for (;;) {
    semaforos[0].sem_num = 0;
    semaforos[0].sem_op = -1;
    semaforos[0].sem_flg = 0;
    
    if (semop(idSem, semaforos, 1) < 0) {
      fprintf(stderr, "No fue posible senalar el semaforo: %d %s\n",
	      errno, strerror(errno));
      exit(1);
    }
    
    for (i = 0; i < PAGE_SIZE; i++) {
      write(1, (bufferSh + i), 1);
    }
    
    semaforos[0].sem_num = 0;
    semaforos[0].sem_op = 1;
    semaforos[0].sem_flg = 0;
    
    if (semop(idSem, semaforos, 1) < 0) {
      fprintf(stderr, "No fue posible senalar el semaforo: %d %s\n",
	      errno, strerror(errno));
      exit(1);
    }    
  }

  exit(0);
}
