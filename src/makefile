prog: main.o merkleTree.o readFile.o sha256.o hash.o sort.o block.o printMerkleTree.o printBlock.o serialize.o validation.o indexStructure.o
	gcc -g -w -o prog merkleTree.o readFile.o sha256.o hash.o block.o sort.o printMerkleTree.o printBlock.o serialize.o validation.o indexStructure.o main.o

main.o: main.c
	gcc -g -w -Wall -Wextra -Wwrite-strings -c main.c

sort.o: sort.h sort.c
	gcc -g -w -Wall -Wextra -Wwrite-strings -c sort.c

readFile.o: readFile.h sort.h readFile.c
	gcc -g -w -Wall -Wextra -Wwrite-strings -c readFile.c

merkleTree.o: merkleTree.h sha256.h merkleTree.c
	gcc -g -w -Wall -Wextra -Wwrite-strings -c merkleTree.c

sha256.o: sha256.h sha256.c
	gcc -g -w -Wall -Wextra -Wwrite-strings -c sha256.c

hash.o: hash.h sha256.h hash.c
	gcc -g -w -Wall -Wextra -Wwrite-strings -c hash.c

block.o: block.h merkleTree.h sha256.h block.c
	gcc -g -w -Wall -Wextra -Wwrite-strings -c block.c

printMerkleTree.o: printMerkleTree.h printMerkleTree.c
	gcc -g -w -Wall -Wextra -Wwrite-strings -c printMerkleTree.c

printBlock.o: printBlock.h printBlock.c
	gcc -g -w -Wall -Wextra -Wwrite-strings -c printBlock.c

serialize.o: serialize.h serialize.c
	gcc -g -w -Wall -Wextra -Wwrite-strings -c serialize.c

validation.o: validation.h validation.c
	gcc -g -w -Wall -Wextra -Wwrite-strings -c validation.c

indexStructure.o: indexStructure.h indexStructure.c
	gcc -g -w -Wall -Wextra -Wwrite-strings -c indexStructure.c

clean:
	rm -f prog
	rm -f *.o
