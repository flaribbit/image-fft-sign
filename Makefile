CC=g++
CFLAGS=-s -O2 -DNDEBUG -I include

a.exe: lodepng.o main.o
	$(CC) lodepng.o main.o $(CFLAGS)
main.o: main.cpp
	$(CC) -c main.cpp $(CFLAGS)
lodepng.o: include/lodepng.cpp
	$(CC) -c include/lodepng.cpp $(CFLAGS)
