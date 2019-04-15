all:
	gcc -Wall -Wextra src/main.c src/SHARED/shared.c src/CLI/cli.c -o main -I./include -lSDL2-2.0

run:
	LIBGL_DEBUG=verbose LD_LIBRARY_PATH=./lib ./main

clean:
	rm -f ./main
