#ifndef __CC_LIST_H
#define __CC_LIST_H

#include <stdint.h>

#ifdef NO_MALLOC
#error "You can NOT use linked list without support for malloc."
#endif

#include "cc_common.h"

///-----------------------------------------------------------------------------
/// List node for the doubly linked list
///-----------------------------------------------------------------------------
struct cc_list_node {
	struct cc_list_node *prev;
	struct cc_list_node *next;
	void *value;
};

struct cc_list_node *cc_list_node_new(void *value);
void cc_list_node_delete(struct cc_list_node *self, cc_handle_fn cleanup_fn);

void cc_list_node_init(struct cc_list_node *self, void *value);

///-----------------------------------------------------------------------------
/// Doubly linked list
///-----------------------------------------------------------------------------
struct cc_list {
	struct cc_list_node *head;
	struct cc_list_node *tail;
	size_t size;
};

struct cc_list *cc_list_new();
void cc_list_delete(struct cc_list *self, cc_handle_fn cleanup_fn);

void cc_list_init(struct cc_list *self);

int cc_list_add(struct cc_list *self, void *value);
int cc_list_concat(struct cc_list *left, struct cc_list *right);

///-----------------------------------------------------------------------------
/// The iterator for the doubly linked list
///-----------------------------------------------------------------------------
struct cc_list_iter {
	struct cc_iter_i *iterator;
	struct cc_list *data;
	struct cc_list_node *cursor;
	uint8_t direction;
};

int cc_list_iter_init(struct cc_list_iter *self, struct cc_list *list, uint8_t direction);

#endif