CC=gcc
CFLAGS=-W -Wall -Wextra
LDFLAGS=`sdl2-config --cflags --libs` 
LIBS = $(shell sdl2-config --libs) -lSDL2_image
EXEC=cimp

all: $(EXEC)

cimp: cimp.o shared.o cli.o parsing.o openSaveImages.o windows.o select.o image_processing.o
	gcc $(CFLAGS) -o $@ $^ $(LDFLAGS) $(LIBS)

cimp.o: src/cimp.c src/shared/shared.h src/cli/cli.h src/cli/parsing.h include/defaultValues.h
	gcc $(CFLAGS) -o  $@ -c $< $(LDFLAGS)
shared.o: src/shared/shared.c src/shared/shared.h src/windows/windows.h
	gcc $(CFLAGS) -o  $@ -c $<   $(LDFLAGS)
cli.o: src/cli/cli.c src/cli/cli.h src/cli/parsing.h
	gcc $(CFLAGS) -o  $@ -c $<  
parsing.o: src/cli/parsing.c src/cli/cli.h src/cli/parsing.h
	gcc $(CFLAGS) -o  $@ -c $< 
openSaveImages.o: src/minimale/openSaveImages.c src/minimale/openSaveImages.h src/minimale/select.h
	gcc $(CFLAGS) -o  $@ -c $< 
select.o: src/minimale/select.c src/minimale/select.h src/minimale/image_processing.h
	gcc $(CFLAGS) -o  $@ -c $< 
image_processing.o: src/minimale/image_processing.c src/minimale/image_processing.h src/minimale/select.h
	gcc $(CFLAGS) -o  $@ -c $< 
windows.o: src/windows/windows.c src/windows/windows.h
	gcc $(CFLAGS) -o  $@ -c $< 

clean:
	-rm *.o $(EXE)
mrproper: clean
	rm -rf $(EXEC) 
