SWLIB=./deps/swlib

run: src/main.c
	gcc -Wall src/main.c ${SWLIB}/vector.c -I${SWLIB} -lcurses -o ./bin/main
	./bin/main
