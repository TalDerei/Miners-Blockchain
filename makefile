prog: merkleTree.o readFile.o sha_256.o
	gcc -g -o prog merkleTree.o readFile.o sha_256.o

sha_256.o: merkleTree.h node.h sha_256.h sha_256.c
	gcc -g -Wall -Wextra -Write-strings -c  sha_256.c

readFile.o: merkleTree.h node.h sha_256.h merkleTree.c
	gcc -g -Wall -Wextra -Write-strings -c readFile.c

merkleTree.o: merkleTree.h node.h sha_256.h merkleTree.c
	gcc -g -Wall -Wextra -Write-strings -c merlkleTree.c

clean:
	rm -f prog
	rm -f *.o
