SWLIB=./deps/swlib

run: main.c
	gcc -Wall main.c ${SWLIB}/vector.c -I${SWLIB} -lcurses -o main
	./a.out
