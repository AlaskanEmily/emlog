/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */
 
 #include "emlog_backend.h"

#include <Windows.h>
#include <stdlib.h>

bool Em_CreateIntermediateDirectories(const char *path){
    char buffer[MAX_PATH];
    unsigned at = 0;
    while(at < MAX_PATH && (buffer[at] = path[at]) != 0){
        if(buffer[at] == '/' || buffer[at] == '\\'){
            buffer[at] = 0;
            _mkdir(buffer);
            buffer[at] = '/';
        }
        at++;
    }
    
    if(!Em_FileExistsFile(path))
        _mkdir(path);
    
    return true;
}

bool Em_FileExistsFile(const char *path){
    WIN32_FIND_DATA data;
    const HANDLE h = FindFirstFile(path, &data);
    if(h == INVALID_HANDLE_VALUE)
        return false;
    
    FindClose(h);
    
    return data.dwFileAttributes == FILE_ATTRIBUTE_NORMAL ||
        data.dwFileAttributes == FILE_ATTRIBUTE_READONLY;
}

bool Em_FileGetLastAccessTime(const char *path,
    unsigned *out0,
    unsigned *out1){
    
    WIN32_FIND_DATA data;
    const HANDLE h = FindFirstFile(path, &data);
    if(h == INVALID_HANDLE_VALUE)
        return false;
    
    FindClose(h);
    
    out0[0] = data.ftLastWriteTime.dwHighDateTime;
    out1[0] = data.ftLastWriteTime.dwLowDateTime;
    
    return true;
}

size_t Em_GetFileSize(const char *path){
    
    WIN32_FIND_DATA data;
    const HANDLE h = FindFirstFile(path, &data);
    if(h == INVALID_HANDLE_VALUE)
        return 0;
    
    FindClose(h);
    
    /* TODO? High component? */
    return data.nFileSizeLow;
}

struct Em_Pipe{
    HANDLE r, w;
};

struct Em_Pipe *Em_CreatePipe(){
    HANDLE r, w;
    if(CreatePipe(&r, &w, NULL, 1024)){    
        struct Em_Pipe *p = malloc(sizeof(struct Em_Pipe));
        p->r = r;
        p->w = w;
    }
    else{
        return NULL;
    }
}

void Em_DestroyPipe(struct Em_Pipe *p){
    CloseHandle(p->r);
    CloseHandle(p->w);
    free(p);
}

void Em_PipeRead(struct Em_Pipe *p, void *dest, size_t len){
    ReadFile(p->r, dest, len, NULL, NULL);
}

void Em_PipeWrite(struct Em_Pipe *p, const void *from, size_t len){
    WriteFile(p->w, from, len, NULL, NULL);
}
