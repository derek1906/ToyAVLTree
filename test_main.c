#include <stdio.h>
#include <unistd.h>
#include <assert.h>
#include "avltree.h"

AvlTreeNode* createNode(int key) {
	AvlTreeNode* node = calloc(sizeof(AvlTreeNode), 1);
	node->key = key;
	return node;
}

void freeNodes(AvlTreeNode** node) {
	if(*node == NULL)	return;

	freeNodes(&(*node)->left);
	freeNodes(&(*node)->right);
	free(*node);
	*node = NULL;
}

int compareTree(AvlTreeNode* root1, AvlTreeNode* root2) {
	if(root1 == NULL || root2 == NULL) {
		if(root1 == NULL && root2 == NULL)	return 1;
		else                              	return 0;
	}

	if(root1->key != root2->key)	return 0;

	return compareTree(root1->left, root2->left) && compareTree(root1->right, root2->right);
}

void makeChild(AvlTreeNode* parent, AvlTreeNode* leftChild, AvlTreeNode* rightChild) {
	parent->left = leftChild;
	parent->right = rightChild;
	if(leftChild) 	leftChild->parent = parent;
	if(rightChild)	rightChild->parent = parent;
}

void testSimpleInsert(AvlTree* tree) {
	avlTreeInsert(&tree->root, createNode(1));
	assert(tree->root && tree->root->key == 1);

	avlTreeInsert(&tree->root, createNode(2));
	avlTreeInsert(&tree->root, createNode(3));
	assert(tree->root && tree->root->key == 2);
	assert(tree->root->left && tree->root->left->key == 1);
	assert(tree->root->right && tree->root->right->key == 3);

	freeNodes(&tree->root);
}

void testInsertTen(AvlTree* tree) {
	AvlTreeNode* nodes = calloc(sizeof(AvlTreeNode), 11);
	for(int i = 0; i <= 10; i++)	nodes[i].key = i;

	makeChild(&nodes[4], &nodes[2], &nodes[8]);
	makeChild(&nodes[2], &nodes[1], &nodes[3]);
	makeChild(&nodes[8], &nodes[6], &nodes[9]);
	makeChild(&nodes[6], &nodes[5], &nodes[7]);
	makeChild(&nodes[9], NULL, &nodes[10]);

	AvlTreeNode* root = &nodes[4];

	for(int i = 1; i <= 10; i++) {
		avlTreeInsert(&tree->root, createNode(i));
	}
	assert(compareTree(tree->root, root));

	free(nodes);
	tree->root = NULL;
}

void testDelete(AvlTree* tree) {
	AvlTreeNode* nodes = calloc(sizeof(AvlTreeNode), 11);
	for(int i = 0; i <= 10; i++)	nodes[i].key = i;

	makeChild(&nodes[8], &nodes[4], &nodes[9]);
	makeChild(&nodes[4], &nodes[2], &nodes[6]);
	makeChild(&nodes[6], &nodes[5], &nodes[7]);
	makeChild(&nodes[9], NULL, &nodes[10]);

	AvlTreeNode* root = &nodes[8];

	for(int i = 1; i <= 10; i++) {
		avlTreeInsert(&tree->root, createNode(i));
	}

	avlTreeRemove(&tree->root, 1);
	avlTreeRemove(&tree->root, 3);
	assert(compareTree(tree->root, root));

	free(nodes);
	tree->root = NULL;
}

int main(int argc, char** argv) {
	AvlTree tree;
	memset(&tree, 0, sizeof(AvlTree));

	// assert tree empty
	assert(tree.root == NULL);

	testSimpleInsert(&tree);
	testInsertTen(&tree);
	testDelete(&tree);

	// free nodes
	freeNodes(&tree.root);
}