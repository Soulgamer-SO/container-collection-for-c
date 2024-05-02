#include "cc_list_map.h"
#include <stdio.h>
#include <stdlib.h>

static int cc_list_map_get_item(struct cc_list_map *self, void *key, struct cc_map_item **result, size_t *index)
{
	struct cc_list_iter iter;
	struct cc_map_item **item;

	if (try_reset_double_p(result))
		return 1;
	if (cc_list_iter_init(&iter, self->data, 0))
		return 2;
	while (!cc_iter_next(&iter, &item, index)) {
		if (self->cmp(key, (*item)->key) == 0) {
			*result = *item;
			return 0;
		}
	}

	return 3;
}

int cc_list_map_get(struct cc_list_map *self, void *key, void **result)
{
	struct cc_map_item *item;

	if (try_reset_double_p(result))
		return 1;
	if (cc_list_map_get_item(self, key, &item, NULL))
		return 2;

	*result = item->value;
	return 0;
}

int cc_list_map_insert_new(struct cc_list_map *self, void *key, void *value)
{
	struct cc_map_item *item;

	item = malloc(sizeof(*item));
	if (item == NULL)
		return 1;

	item->key = key;
	item->value = value;
	if (cc_list_insert(self->data, 0, item))
		goto fail;

	return 0;

fail:
	free(item);
	return 2;
}

int cc_list_map_set_new(struct cc_list_map *self, void *key, void *value)
{
	struct cc_map_item *item;

	if (!cc_list_map_get_item(self, key, &item, NULL))
		return 1;
	if (cc_list_map_insert_new(self, key, value))
		return 2;

	return 0;
}

int cc_list_map_set(struct cc_list_map *self, void *key, void *value)
{
	struct cc_map_item *item;

	if (!cc_list_map_get_item(self, key, &item, NULL)) {
		item->value = value;
		return 0;
	}

	if (cc_list_map_insert_new(self, key, value))
		return 1;

	return 0;
}

int cc_list_map_del(struct cc_list_map *self, void *key, void **result)
{
	struct cc_map_item *item;
	size_t index;

	if (try_reset_double_p(result))
		return 1;
	if (cc_list_map_get_item(self, key, &item, &index))
		return 2;
	if (cc_list_remove(self->data, index, (void **)&item))
		return 3;

	*result = item->value;
	free(item);
	return 0;
}

int cc_list_map_print(struct cc_list_map *self, char *end_string)
{
	struct cc_list_map_iter iter;
	struct cc_map_item *item;
	size_t index;

	if (cc_list_map_iter_init(&iter, self))
		return 1;
	while (!cc_iter_next(&iter, &item, &index))
		cc_debug_print("(%d){%zu -> %zu} ", index, item->key, item->value);

	cc_debug_print("%s", end_string);
	return 0;
}

static struct cc_map_i map_interface = {
	.get = (cc_map_get_fn_t)cc_list_map_get,
	.set = (cc_map_set_fn_t)cc_list_map_set,
	.set_new = (cc_map_set_fn_t)cc_list_map_set_new,
	.del = (cc_map_del_fn_t)cc_list_map_del,
};

struct cc_list_map *cc_list_map_new(cc_cmp_fn_t cmp)
{
	struct cc_list_map *self;

	self = malloc(sizeof(*self));
	if (self == NULL)
		goto fail1;

	self->interface = &map_interface;
	self->data = cc_list_new();
	if (self->data == NULL)
		goto fail2;

	self->cmp = CC_WITH_DEFAULT(cmp, cc_default_cmp_fn);
	return self;

fail2:
	free(self);
fail1:
	return NULL;
}

int cc_list_map_delete(struct cc_list_map *self)
{
	struct cc_list_iter iter;
	struct cc_map_item **item;

	if (cc_list_iter_init(&iter, self->data, 0))
		return 1;
	while (!cc_iter_next(&iter, &item, NULL))
		free(*item);

	if (cc_list_delete(self->data))
		return 2;

	free(self);
	return 0;
}

static struct cc_iter_i iterator_interface = {
	.next = (cc_iter_next_fn_t)cc_list_map_iter_next,
};

int cc_list_map_iter_next(struct cc_list_map_iter *self, void **item, size_t *index)
{
	struct cc_map_item **tmp_item;

	if (try_reset_double_p(item))
		return 1;
	if (cc_list_iter_next(&self->inner_iter, (void **)&tmp_item, index))
		return 2;

	*item = *tmp_item;
	return 0;
}

int cc_list_map_iter_init(struct cc_list_map_iter *self, struct cc_list_map *map)
{
	if (map == NULL)
		return 1;

	self->iterator = &iterator_interface;
	return cc_list_iter_init(&self->inner_iter, map->data, 0);
}
