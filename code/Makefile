all:
	gcc -Wall -Wextra  src/main.c src/shared/shared.c src/extension/zoom.c  src/minimal/load_save.c src/minimal/image_processing.c  src/minimal/select.c -o main -L./lib -I./include -lSDL2-2.0 -lSDL2_image


run:
	LIBGL_DEBUG=verbose LD_LIBRARY_PATH=./lib ./main

clean:
	rm -f ./main
