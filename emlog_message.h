/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */
 
 #ifndef EMLOG_MESSAGE_H
#define EMLOG_MESSAGE_H
#pragma once

#ifdef __cplusplus
extern "C" {
#endif

struct Em_Message{
    /* The categories are a path to where the message will be logged.
     * It can be NULL for no categories */
    const char *categories;
    const char *message;
    void *buffer;
};

int Em_GetMessage(struct Em_Message *out);
void Em_FreeMessage(struct Em_Message *msg);

#ifdef __cplusplus
} // extern "C"
#endif

#endif /* EMLOG_MESSAGE_H */
