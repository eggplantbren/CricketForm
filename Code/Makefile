CFLAGS = -O2 -Wall -Wextra -ansi -pedantic
LIBS =  -ldnest3 -lgsl -lgslcblas -lboost_system -lboost_thread
OBJECTS = Data.o MyModel.o

default:
	g++ $(CFLAGS) -c *.cpp
	g++ -o main main.o $(OBJECTS) $(LIBS)
	rm -f *.o

