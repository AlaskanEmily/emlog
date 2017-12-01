/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "emlog.h"
#include "emlog_message.h"

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LOG_ROTATIONS 3
#define MAX_LOG_SIZE 10

struct Em_Log{
    char *path;
    size_t path_len;
    FILE *file;
    size_t size;
    unsigned char rotation;
};

/* path must be able to hold path_len + 3 characters */
static void Em_ChooseLog(struct Em_Log *out,
    char *path,
    size_t path_len){
    
    unsigned i,
        lowest_time0 = ~((unsigned)0),
        lowest_time1 = ~((unsigned)0),
        lowest_time_rotation;
    
    path[path_len] = '_';
    path[path_len+2] = 0;
    
    out->path = path;
    out->path_len = path_len;
    
    for(i = 0; i < MAX_LOG_ROTATIONS; i++){
        path[path_len + 1] = '0' + i;
        if(!Em_FileExistsFile(path)){
            out->file = fopen(path, "wb");
            out->size = 0;
            out->rotation = i;
            return;
        }
        else{
            unsigned time0, time1;
            Em_FileGetLastAccessTime(path, &time0, &time1);
            
            if(time0 < lowest_time0 ||
                (time0 == lowest_time0 && time1 < lowest_time1)){
                lowest_time_rotation = i;
                lowest_time0 = time0;
                lowest_time1 = time1;
            }
        }
    }
    
    path[path_len + 1] = '0' + lowest_time_rotation;
    
    out->rotation = lowest_time_rotation;
    {
        const size_t size = Em_GetFileSize(path);
        if(size > MAX_LOG_SIZE){
            out->size = 0;
            out->file = fopen(path, "wb");
        }
        else{
            out->size = size;
            out->file = fopen(path, "ab");
        }
    }
}

void Em_CreateContext(struct Em_Context *ctx){
    memset(ctx, 0, sizeof(struct Em_Context));
}

void Em_DestroyContext(struct Em_Context *ctx){
    size_t i;
    for(i = 0; i < ctx->num_logs; i++){
        fclose(ctx->logs[i].file);
        free(ctx->logs[i].path);
    }
    free(ctx->logs);
    
    memset(ctx, 0, sizeof(struct Em_Context));
}

void Em_HandleMessage(struct Em_Context *ctx, const struct Em_Message *msg){
    
    FILE *file = NULL;
    struct Em_Log *dest_log = NULL;
    
    if(msg->categories == NULL){
        file = stdout;
        goto em_handle_msg_found_log;
    }
    else{
        const char *const path = msg->categories;
        const size_t path_len = strlen(path);
        size_t i;
        for(i = 0; i < ctx->num_logs; i++){
            if(ctx->logs[i].path_len == path_len){
                if(memcmp(ctx->logs[i].path, path, path_len) == 0){
                    file = ctx->logs[i].file;
                    goto em_handle_msg_found_log;
                }
            }
        }
        
        /* If we didn't return, then we did not find the log.
         * TODO: Try to open the new file before any allocations. */
        if(ctx->num_logs >= ctx->cap_logs){
            const size_t new_cap = (ctx->cap_logs <<= 1);
            assert(ctx->num_logs == ctx->cap_logs);
            ctx->logs = realloc(ctx->logs, sizeof(struct Em_Log) * new_cap);
        }
        
        dest_log = ctx->logs + ctx->num_logs++;
        {
            char *path_buffer = malloc(path_len + 3);
            memcpy(path_buffer, path, path_len);
            Em_ChooseLog(dest_log, path_buffer, path_len);
        }
        file = dest_log->file;
    }

em_handle_msg_found_log:
    {
        const size_t message_size = strlen(msg->message);
        if(dest_log){
            /* Plus one because of the newline */
            dest_log->size += message_size + 1;
            
            if(dest_log->size > MAX_LOG_SIZE){
                fclose(dest_log->file);
                dest_log->file = NULL;
                Em_ChooseLog(dest_log, dest_log->path, dest_log->path_len);
            }
        }
        fwrite(msg->message, 1, message_size, file);
        fputc('\n', file);
    }
}
