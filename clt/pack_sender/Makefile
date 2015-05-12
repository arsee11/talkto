Libs=-L../../../mylib/lib
Includes=-I../../../mylib
std=--std=c++11

pack_sender:pack_sender.o
	g++ $(std) pack_sender.o -o pack_sender $(Libs) -lmymvc -lmynet

pack_sender.o: pack_sender.cpp
	g++ $(std) $(Includes) -g -c pack_sender.cpp

clean:
	rm *.o pack_sender
