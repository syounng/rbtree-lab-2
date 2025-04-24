.PHONY: test

CFLAGS=-I ../src -Wall -g -DSENTINEL

test: test-rbtree
	./test-rbtree
	valgrind ./test-rbtree

test-rbtree: test-rbtree.o ../src/rbtree.o ../src/a.o

../src/rbtree.o:
	$(MAKE) -C ../src rbtree.o 
	
../src/a.o:
	$(MAKE) -C ../src a.o

clean:
	rm -f test-rbtree *.o
