#ifndef AVLTREE_HELPERS
#define AVLTREE_HELPERS

#include "avltree.h"

/*
	Single rotations
 */

AvlTreeNode* rightRotation(AvlTreeNode* root) {
	printf("Rotate right\n");

	AvlTreeNode* child = root->left;

	root->left = child->right;
	if(child->right)	child->right->parent = root;

	child->right = root;
	child->parent = root->parent;

	root->parent = child;

	root->height = 1 + MAX(avlTreeGetHeight(root->left), avlTreeGetHeight(root->right));

	return child;
}

AvlTreeNode* leftRotation(AvlTreeNode* root) {
	printf("Rotate left\n");

	AvlTreeNode* child = root->right;

	root->right = child->left;
	if(child->left)	child->left->parent = root;

	child->left = root;
	child->parent = root->parent;

	root->parent = child;

	root->height = 1 + MAX(avlTreeGetHeight(root->left), avlTreeGetHeight(root->right));

	return child;
}

/*
	Double rotations
 */

AvlTreeNode* leftRightRotation(AvlTreeNode* root) {
	// left rotation
	root->left = leftRotation(root->left);
	// right rotation
	return rightRotation(root);
}

AvlTreeNode* rightLeftRotation(AvlTreeNode* root) {
	// right rotation
	root->right = rightRotation(root->right);
	// left rotation
	return leftRotation(root);
}

/*
	Format string for branch
 */
void generateBranch(char* str, int level, char direction) {
	int indentSize = 8;
	for(int i = 0; i < level - 1; i++) {
		memset(str + i * indentSize, ' ', 8);
	}
	if(level > 0) {
		sprintf(str + (level - 1) * indentSize, "   %c--- ", direction);
	}
}

#endif