/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef EMLOG_H
#define EMLOG_H
#pragma once

#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

struct Em_Message;

struct Em_Log;
struct Em_Context{
    struct Em_Log *logs;
    size_t num_logs, cap_logs;
};

void Em_CreateContext(struct Em_Context *ctx);
void Em_DestroyContext(struct Em_Context *ctx);

void Em_HandleMessage(struct Em_Context *ctx, const struct Em_Message *msg);

#ifdef __cplusplus
} // extern "C"
#endif

#endif /* EMLOG_EMLOG_H */
