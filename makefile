src = $(wildcard src/*.c)
cflags = -O2 -lm

all: build run
build:
	gcc $(src) -o main $(cflags)
run:
	./main