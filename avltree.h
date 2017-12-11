#ifndef AVLTREE
#define AVLTREE

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define MAX(X, Y) (((X) < (Y)) ? (Y) : (X))

typedef struct AvlTreeNode {
	int key;
	int height;
	struct AvlTreeNode* parent;
	struct AvlTreeNode* left;
	struct AvlTreeNode* right;
} AvlTreeNode;

typedef struct {
	struct AvlTreeNode* root;
} AvlTree;

int avlTreeGetHeight(AvlTreeNode* node);

int avlTreeBalance(AvlTreeNode* node);

void avlTreeInsert(AvlTreeNode** root, AvlTreeNode* newNode);

AvlTreeNode* avlTreeRemove(AvlTreeNode** root, int key);

AvlTreeNode* avlTreeRebalance(AvlTreeNode* node);

void avlTreePrint(AvlTreeNode* node);
void _avlTreePrint(AvlTreeNode* node, int level, char direction);

#endif