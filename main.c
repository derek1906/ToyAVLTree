#include <stdio.h>
#include <unistd.h>
#include "avltree.h"

#define PROMPT_MSG "Enter key ('q' to quit): "
#define INVALID_COMMAND "\
Invalid command. Pick from\n\
\n\
'q'         	- quit\n\
'p'         	- print tree\n\
'l [num:10]'	- insert range from 1 to num inclusive\n\
'i <key>'   	- insert 'key'\n\
'd <key>'   	- delete 'key'\n\
'r'         	- reset tree\n\
"

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

int main(int argc, char** argv) {
	AvlTree* tree = calloc(sizeof(AvlTree), 1);

	char* input = NULL;
	size_t size = 0;
	char quit = 0;
	while(1) {
		write(1, PROMPT_MSG, strlen(PROMPT_MSG));
		ssize_t read = getline(&input, &size, stdin);
		if(read == -1)	break;

		char command;
		int key;
		size_t num = sscanf(input, "%c %d", &command, &key);
		switch(command) {
			case 'q':
				// quit
				quit = 1; break;
			case 'i':
				// insert a single key
				if(num < 2){
					printf("%s\n", INVALID_COMMAND);
					break;
				}
				avlTreeInsert(&tree->root, createNode(key));
				avlTreePrint(tree->root);
				break;
			case 'd': {
				// delete a single key
				if(num < 2){
					printf("%s\n", INVALID_COMMAND);
					break;
				}
				AvlTreeNode* deletedNode = avlTreeRemove(&tree->root, key);
				printf("Result");
				avlTreePrint(tree->root);
				free(deletedNode);
				break;
			}
			case 'l':
				// insert a range of keys
				if(num < 2)	key = 10;
				for(int i = 1; i <= key; i++) {
					avlTreeInsert(&tree->root, createNode(i));
				}
				avlTreePrint(tree->root);
				break;
			case 'p':
				// print tree
				avlTreePrint(tree->root);
				break;
			case 'r':
				// clear tree
				freeNodes(&tree->root);
				avlTreePrint(tree->root);
				break;
			default:
				printf("%s\n", INVALID_COMMAND);
				break;
		}

		if(quit)	break;
	}
	free(input);

	// free nodes
	freeNodes(&tree->root);
	// free tree
	free(tree);
	
	return 0;
}