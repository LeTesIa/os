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

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>

#include "message.h"

/**
 * @author Alain Lebret
 * @version	1.0
 * @date 2012-04-10
 */

/**
 * @file unix_msg_recv.c
 *
 * Consumer program using a System V IPC message mechanism.
 */

 /**
 * Handles a fatal error. It displays a message, then exits.
 */
void handle_fatal_error(const char *msg)
{
    perror(msg);
    exit(EXIT_FAILURE);
}

/**
 * Returns the message queue ID for the given key.
 */
int get_msgq_id(key_t key)
{
    int msgq_id;
    int msg_flag;

    msg_flag = 0666;
    msgq_id = msgget(key, msg_flag);

    if (msgq_id < 0) {
        handle_fatal_error("Error using msgget()! ");
    }

    return msgq_id;
}

int main(void)
{
    int msgq_id;
    message_t message;

    msgq_id = get_msgq_id(1234);

    /* Receive an answer of message type MSG_TYPE_HANDOUT */
    if (msgrcv(msgq_id, &message, MESSAGE_SIZE, MSG_TYPE_HANDOUT, 0) < 0) {
        handle_fatal_error("Error using msgrcv()! ");
    }

    msg_display(&message);
/*	printf("%s\n", message.content.buffer); */

    exit(EXIT_SUCCESS);
}
