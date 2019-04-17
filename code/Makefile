CC=gcc
CFLAGS=-W -Wall -Wextra
LDFLAGS=`sdl2-config --cflags --libs`
EXEC=cimp

all: $(EXEC)

cimp: cimp.o shared.o cli.o parsing.o openSaveImages.o windows.o
	gcc $(CFLAGS) -o $@ $^ $(LDFLAGS)

cimp.o: src/cimp.c src/shared/shared.h src/cli/cli.h src/cli/parsing.h
	gcc $(CFLAGS) -o  $@ -c $< $(LDFLAGS)
shared.o: src/shared/shared.c src/shared/shared.h
	gcc $(CFLAGS) -o  $@ -c $<   $(LDFLAGS)
cli.o: src/cli/cli.c src/cli/cli.h src/cli/parsing.h
	gcc $(CFLAGS) -o  $@ -c $<  
parsing.o: src/cli/parsing.c src/cli/cli.h src/cli/parsing.h
	gcc $(CFLAGS) -o  $@ -c $< 
openSaveImages.o: src/minimale/openSaveImages.c src/minimale/openSaveImages.h
	gcc $(CFLAGS) -o  $@ -c $< 
windows.o: src/windows/windows.c src/windows/windows.h
	gcc $(CFLAGS) -o  $@ -c $< 

clean:
	rm -rf *.o
mrproper: clean
	rm -rf $(EXEC) 
