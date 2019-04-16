all:
	gcc -Wall -Wextra src/cimp.c src/CLI/cli.c src/CLI/parsing.c -o main -L./lib -I./include -lSDL2-2.0

run:
	LIBGL_DEBUG=verbose LD_LIBRARY_PATH=./lib ./main

clean:
	rm -f ./main
