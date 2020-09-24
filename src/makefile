run: main.o merkleTree.o readFile.o sort.o sha256.o hash.o
	gcc -g -o prog merkleTree.o readFile.o sort.o sha256.o hash.o main.o

sort.o: merkleTree.h sort.c
	gcc -g -Wall -Wextra -Wwrite-strings -c sort.c

readFile.o: merkleTree.h node.h sha256.h readFile.c
	gcc -g -Wall -Wextra -Wwrite-strings -c readFile.c

merkleTree.o: merkleTree.h node.h sha256.h merkleTree.c 
	gcc -g -Wall -Wextra -Wwrite-strings -c merkleTree.c

sha256.o: sha256.h sha256.c
	gcc -g -Wall -Wextra -Wwrite-strings -c sha256.c

hash.o: merkletree.h hash.c
	gcc -g -Wall -Wextra -Wwrite-strings -c hash.c

clean:
	rm -f prog
	rm -f *.o
