/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */
 
 #ifndef EMLOG_BACKEND_H
#define EMLOG_BACKEND_H
#pragma once

#include <stdbool.h>
#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

bool Em_CreateIntermediateDirectories(const char *path);
bool Em_FileExistsFile(const char *path);

/* Outputs the last access time for the given path. out0 is the high-part,
 * out1 is the low-part.
 */
bool Em_FileGetLastAccessTime(const char *path,
    unsigned *out0,
    unsigned *out1);

size_t Em_GetFileSize(const char *path);

struct Em_Pipe;
struct Em_Pipe *Em_CreatePipe();
void Em_DestroyPipe(struct Em_Pipe *p);
void Em_PipeRead(struct Em_Pipe *p, void *dest, size_t len);
void Em_PipeWrite(struct Em_Pipe *p, const void *from, size_t len);

#ifdef __cplusplus
} // extern "C"
#endif

#endif /* EMLOG_BACKEND_H */
