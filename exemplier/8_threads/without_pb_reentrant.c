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

#include <pthread.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

#define THREADS 4

/**
 * @author Alain Lebret
 * @version	1.0
 * @date 2012-04-10
 */

/**
 * @file without_pb_reentrant.c
 * @see pb_reentrant.c
 *
 * A simple program to show the importance of using "reentrant" functions.
 *
 */

static pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

int f_reentrant(void)
{
    static unsigned int next;

    pthread_mutex_lock(&mutex);
    /* Beginning of critical section */
    next = 1;
    next = next * 1103515245 + 12345;
    next = (unsigned int) (next / 65536) % 32768;
    /* End of critical section */
    pthread_mutex_unlock(&mutex);

    usleep(10);

    return next;
}

void *doit(void *vargp)
{
    printf("[%ld]: val = %d\n", pthread_self(), f_reentrant());
    return NULL;
}

int main(void)
{
    int i;
    pthread_t tid[4];

    for (i = 0; i < THREADS; i++) {
        pthread_create(&tid[i], NULL, doit, NULL);
    }

    for (i = 0; i < THREADS; i++) {
        pthread_join(tid[i], NULL);
    }

    exit(EXIT_SUCCESS);
}

