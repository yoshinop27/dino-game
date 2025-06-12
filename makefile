CC := clang
CFLAGS := -g $(shell pkg-config sdl2 SDL2_image --cflags)
LDFLAGS := $(shell pkg-config sdl2 SDL2_image --libs)

game: main.o sdl_init.o cactus.o
	$(CC) -o game main.o sdl_init.o cactus.o $(LDFLAGS)

main.o: main.c sdl_init.h cactus.h
	$(CC) $(CFLAGS) -c main.c -o main.o

sdl_init.o: sdl_init.c sdl_init.h
	$(CC) $(CFLAGS) -c sdl_init.c -o sdl_init.o

cactus.o: cactus.h cactus.c
	$(CC) $(CFLAGS) -c cactus.c -o cactus.o