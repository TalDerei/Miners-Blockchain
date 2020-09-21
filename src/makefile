prog: main.o merkleTree.o readFile.o 
	gcc -g -o prog merkleTree.o readFile.o main.o

readFile.o: merkleTree.h node.h merkleTree.c
	gcc -g -Wall -Wextra -Wwrite-strings -c readFile.c

merkleTree.o: merkleTree.h node.h merkleTree.c 
	gcc -g -Wall -Wextra -Wwrite-strings -c merkleTree.c

clean:
	rm -f prog
	rm -f *.o
