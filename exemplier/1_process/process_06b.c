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

#include <stdint.h>    /* C99 int types */
#include <inttypes.h>  /* C99 int types */
#include <stdio.h>     /* printf() */
#include <stdlib.h>    /* exit() and execl()*/
#include <unistd.h>    /* fork() */
#include <sys/types.h> /* pid_t */
#include <sys/wait.h>  /* wait() */

/**
 * @author Alain Lebret <alain.lebret@ensicaen.fr>
 * @version	1.1
 * @date 2017-12-31
 */

/**
 * @file process_06b.c
 *
 * A simple program that clones a process using the \c fork() primitive. The
 * child is "replaced" by a new program using one of the \c exec() family
 * functions.
 */

/**
 * @brief Handles a fatal error and exit. 
 *
 * It displays the given error message, then exits.
 * @param msg The error message to display before exiting.
 */
void handle_fatal_error_and_exit(const char *msg)
{
    perror(msg);
    exit(EXIT_FAILURE);
}

/**
 * @brief Manages the parent process. 
 *
 * The parent is waiting for his child to exit.
 */
void manage_parent()
{
    pid_t child;
    int32_t status;

    printf("Parent process (PID %" PRId32 ")\n", getpid());

    child = wait(&status);
    if (WIFEXITED(status)) {
        printf("%d : child %"
            PRId32 " has finished is work (code: %" PRId32 ")\n",
            getpid(), child, WEXITSTATUS(status));
    }
}

/**
 * @brief Manages the child process. 
 *
 * The child process is calling the execlp()) function to execute the
 * \em gnuplot program.
 */
void manage_child()
{
    const char *path = "gnuplot";
    const char *command = "gnuplot";
    const char *argument1 = "-persist";
    const char *argument2 = "command.gp";

    printf("Child process (PID %" PRId32 ")\n", getpid());
    printf("Child is going to be replaced by Gnuplot program. Oups!!!\n");

    if (execlp(path, command, argument1, argument2, (void *) 0) == -1) {
        handle_fatal_error_and_exit("Unable to run Gnuplot using execlp().");
    }
}

int main(void)
{
    pid_t pid;

    pid = fork();
    if (pid == -1) {
        handle_fatal_error_and_exit("Error [fork()]: ");
    }

    if (pid > 0) {
        manage_parent();
    } else {
        manage_child();
    }

    exit(EXIT_SUCCESS);
}
