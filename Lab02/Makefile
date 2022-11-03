TARGET := main.out
SRC := ${wildcard src/*.cpp}
OBJ := ${patsubst %.cpp,%.o,${SRC}}
OBJ := ${patsubst src/%,build/%,${OBJ}}
CC := g++
CXX_FLAGS := -Iinclude -g -Wall

all: ${OBJ}
	${CC} $^ -o ${TARGET} ${CXX_FLAGS}

build/%.o: src/%.cpp
	@mkdir -p build
	${CC} -o $@ -c $< ${CXX_FLAGS}

clean:
	rm -rf build
	rm -rf ${TARGET}