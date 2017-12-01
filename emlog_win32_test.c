/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */
 
#include <Windows.h>

#include <assert.h>
#include <string.h>
#include <stdio.h>

/*
void Em_PipeRead(struct Em_Pipe *p, void *dest, size_t len){
    ReadFile(p->r, dest, len, NULL, NULL);
}

void Em_PipeWrite(struct Em_Pipe *p, const void *from, size_t len){
    WriteFile(p->w, from, len, NULL, NULL);
}
*/
int main(int argc, char **argv){
    HANDLE r, w;
    STARTUPINFO start_info;
    PROCESS_INFORMATION proc_info;
    
    {
        SECURITY_ATTRIBUTES attributes;
        attributes.nLength = sizeof(SECURITY_ATTRIBUTES);
        attributes.bInheritHandle = TRUE;
        attributes.lpSecurityDescriptor = NULL;
        if(!CreatePipe(&r, &w, &attributes, 0)){
            printf("CreatePipe Error: %i\n", GetLastError());
            return EXIT_FAILURE;
        }
    }
    
    memset(&start_info, 0, sizeof(STARTUPINFO));
    start_info.cb = sizeof(STARTUPINFO);
    start_info.dwFlags = STARTF_USESTDHANDLES;
    start_info.hStdInput = r;
    start_info.hStdOutput = w;
    start_info.hStdError = w;
    
    if(!CreateProcess("emlog.exe",
        NULL,
        NULL,
        NULL,
        TRUE,
        0,
        NULL,
        NULL,
        &start_info,
        &proc_info)){
        printf("CreateProcess Error: %i\n", GetLastError());
        return EXIT_FAILURE;
    }
    
forever:
    {
        size_t buffer_size = 80, at = 0;
        char *buffer = malloc(buffer_size);
        int c;
        
        do{
            c = getchar();
            /* Do at + 1 so that we always have room for the NULL on the end
             * without doing another realloc */
            if(at >= buffer_size){
                assert(at == buffer_size);
                buffer_size <<= 1;
                buffer = realloc(buffer, buffer_size);
            }
            
            buffer[at++] = c;
        }while(c != '\n' && c != EOF);
        
        WriteFile(w, buffer, at, NULL, NULL);
        
        free(buffer);
    }
    goto forever;
}
