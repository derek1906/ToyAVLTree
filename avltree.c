#include "avltree.h"
#include "avltree_helpers.h"

/*
	Get node height.
 */
int avlTreeGetHeight(AvlTreeNode* node) {
	if(node == NULL)	return 0;
	return node->height;
}

/*
	Get node balance.
	Negative - left heavy
	Position - right heavy
 */
int avlTreeBalance(AvlTreeNode* node) {
	if(node == NULL)	return 0;
	return avlTreeGetHeight(node->right) - avlTreeGetHeight(node->left);
}

/*
	Get node successor. Returns NULL if no successor
 */
AvlTreeNode* avlTreeGetNext(AvlTreeNode* node) {
	if(node->right != NULL) {
		// somewhere in right subtree
		AvlTreeNode* rtn = node->right;
		while(rtn->left)	rtn = rtn->left;
		return rtn;
	} else {
		// travel up
		AvlTreeNode* rtn = node;
		while(1) {
			if(rtn->parent == NULL)     	return NULL;
			if(rtn->parent->left == rtn)	return rtn->parent;
			rtn = rtn->parent;
		}
	}
}

/*
	Insert a node.
 */
void avlTreeInsert(AvlTreeNode** root, AvlTreeNode* newNode) {
	if(*root == NULL) {
		*root = newNode;
		newNode->height = 1;
		return;
	}

	int rootKey = (*root)->key;
	int nodeKey = newNode->key;

	// insert node
	if(nodeKey < rootKey)	avlTreeInsert(&(*root)->left, newNode);
	else                 	avlTreeInsert(&(*root)->right, newNode);
	(*root)->height = 1 + MAX(avlTreeGetHeight((*root)->left), avlTreeGetHeight((*root)->right));

	// check balance
	if(abs(avlTreeBalance(*root)) > 1) {
		// rebalance
		*root = avlTreeRebalance(*root);
	}

	// update parent pointers
	(*root)->parent = NULL;
	if((*root)->left) 	(*root)->left->parent = *root;
	if((*root)->right)	(*root)->right->parent = *root;
}

/*
	Delete a tree node. Private method.
 */
AvlTreeNode* _avlTreeDelete(AvlTreeNode** root) {
	AvlTreeNode* node = *root;
	AvlTreeNode* parent = node->parent;

	if(node->left == NULL && node->right == NULL) {
		// 0 children
		*root = NULL;
		return node;
	} else if(node->left != NULL && node->right != NULL) {
		// 2 children

		// get successor
		AvlTreeNode* next = node->right;
		AvlTreeNode** nextAddress = &node->right;
		while(next->left) {
			next = next->left;
			nextAddress = &next->left;
		}

		node->key = next->key;             	// replace content
		return _avlTreeDelete(nextAddress);	// delete extra node

	} else {
		// 1 child

		// get child
		AvlTreeNode* childNode = node->left ? node->left : node->right;
		AvlTreeNode** childNodeAddr = node->left ? &node->left : &node->right;

		node->key = childNode->key;          	// replace content
		return _avlTreeDelete(childNodeAddr);	// delete extra node
	}
}

/*
	Remove a node and rebalance tree.
 */
AvlTreeNode** originalRoot = NULL;
AvlTreeNode* avlTreeRemove(AvlTreeNode** root, int key) {
	if(*root == NULL)	return NULL;

	int rootKey = (*root)->key;
	if(originalRoot == NULL)	originalRoot = root;

	// recurse
	if(key < rootKey)	return avlTreeRemove(&(*root)->left, key);
	if(key > rootKey)	return avlTreeRemove(&(*root)->right, key);
	
	AvlTreeNode* deletedLeaf = _avlTreeDelete(root);
	AvlTreeNode* node = deletedLeaf->parent;
	// rebalance ancestors
	while(node) {
		node->height = 1 + MAX(avlTreeGetHeight(node->left), avlTreeGetHeight(node->right));

		AvlTreeNode** parentAddr = node->parent
			? (node->parent->left == node ? &node->parent->left : &node->parent->right)
			: originalRoot;
		AvlTreeNode* newRoot = avlTreeRebalance(node);
		*parentAddr = newRoot;

		node = node->parent;

		// print tree
		avlTreePrint(*originalRoot);
	}

	originalRoot = NULL;

	return deletedLeaf;
}

/*
	Rebalance node.
 */
AvlTreeNode* avlTreeRebalance(AvlTreeNode* node) {
	printf("Checking balance of node %d:(%d|%d)\n", node->key, avlTreeBalance(node), node->parent ? node->parent->key : -1);

	int balance = avlTreeBalance(node);
	if(balance < -1) {
		int childBalance = avlTreeBalance(node->left);
		if(childBalance > 0)	return leftRightRotation(node);
		else                	return rightRotation(node);
	} else if(balance > 1) {
		int childBalance = avlTreeBalance(node->right);
		if(childBalance < 0)	return rightLeftRotation(node);
		else                	return leftRotation(node);
	} else {
		return node;
	}
}

/*
	Print Tree.
 */
void avlTreePrint(AvlTreeNode* node) {
	printf("\n");
	_avlTreePrint(node, 0, ' ');
	printf("\n");
}

/*
	Recursively print tree nodes. Private method.
 */
void _avlTreePrint(AvlTreeNode* node, int level, char direction) {
	char pad[level * 8 + 1];
	pad[level * 8] = 0;
	generateBranch(pad, level, direction);

	if(node == NULL) {
		return;
	}else{
		_avlTreePrint(node->right, level + 1, '/');
		printf("%s%d (h:%d|p:%d)\n", pad, node->key, node->height, node->parent ? node->parent->key : -1);
		_avlTreePrint(node->left, level + 1, '\\');
	}
}