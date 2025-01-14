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
#include <unistd.h>    /* fork(), ...  */
#include <signal.h>    /* sigaction() */
#include <string.h>    /* memset() */

#define DURATION 5

/**
 * @author Alain Lebret <alain.lebret@ensicaen.fr>
 * @version	1.0
 * @date 2011-12-01
 */

/**
 * @file signal_02.c
 *
 * A simple program that uses POSIX signals and handles SIGALRM.
 */

/**
 * @brief Defines the new handler of the SIGALRM signal.
 * @param signal Number of the signal
 */
void handle_alarm(int signal)
{
    if (signal == SIGALRM) {
        printf("\nToo late!\n");
        exit(EXIT_FAILURE);
    }
}

int main(void)
{
    struct sigaction action;
    int value;
    int remaining_time;

    /* Clean up the structure before using it */
    memset(&action, '\0', sizeof(action));

    /* Set the new handler */
    action.sa_handler = &handle_alarm;

    /* Install the new handler of the SIGALRM signal */
    sigaction(SIGALRM, &action, NULL);

    printf("You have %d seconds to enter a number: ", DURATION);

    /* The OS will send an alarm signal to the process in 'DURATION' sec. */
    alarm(DURATION);

    /* Wait for the user to enter a value */
    scanf("%d", &value);

    /* Deactivate the sending of an alarm signal by the OS */
    remaining_time = alarm(0);

    printf("\nElapsed time: %d seconds.\n", 5 - remaining_time);

    exit(EXIT_SUCCESS);
}
