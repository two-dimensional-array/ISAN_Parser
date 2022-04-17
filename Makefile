CC=gcc
PY=python3
CFLAGS=-c -x c++ -Wall --std=c++17 -lstdc++
EXECUTABLE=parse_isan

all: main

main: main.o ISANObject.o ISANParser.o
	$(CC) main.o ISANObject.o ISANParser.o -o $(EXECUTABLE) -lstdc++
	$(PY) test.py

main.o: src/main.cc
	$(CC) $(CFLAGS) src/main.cc

ISANObject.o: src/ISANObject.h src/ISANObject.cc
	$(CC) $(CFLAGS) src/ISANObject.h src/ISANObject.cc

ISANParser.o: src/ISANParser.h src/ISANParser.cc
	$(CC) $(CFLAGS) src/ISANParser.h src/ISANParser.cc

clean:
	rm -rf *.o *.gch $(EXECUTABLE)
