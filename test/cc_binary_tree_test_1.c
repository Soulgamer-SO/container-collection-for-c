#include "cc_binary_tree.h"
#include "cc_common.h"
#include "cc_list.h"
#include <assert.h>

int main() {
	struct cc_binary_tree *tree;
	struct cc_binary_tree_iter iter;
	struct cc_list *queue;
	size_t *tmp, index;

	tree = cc_binary_tree_new();
	assert(tree != NULL);

	queue = cc_list_new();
	assert(queue != NULL);

	assert(!cc_binary_node_insert_left(&tree->root, (void **)1));
	assert(!cc_binary_node_insert_left(&tree->root, (void **)2));
	assert(!cc_binary_node_insert_left(&tree->root, (void **)3));
	assert(!cc_binary_node_insert_left(&tree->root, (void **)4));
	assert(!cc_binary_node_insert_right(&tree->root, (void **)5));
	assert(!cc_binary_node_insert_right(&tree->root, (void **)6));
	assert(!cc_binary_node_insert_right(&tree->root, (void **)7));

	assert(tree->root.left->data == (void *)4);
	assert(tree->root.left->left->data == (void *)3);
	assert(tree->root.left->left->left->data == (void *)2);
	assert(tree->root.left->left->left->left->data == (void *)1);
	assert(tree->root.right->data == (void *)7);
	assert(tree->root.right->right->data == (void *)6);
	assert(tree->root.right->right->right->data == (void *)5);

	assert(!cc_binary_node_print(&tree->root, 0));

	assert(!cc_binary_tree_iter_init(&iter, tree, queue));
	while (!cc_iter_next(&iter, &tmp, &index))
		cc_debug_print("(%u) %d, ", index, *tmp);
	cc_debug_print("\n");

	assert(!cc_binary_node_rotate_left(&tree->root.right));
	assert(!cc_binary_node_rotate_left(&tree->root.right));
	assert(cc_binary_node_rotate_left(&tree->root.right));

	assert(!cc_binary_node_rotate_right(&tree->root.left));
	assert(!cc_binary_node_rotate_right(&tree->root.left));
	assert(!cc_binary_node_rotate_right(&tree->root.left));
	assert(cc_binary_node_rotate_right(&tree->root.left));

	assert(tree->root.left->data == (void *)1);
	assert(tree->root.left->right->data == (void *)2);
	assert(tree->root.left->right->right->data == (void *)3);
	assert(tree->root.left->right->right->right->data == (void *)4);
	assert(tree->root.right->data == (void *)5);
	assert(tree->root.right->left->data == (void *)6);
	assert(tree->root.right->left->left->data == (void *)7);

	assert(!cc_binary_node_print(&tree->root, 0));

	assert(!cc_binary_tree_iter_init(&iter, tree, queue));
	while (!cc_iter_next(&iter, &tmp, &index))
		cc_debug_print("(%u) %d, ", index, *tmp);
	cc_debug_print("\n");

	assert(!cc_list_delete(queue));
	assert(!cc_binary_tree_delete(tree));

	return 0;
}