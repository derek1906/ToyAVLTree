all: interactive test

interactive: main.c avltree.c
	gcc -g main.c avltree.c -o avl

test: test_main.c avltree.c
	gcc -g test_main.c avltree.c -o avl_test

clean:
	rm -f avl avl_test *.o