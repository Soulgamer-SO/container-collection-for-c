#ifndef __CC_LIST_H
#define __CC_LIST_H

#ifdef NO_MALLOC
#error "You can NOT use `cc_list` without support for the `malloc` function."
#endif

#include "cc_iter.h"

////////////////////////////////////////////////////////////////////////////////
/// The List Node
////////////////////////////////////////////////////////////////////////////////
struct cc_list_node {
	struct cc_list_node *prev;
	struct cc_list_node *next;
	union {
		/// For data nodes, we use the `data` to hold a value or a pointer to value.
		void *data;
		/// For root node, we use `size` to keep the number of elements in the list.
		size_t size;
	};
};

////////////////////////////////////////////////////////////////////////////////
/// The List Container
////////////////////////////////////////////////////////////////////////////////
struct cc_list {
	struct cc_list_node root;
};

int cc_list_new(struct cc_list **self);
int cc_list_delete(struct cc_list *self);

int cc_list_append(struct cc_list *self, void *value);
int cc_list_concat(struct cc_list *left, struct cc_list *right);
int cc_list_insert(struct cc_list *self, size_t index, void *value);
int cc_list_remove(struct cc_list *self, size_t index, void **result);

size_t cc_list_size(struct cc_list *self);

int cc_list_print(struct cc_list *self, int direction);

////////////////////////////////////////////////////////////////////////////////
/// The List Cursor
////////////////////////////////////////////////////////////////////////////////
enum {
	CC_LIST_CURSOR_MOVE_OUT_OF_RANGE = 0xFF10,
	CC_LIST_CURSOR_GET_OUT_OF_RANGE = 0xFF11,
};

struct cc_list_cursor {
	struct cc_list *list;
	struct cc_list_node *current;
};

int cc_list_cursor_new(struct cc_list_cursor **self, struct cc_list *list);
int cc_list_cursor_delete(struct cc_list_cursor *self);

int cc_list_cursor_get_n(struct cc_list_cursor *self, int offset, int count, void **result);
int cc_list_cursor_move_n(struct cc_list_cursor *self, int offset);

////////////////////////////////////////////////////////////////////////////////
/// The List Iterator
////////////////////////////////////////////////////////////////////////////////
struct cc_list_iter {
	struct cc_iter_i *iterator;
	struct cc_list *list;
	struct cc_list_node *cursor;
	size_t index;
	int direction;
};

int cc_list_iter_init(struct cc_list_iter *self, struct cc_list *list, int direction);
int cc_list_iter_next(struct cc_list_iter *self, void **item, size_t *index);

#endif
