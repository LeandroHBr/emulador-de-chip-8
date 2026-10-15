INCLUDES = -I ./include
FLAGS = -g

OBJECTS = ./build/memory.o ./build/chip8stack.o ./build/keyboard.o ./build/chip8.o ./build/chip8screen.o
all: ${OBJECTS}
	gcc ${FLAGS} ${INCLUDES} ./src/main.c ${OBJECTS} -L ./lib -lmingw32 -lSDL3 -o ./bin/main

./build/memory.o: ./src/memory.c
	gcc ${FLAGS} ${INCLUDES} ./src/memory.c -c -o ./build/memory.o

./build/chip8stack.o: ./src/chip8stack.c
	gcc ${FLAGS} ${INCLUDES} ./src/chip8stack.c -c -o ./build/chip8stack.o

./build/keyboard.o: ./src/keyboard.c
	gcc ${FLAGS} ${INCLUDES} ./src/keyboard.c -c -o ./build/keyboard.o

./build/chip8.o: ./src/chip8.c
	gcc ${FLAGS} ${INCLUDES} ./src/chip8.c -c -o ./build/chip8.o

./build/chip8screen.o: ./src/chip8screen.c
	gcc ${FLAGS} ${INCLUDES} ./src/chip8screen.c -c -o ./build/chip8screen.o

clean:
	del build\*