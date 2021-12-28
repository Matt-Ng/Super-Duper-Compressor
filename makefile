CC = g++
CFLAGS = -std=c++11 -Wall

all: huffman

huffman: main.o encode.o decode.o
	$(CC) $(CFLAGS) -o huffman main.o encode.o decode.o

main.o: main.cc encode.h
	$(CC) $(CFLAGS) -c main.cc

encode.o: encode.cc encode.h
	$(CC) $(CFLAGS) -c encode.cc
decode.o: decode.cc encode.h
	$(CC) $(CFLAGS) -c decode.cc