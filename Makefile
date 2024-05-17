SWLIB=../swlib

run: main.c
	gcc -Wall main.c ${SWLIB}/vector.c -I../swlib -lcurses
	./a.out
