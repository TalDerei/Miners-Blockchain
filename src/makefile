prog: main.o merkleTree.o readFile.o sha256.o hash.o sort.o block.o
	gcc -g -o prog merkleTree.o readFile.o sha256.o hash.o block.o sort.o main.o

sort.o: sort.h sort.c
	gcc -g -Wall -Wextra -Wwrite-strings -c sort.c

readFile.o: readFile.h readFile.c
	gcc -g -Wall -Wextra -Wwrite-strings -c readFile.c

merkleTree.o: merkleTree.h sha256.h merkleTree.c 
	gcc -g -Wall -Wextra -Wwrite-strings -c merkleTree.c

sha256.o: sha256.h sha256.c
	gcc -g -Wall -Wextra -Wwrite-strings -c sha256.c

hash.o: hash.h sha256.h hash.c
	gcc -g -Wall -Wextra -Wwrite-strings -c hash.c

block.o: block.h merkleTree.h sha256.h block.c
	gcc -g -Wall -Wextra -Wwrite-strings -c block.c

clean:
	rm -f prog
	rm -f *.o
