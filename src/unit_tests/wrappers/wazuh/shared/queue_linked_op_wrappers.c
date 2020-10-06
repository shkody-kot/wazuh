/* Copyright (C) 2015-2020, Wazuh Inc.
 * All rights reserved.
 *
 * This program is free software; you can redistribute it
 * and/or modify it under the terms of the GNU General Public
 * License (version 2) as published by the FSF - Free Software
 * Foundation
 */

#include "queue_linked_op_wrappers.h"
#include <stddef.h>
#include <stdarg.h>
#include <setjmp.h>
#include <cmocka.h>

void __wrap_linked_queue_push_ex(w_linked_queue_t * queue, void * data) {
    check_expected_ptr(queue);
    check_expected(data);

    w_linked_queue_node_t *node;
    os_calloc(1, sizeof(w_linked_queue_node_t), node);
    node->data = data;
    node->next = NULL;
    if (!queue->first) {
        // First node, will be both first and last
        queue->first = node;
        queue->last = node;
        node->prev = NULL;
        node->next = NULL;
    } else {
        queue->last->next = node;
        node->prev = queue->last;
        node->next = NULL;
        queue->last = node;
    }
    queue->elements++;
}

void * __wrap_linked_queue_pop_ex(w_linked_queue_t * queue) {
    void * data = NULL;

    int retval = mock();

    check_expected_ptr(queue);

    if (retval != -1) {
        if (!queue->first) {
            data = NULL;
        } else {
            data = queue->last->data;
            w_linked_queue_node_t *tmp = queue->last;
            queue->last = queue->last->prev;
            if (queue->last) {
                queue->last->next = NULL;
            } else {
                // queue is now empty
                queue->first = NULL;
            }
            queue->elements--;
            os_free(tmp);
        }
    }

    return data;
}
