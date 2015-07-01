CC=g++
CXX=g++
FLAGS= $(Inc) --std=c++11 -c -g -O -fno-inline 
OBJS=models-odb.o modelsys-odb.o
DEBUG=-DDEBUG

all: $(OBJS)

models-odb.o: models-odb.cxx
	$(CC) $(FLAGS) models-odb.cxx

modelsys-odb.o: modelsys-odb.cxx
	$(CC) $(FLAGS) modelsys-odb.cxx

clean:
	rm -f *.o
