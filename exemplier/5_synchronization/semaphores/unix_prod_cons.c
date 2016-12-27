/*
 * ENSICAEN
 * 6 Boulevard Maréchal Juin
 * F-14050 Caen Cedex
 *
 * Unix System Programming Examples / Exemplier de programmation système Unix
 * "Process synchronization" / "Synchronisation des processus"
 *
 * Copyright (C) 1995-2016 Alain Lebret (alain.lebret@ensicaen.fr)
 *
 * This work is licensed under a Creative Commons Attribution-ShareAlike 4.0
 * International License. You should have received a copy of the License.
 * If not, see https://creativecommons.org/licenses/by-sa/4.0/.
 */

/**
 * @author Alain Lebret <alain.lebret@ensicaen.fr>
 * @version	1.0
 * @date 2011-12-01
 */
/**
 * @author Alain Lebret (2011)
 * @author Janet Davis (2006)
 * @author Henry Walker (2004)
 * @version	1.0
 * @date 2011-12-01
 */

/**
 * @file prod_cons.c
 *
 * Producer-consumer program using a set of two IPC semaphores.
 */

#include <signal.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define EVER ;;
#define BUFFER_SIZE 10
#define BUFFER_SPACE 0
#define BUFFER_USED 1

typedef int semaphore_t;

/**
 * Handles a fatal error. It displays a message, then exits.
 */
void handle_fatal_error(char *msg) {
	perror(msg);
	exit(EXIT_FAILURE);
}

/**
 * Modifies the value of the specified System V semaphore.
 * @param sem The identifier of the set of semaphores
 * @param sem_num The index of the semaphore in the set
 * @param new_value The new value to associate to the semaphore
 */
void modify_semaphore_value(semaphore_t sem, int sem_num, int new_value) {
	struct sembuf sb[1];

	sb[0].sem_num = sem_num;
	sb[0].sem_op = new_value;
	sb[0].sem_flg = 0;

	if (semop(sem, sb, 1) != 0)
		handle_fatal_error("Error trying to modify semaphore's value\n");
}

/**
 * Performs a P() operation ("wait") on a semaphore.
 * @param sem Identifier of the semaphore.
 */
void P(semaphore_t sem, int sem_num) {
	modify_semaphore_value(sem, sem_num, -1);
}

/**
 * Performs a V() operation ("signal") on a semaphore.
 * @param sem Indentifier of the semaphore.
 */
void V(semaphore_t sem, int sem_num) {
	modify_semaphore_value(sem, sem_num, 1);
}

/**
 * Writes values to a shared buffer protected by semaphores.
 * @param buffer Buffer to read on.
 * @param id_semaphores Identifier of the set of two semaphores.
 */
void produce(int *buffer, int id_semaphores) {
	int in_index;
	int next_value;

	srand((unsigned int) getpid());
	in_index = 0;

	for (EVER) {  /* produce next_value */
		sleep((unsigned int) (5 + rand() % 5));
		next_value = rand() % 100;
		printf("producer: %d\n", next_value);

		P(id_semaphores, BUFFER_SPACE);

		buffer[in_index] = next_value;
		in_index = (in_index + 1) % BUFFER_SIZE;

		V(id_semaphores, BUFFER_USED);
	}
}

/**
 * Reads values from a shared buffer a shared buffer protected by semaphores.
 * @param buffer Buffer to read on.
 * @param id_semaphore Identifier of the set of two semaphores.
 */
void consume(int *buffer, int id_semaphores) {
	int out_index;
	int next_value;

	srand((unsigned int) getpid());
	out_index = 0;

	for (EVER) { /* consume next_value */

		P(id_semaphores, BUFFER_USED);

		next_value = buffer[out_index];
		out_index = (out_index + 1) % BUFFER_SIZE;

		V(id_semaphores, BUFFER_SPACE);

		/* consume next */
		printf("consumer: %d\n", next_value);
		sleep((unsigned int) (10 + rand() % 10));
	}
}

void handler(int signal) {
}

int main(void) {
	int *buffer;
	int id_semaphore;
	int id_memory;

	/* creation of the set of 2 IPC semaphores */
	id_semaphore = semget(IPC_PRIVATE, 2, 0644 | IPC_CREAT | IPC_EXCL);
	if (id_semaphore == -1) {
		handle_fatal_error("Error creating the set of semaphores\n");
	}

	/* initialization of the two semaphores */
	semctl(id_semaphore, BUFFER_USED, SETVAL, 0);
	semctl(id_semaphore, BUFFER_SPACE, SETVAL, BUFFER_SIZE);

	/* creation and attachment of a shared memory */
	id_memory = shmget(IPC_PRIVATE, BUFFER_SIZE * sizeof(int),
								0644 | IPC_CREAT | IPC_EXCL);
	if (id_memory == -1) {
		semctl(id_semaphore, 0, IPC_RMID);
		handle_fatal_error("Error creating the shared memory\n");
	}
	buffer = shmat(id_memory, NULL, 0);

	if (fork() == 0) produce(buffer, id_semaphore);
	if (fork() == 0) consume(buffer, id_semaphore);

	/* remove IPC semaphores */
	signal(SIGINT, handler);
	pause();
	semctl(id_semaphore, 0, IPC_RMID);
	shmctl(id_memory, IPC_RMID, NULL);

	exit(EXIT_SUCCESS);
}