FLAGS=-pedantic -Wall -Werror -Wno-sign-compare -Wno-long-long -pthread -std=c++11
LIBS=-lm -lzmq
COMPILLER=g++

all: start

start: map.o geometry.o connection.o parser.o converter.o interface.o main.o 
	$(COMPILLER) $(FLAGS) -o sea-battle map.o geometry.o connection.o parser.o converter.o interface.o main.o $(LIBS)

main.o: main.cpp
	$(COMPILLER) -c $(FLAGS) main.cpp $(LIBS)

map.o: map.cpp
	$(COMPILLER) -c $(FLAGS) map.cpp $(LIBS)

parser.o: parser.cpp
	$(COMPILLER) -c $(FLAGS) parser.cpp $(LIBS)

geometry.o: geometry.cpp
	$(COMPILLER) -c $(FLAGS) geometry.cpp $(LIBS)

converter.o: converter.cpp
	$(COMPILLER) -c $(FLAGS) converter.cpp $(LIBS)

interface.o: interface.cpp
	$(COMPILLER) -c $(FLAGS) interface.cpp $(LIBS)

connection.o: connection.cpp
	$(COMPILLER) -c $(FLAGS) connection.cpp $(LIBS)

clean:
	-rm -f *.o *.gch sea-battle
