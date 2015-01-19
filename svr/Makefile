CC=g++
CXX=g++
Inc=../../mylib/
NetInc=-I../../mylib/
JsonInc=-I/home/arsee_p/lib/jsoncpp-master/include/
Libs=-lmymvc -lmynet
Libdir=-L../../mylib/lib/
FLAGS= -I$(Inc) --std=c++11 -g -O -fno-inline 
TARGT=talkto
OBJS=main.o logics.o member.o
DEBUG=-DDEBUG

$(TARGT): $(OBJS)
	$(CXX) -o $(TARGT) $(OBJS) -static $(Libdir) $(Libs) 

main.o: logics.h member.h controls.h mysession.h 
	$(CC) $(DEBUG) $(NetInc) $(JsonInc) $(FLAGS) -c main.cpp

logics.o:logics.cpp logics.h
	$(CC) $(FLAGS) $(JsonInc) -c logics.cpp

member.o: member.h 
	$(CC) $(FLAGS) $(JsonInc) -c member.cpp

clean:
	rm -f $(TARGT) $(OBJS)
