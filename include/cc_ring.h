#ifndef __CC_RING_H
#define __CC_RING_H

#include "cc_array.h"
#include "cc_common.h"
#include <stddef.h>

struct cc_ring {
	struct cc_array *data;
	size_t read_index;
	size_t write_index;
};

#ifndef NO_MALLOC
struct cc_ring *cc_ring_new(size_t elem_nums, size_t elem_size);
void cc_ring_delete(struct cc_ring *self, cc_cleanup_fn fn);
#endif

void cc_ring_init(struct cc_ring *self, struct cc_array *data);
size_t cc_ring_space(struct cc_ring *self);

int cc_ring_append(struct cc_ring *self, void *item);
int cc_ring_shift(struct cc_ring *self, void *item);

#endif
