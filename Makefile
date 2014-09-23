GPSTime:test.o TimeStruct.o
	g++ -Wall test.o TimeStruct.o -o GPSTime
test.o:test.cpp 
	g++ -c test.cpp
TimeStruct.o:TimeStruct.cpp TimeStruct.h
	g++ -c TimeStruct.cpp
.PHONY: clean
clean:
	rm *~  *.o 
