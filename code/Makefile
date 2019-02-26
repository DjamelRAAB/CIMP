all:
	gcc -Wall -Wextra -L./lib -I./include -lSDL2-2.0 src/main.c -o main

run:
	LIBGL_DEBUG=verbose LD_LIBRARY_PATH=./lib ./main

clean:
	rm -f ./main
