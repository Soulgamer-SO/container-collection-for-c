#include "cc_hash_map.h"
#include "cc_common.h"
#include "cc_list_map.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

/// Get the slot (whose type is `struct cc_list_map **`) by key.
static inline int get_list_map(struct cc_hash_map *self, void *key, struct cc_list_map ***result) {
	size_t hash_tmp;
	hash_tmp = self->calc_hash(key) % self->bucket_size;
	return cc_array_get_ref(self->data, hash_tmp, (void **)result);
}

int cc_hash_map_get_item(struct cc_hash_map *self, void *key, struct cc_map_item **result) {
	struct cc_list_map **list_map_tmp;

	if (!check_and_reset_double_p(result))
		return 0;

	assert(get_list_map(self, key, &list_map_tmp));
	if (*list_map_tmp != NULL)
		return cc_list_map_get_item(*list_map_tmp, key, result, NULL);
	else
		return 0;
}

int cc_hash_map_get(struct cc_hash_map *self, void *key, void **result) {
	struct cc_map_item *tmp;

	if (!check_and_reset_double_p(result))
		return 0;

	if (!cc_hash_map_get_item(self, key, &tmp))
		return 0;

	*result = tmp->value;
	return 1;
}

int cc_hash_map_set(struct cc_hash_map *self, void *key, void *value) {
	struct cc_map_item *tmp;
	struct cc_list_map **list_map_tmp;

	/// Try to find existing map item for `key`.

	if (cc_hash_map_get_item(self, key, &tmp)) {
		tmp->value = value;
		return 1;
	}

	/// No existing item for `key`.

	assert(get_list_map(self, key, &list_map_tmp));
	if (*list_map_tmp == NULL)
		*list_map_tmp = cc_list_map_new(self->cmp);

	return cc_list_map_set(*list_map_tmp, key, value);
}

int cc_hash_map_del(struct cc_hash_map *self, void *key, void **result) {
	struct cc_list_map **list_map_tmp;
	size_t hash_tmp;

	if (!check_and_reset_double_p(result))
		return 0;

	assert(get_list_map(self, key, &list_map_tmp));
	if (*list_map_tmp != NULL)
		return cc_list_map_del(*list_map_tmp, key, result);
	else
		return 0;
}

void cc_hash_map_print_slot(struct cc_list_map *slot, int index) {
	cc_debug_print("[% 9d] ", index);
	if (slot != NULL)
		cc_list_map_print(slot, "\n");
	else
		cc_debug_print("\n");
}

void cc_hash_map_print(struct cc_hash_map *self, char *end_string) {
	struct cc_array_iter iter;
	struct cc_list_map **tmp;
	int i = 0;

	cc_array_iter_init(&iter, self->data);
	while (cc_iter_next(&iter, &tmp, NULL))
		cc_hash_map_print_slot(*tmp, i++);

	cc_debug_print("%s", end_string);
}

static const struct cc_map_i map_interface = {
	.get = (cc_map_get_fn)cc_hash_map_get,
	.set = (cc_map_set_fn)cc_hash_map_set,
	.del = (cc_map_del_fn)cc_hash_map_del,
};

struct cc_hash_map *cc_hash_map_new(size_t bucket_size, cc_cmp_fn cmp, cc_hash_fn calc_hash) {
	struct cc_hash_map *self;
	struct cc_array_iter iter;
	struct cc_list_map **tmp;

	if (bucket_size == 0)
		return NULL;

	self = malloc(sizeof(*self));
	if (self == NULL)
		return NULL;

	self->data = cc_array_new(bucket_size, sizeof(struct cc_list_map *));
	if (self->data == NULL)
		return NULL;

	self->bucket_size = bucket_size;

	/// The elements of the array should be initialized as NULLs.
	cc_array_iter_init(&iter, self->data);
	while (cc_iter_next(&iter, &tmp, NULL))
		*tmp = NULL;

	self->cmp = CC_WITH_DEFAULT(cmp, cc_default_cmp_fn);
	self->calc_hash = CC_WITH_DEFAULT(calc_hash, cc_default_hash_fn);

	self->interface = (struct cc_map_i *)&map_interface;

	return self;
}

void cc_hash_map_delete(struct cc_hash_map *self) {
	struct cc_array_iter iter;
	struct cc_list_map **tmp;

	cc_array_iter_init(&iter, self->data);
	while (cc_iter_next(&iter, &tmp, NULL)) {
		if (*tmp != NULL)
			cc_list_map_delete(*tmp);
	}

	cc_array_delete(self->data);
	free(self);
}
