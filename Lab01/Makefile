TARGET := main.out
SRC := ${wildcard src/*.c}
OBJ := ${patsubst %.c,%.o,${SRC}}
OBJ := ${patsubst src/%,build/%,${OBJ}}
CC := gcc
C_FLAGS := -Iinclude -g -Wall

all: ${OBJ}
	${CC} $^ -o ${TARGET} ${C_FLAGS}

build/%.o: src/%.c
	@mkdir -p build
	${CC} -o $@ -c $< ${C_FLAGS}

clean:
	rm -rf build
	rm -rf ${TARGET}