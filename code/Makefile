CC=gcc
CFLAGS=-W -Wall -Wextra
LDFLAGS=`sdl2-config --cflags --libs`
EXEC=cimp

all: $(EXEC)

cimp: cimp.o shared.o cli.o parsing.o	
	gcc $(CFLAGS) -o $@ $^ $(LDFLAGS)

cimp.o: src/cimp.c src/SHARED/shared.h src/CLI/cli.h src/CLI/parsing.h
	gcc $(CFLAGS) -o  $@ -c $< $(LDFLAGS)
shared.o: src/SHARED/shared.c src/SHARED/shared.h
	gcc $(CFLAGS) -o  $@ -c $<   $(LDFLAGS)
cli.o: src/CLI/cli.c src/CLI/cli.h src/CLI/parsing.h
	gcc $(CFLAGS) -o  $@ -c $<  
parsing.o: src/CLI/parsing.c src/CLI/cli.h src/CLI/parsing.h
	gcc $(CFLAGS) -o  $@ -c $< 

clean:
	rm -rf *.o
mrproper: clean
	rm -rf $(EXEC)