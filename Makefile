CC=g++
CFLAGS=--std=c++11

all:
	$(CC) $(CFLAGS) strcmp.cpp -o strcmp
	./strcmp ascii_file_example.txt
