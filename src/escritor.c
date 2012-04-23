#include <sys/ipc.h>
#include <sys/types.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <sys/user.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>

#if defined(__GNU_LIBRARY__) && !defined(_SEM_SEMUN_UNDEFINED)
#else
union semun {
  int val;                    /* valor para SETVAL */
  struct semid_ds *buf;       /* buffer para IPC_STAT, IPC_SET */
  unsigned short int *array;  /* array para GETALL, SETALL */
  struct seminfo *__buf;      /* buffer para IPC_INFO */
};
#endif

int
main(int argc, char *argv[]) {

  int idSeg;
  int idSem;
  char *bufferSh;
  int i;
  int key;
  union semun semunion;
  int veces;
  struct sembuf semaforos[1];

  if (argc != 2) {
    fprintf(stderr, "Uso: %s \n", argv[0]);
    exit(1);
  }

  key = strtol(argv[1], NULL, 10);

  if ((idSeg = shmget(key, PAGE_SIZE, IPC_CREAT | IPC_EXCL | 0660 )) < 0) {
    fprintf(stderr, "Fallo al crear el segmento de memoria debido a: %d %s\n",
	    errno, strerror(errno));
    exit(1);
  }

  if ((bufferSh = (char *) shmat(idSeg, 0, 0)) == (void *) 0) {
    fprintf(stderr, "No pudo ser asignado el segmento de memoria: %d %s\n",
	    errno, strerror(errno));
    exit(1);
  }

  if ((idSem = semget(key, 1, IPC_CREAT | IPC_EXCL | 0660)) < 0) {
    fprintf(stderr, "No se pudo crear un semaforo: %d %s\n",
	    errno, strerror(errno));
    exit(1);
  }

  semunion.val = 0;

  if (semctl(idSem, 0, SETVAL, semunion) < 0) {
    fprintf(stderr, "No se pudo establecer el valor del semaforo: %d %s\n",
	    errno, strerror(errno));
    exit(1);
  }

  for (;;) {

    for (i = 0; i < PAGE_SIZE; i++) {
      bufferSh[i]  = ((i + veces) % ('Z' - 'A' + 1)) + 'A';
    }

    veces = (veces + 1) % ('Z' - 'A' + 1); 

    semaforos[0].sem_num = 0;
    semaforos[0].sem_op = 1;
    semaforos[0].sem_flg = 0;

    if (semop(idSem, semaforos, 1) < 0) {
      fprintf(stderr, "No fue posible senalar el semaforo: %d %s\n",
	      errno, strerror(errno));
      exit(1);
    }

    semaforos[0].sem_num = 0;
    semaforos[0].sem_op = -1;
    semaforos[0].sem_flg = 0;

    if (semop(idSem, semaforos, 1) < 0) {
      fprintf(stderr, "No fue posible senalar el semaforo: %d %s\n",
	      errno, strerror(errno));
      exit(1);
    }
  }

  exit(0);
}
  
    
  
  
