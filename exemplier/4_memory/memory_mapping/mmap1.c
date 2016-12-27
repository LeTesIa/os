/*
 * ENSICAEN
 * 6 Boulevard Maréchal Juin
 * F-14050 Caen Cedex
 *
 * Unix System Programming Examples / Exemplier de programmation système Unix
 * "Memory management" / "Gestion de la mémoire"
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
 * @file mmap1.c
 * 
 * A memmory mapping example using the \c mmap primitive.
 */

#include <stdio.h>  /* pour printf() */
#include <stdlib.h> /* pour exit() et execl()*/
#include <unistd.h> /* pour fork() */
#include <fcntl.h> /* pour open() */
#include <sys/mman.h> /* pour mmap() */
#include <sys/stat.h> /* pour stat() */
#include <sys/types.h>
#include <ctype.h> /* pour isalpha(), isspace() */
#include <assert.h>

#define TERMINAL 1

size_t get_file_size(const char *filename) {
	struct stat st;
	stat(filename, &st);
	printf("%lld\n", st.st_size);
	return (size_t) st.st_size;
}

int main(int argc, char **argv) {
	int fd;
	int i;
	size_t file_size;
	char *projection;

	file_size = get_file_size(argv[1]);

	fd = open(argv[1], O_RDONLY, 0);

	/* Project the file content to memory */
	projection = mmap(NULL, file_size, PROT_READ, MAP_SHARED, fd, 0);
	assert(projection != MAP_FAILED);

	/* Write the content to the terminal */
	write(TERMINAL, projection, file_size);

	/* Do some stuff with data */
	for (i = 0; i < file_size; i++) {
		char c;

		c = projection[i];
		printf("%c", c);
		if (!isalpha(c) && !isspace(c)) {
			putchar(c);
		}
		if (i % 80 == 79) {
			putchar('\n');
		}
	}

	/* Unmapping memory */
	munmap(projection, file_size);

	close(fd);

	exit(EXIT_SUCCESS);
}