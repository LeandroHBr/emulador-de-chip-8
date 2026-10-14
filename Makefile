INCLUDES = -I ./include
FLAGS = -g

OBJECTS = ./build/memory.o ./build/chip8stack.o
all: ${OBJECTS}
	gcc ${FLAGS} ${INCLUDES} ./src/main.c ${OBJECTS} -L ./lib -lmingw32 -lSDL3 -o ./bin/main

./build/memory.o: ./src/memory.c
	gcc ${FLAGS} ${INCLUDES} ./src/memory.c -c -o ./build/memory.o

./build/memory.o: ./src/chip8stack.c
	gcc ${FLAGS} ${INCLUDES} ./src/chip8stack.c -c -o ./build/chip8stack.o


clean:
	del build\*