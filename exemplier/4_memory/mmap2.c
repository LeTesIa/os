/*
 * Unix System Programming Examples / Exemplier de programmation système Unix
 *
 * Copyright (C) 1995-2022 Alain Lebret <alain.lebret [at] ensicaen [dot] fr>
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include <stdio.h>     /* printf() */
#include <stdlib.h>    /* exit() and execl()*/
#include <unistd.h>    /* fork(), close() */
#include <fcntl.h>     /* open() opening flags and file modes */
#include <sys/mman.h>  /* mmap() */
#include <sys/stat.h>  /* stat() */
#include <sys/types.h>
#include <ctype.h>     /* isalpha(), isspace() */
#include <string.h>    /* memcpy() */
#include <assert.h>

/**
 * @author Alain Lebret <alain.lebret@ensicaen.fr>
 * @version	1.0
 * @date 2011-12-01
 */

/**
 * @file mmap2.c
 *
 * Memory mapping using mmap. This program do the same as the \em cp command.
 */

long get_file_size(const char *filename)
{
    struct stat st;

    stat(filename, &st);
    printf("%ld\n", st.st_size);

    return (long) st.st_size;
}

int main(int argc, char *argv[])
{
    int fdin;
    int fdout;
    char *src;
    char **dst;
    long file_size;

    if (argc != 3) {
        printf("Usage: mmap2 <src> <dest>\n");
        exit(EXIT_FAILURE);
    }

    file_size = get_file_size(argv[1]);

    fdin = open(argv[1], O_RDONLY, 0);
    fdout = open(argv[2], O_RDWR | O_CREAT | O_TRUNC, 0666);

    /* Look for the last byte in the destination file */
    lseek(fdout, file_size - 1, SEEK_SET);

    /* Write an empty char */
    if (write(fdout, "", 1) == -1) {
        perror("Error using write(): ");
    }

    /* Project the input file in memory */
    src = mmap(NULL, file_size, PROT_READ, MAP_SHARED, fdin, 0);
    assert(src != MAP_FAILED);

    /* Same for the output one */
    dst = mmap(NULL, file_size, PROT_READ | PROT_WRITE, MAP_SHARED, fdout, 0);
    assert(src != MAP_FAILED);

    /* Performs a memory copy from src to dst */
    memcpy(dst, src, file_size);

    exit(EXIT_SUCCESS);
}

