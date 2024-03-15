#ifndef __CC_LIST_MAP_H
#define __CC_LIST_MAP_H

#ifdef NO_MALLOC
#error "You can NOT use list map without support for the `malloc` function."
#endif

#include "cc_common.h"
#include "cc_list.h"
#include "cc_map.h"

struct cc_list_map {
	struct cc_map_i *interface;
	struct cc_list *data;
};

struct cc_list_map *cc_list_map_new();
void cc_list_map_delete(struct cc_list_map *self, cc_cleanup_fn fn);

int cc_list_map_get(struct cc_list_map *self, void *key, void *value);
int cc_list_map_set(struct cc_list_map *self, void *key, void *value);

#endif
