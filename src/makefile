prog: main.o merkleTree.o readFile.o sort.o
	gcc -g -o prog merkleTree.o readFile.o sort.o main.o

sort.o: merkleTree.h merkleTree.c sort.c
	gcc -g -Wall -Wextra -Wwrite-strings -c sort.c

readFile.o: merkleTree.h node.h merkleTree.c
	gcc -g -Wall -Wextra -Wwrite-strings -c readFile.c

merkleTree.o: merkleTree.h node.h merkleTree.c 
	gcc -g -Wall -Wextra -Wwrite-strings -c merkleTree.c

clean:
	rm -f prog
	rm -f *.o
