/*
 * ENSICAEN
 * 6 Boulevard Maréchal Juin
 * F-14050 Caen Cedex
 *
 * Unix System Programming Examples / Exemplier de programmation système Unix
 * "Files, pipes and fifo" / "Fichiers et entrées-sorties"
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
 * @file mkfifo_consumer.c
 *
 * A consumer program that uses a named pipe (mkfifo) to receive messages from
 * a producer.
 */

#include <stdio.h>  /* pour printf() */
#include <stdlib.h> /* pour exit() et execl()*/
#include <unistd.h> /* pour fork() */
#include <sys/types.h> /* pour pid_t et mkfifo() */
#include <sys/stat.h> /* pour mkfifo() */
#include <ctype.h>
#include <fcntl.h>
#include <string.h>

#define BUFFER_SIZE 100

/**
 * Handles a fatal error. It displays a message, then exits.
 */
void handle_fatal_error(const char *msg) {
	perror(msg);
	exit(EXIT_FAILURE);
}

/**
 * Open the pipe with the specified name in a read only mode.
 * @param name Name of the pipe.
 * @return The pipe descriptor.
 */
int open_pipe(const char *name) {
	int pd;

	pd = open(name, O_RDONLY);
	if (pd == -1) {
		handle_fatal_error("\nError when trying to open the named pipe.\n");
	}
	return pd;
}

int main(void) {
	char buffer[BUFFER_SIZE];
	int pd;
	ssize_t message_length;

	pd = open_pipe("./testfifo");

	strcpy(buffer, "");
	message_length = read(pd, buffer, BUFFER_SIZE);

	if (message_length > 0) {
		printf("\nRead message from the pipe...\n");
		printf("%s\n", buffer);
	} else {
		printf("Named pipe is empty.\n");
	}

	close(pd);
	unlink("./testfifo");

	exit(EXIT_SUCCESS);
}