/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "emlog_message.h"

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

int Em_GetMessage(struct Em_Message *msg){
    size_t buffer_size = 64,
        at = 0,
        first_colon = 0;
    char *buffer = malloc(buffer_size);
    
    int c = getchar();
    
    while(c != '\n' && c != EOF){
        /* Do at + 1 so that we always have room for the NULL on the end
         * without doing another realloc */
        if(at + 1 >= buffer_size){
            assert(at + 1 == buffer_size);
            buffer_size <<= 1;
            buffer = realloc(buffer, buffer_size);
        }
        
        if(c == ':')
            first_colon = at;
        
        buffer[at++] = c;
        c = getchar();
    }
    
    buffer[at++] = '\0';
    
    msg->buffer = buffer;
    
    if(buffer[0] == '/' && buffer[1] == ':'){
        msg->categories = NULL;
        msg->message = buffer + 2;
    }
    else if(buffer[0] == '/'){
        buffer[first_colon] = '\0';
        
        msg->categories = buffer + 1;
        msg->message = buffer + first_colon + 1;
    }
    else{
        msg->categories = NULL;
        msg->message = buffer;
    }
    
    return c != EOF;
}

void Em_FreeMessage(struct Em_Message *msg){
    free(msg->buffer);
}
