/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "emlog.h"
#include "emlog_message.h"
#include "emlog_backend.h"

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>

static struct Em_Context ctx;

void handler(int x){
    (void)x;
    Em_DestroyContext(&ctx);
    exit(EXIT_FAILURE);
}

int main(int argc, char **argv){
    struct Em_Message msg;
    Em_CreateContext(&ctx);
    
    Em_CreateIntermediateDirectories("Test0/Test1/Test2");
    
    signal(SIGINT, handler);
    signal(SIGABRT, handler);
    signal(SIGTERM, handler);
    
    while(Em_GetMessage(&msg)){
        Em_HandleMessage(&ctx, &msg);
        Em_FreeMessage(&msg);
    }
    
    Em_DestroyContext(&ctx);
}
