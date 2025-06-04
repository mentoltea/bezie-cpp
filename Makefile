CC=g++
FLAGS=
BUILD_DIR=./build/
TARGET_NAME=main.exe
LIBS=-lm 

all: target

clean:
	rm -rf $(BUILD_DIR)/*

target: bezie.o curve.o main.o alg.o
	$(CC) $(FLAGS) $(BUILD_DIR)bezie.o $(BUILD_DIR)curve.o $(BUILD_DIR)main.o $(BUILD_DIR)alg.o -o $(BUILD_DIR)$(TARGET_NAME) $(LIBS)

bezie.o: bezie.cpp
	$(CC) $(FLAGS) -c bezie.cpp -o $(BUILD_DIR)bezie.o 

curve.o: curve.cpp
	$(CC) $(FLAGS) -c curve.cpp -o $(BUILD_DIR)curve.o 

alg.o: alg.cpp
	$(CC) $(FLAGS) -c alg.cpp -o $(BUILD_DIR)alg.o 

main.o: main.cpp
	$(CC) $(FLAGS) -c main.cpp -o $(BUILD_DIR)main.o