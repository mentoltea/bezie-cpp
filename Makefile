CC=g++
FLAGS=
BUILD_DIR=./build/
TARGET_NAME=main.exe
LIBS=-lm 

all: target

clean:
	rm -rf $(BUILD_DIR)/*

target: bezie.o curve.o main.o
	$(CC) $(FLAGS) $(BUILD_DIR)bezie.o $(BUILD_DIR)curve.o $(BUILD_DIR)main.o -o $(BUILD_DIR)$(TARGET_NAME) $(LIBS)

bezie.o: bezie.cpp
	$(CC) $(FLAGS) -c bezie.cpp -o $(BUILD_DIR)bezie.o 

curve.o: curve.cpp
	$(CC) $(FLAGS) -c curve.cpp -o $(BUILD_DIR)curve.o 

main.o: main.cpp
	$(CC) $(FLAGS) -c main.cpp -o $(BUILD_DIR)main.o