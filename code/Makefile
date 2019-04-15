all:
	gcc -Wall -Wextra src/CLI/cli.c src/CLI/parsing.c -o main -I./include 

run:
	LIBGL_DEBUG=verbose LD_LIBRARY_PATH=./lib ./main

clean:
	rm -f ./main
