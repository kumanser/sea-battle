FLAGS=-pedantic -Wall -Werror -Wno-sign-compare -Wno-long-long -lm -pthread -std=c++11
COMPILLER=g++

all: start

start: map.o geometry.o parser.o converter.o interface.o main.o 
	$(COMPILLER) $(FLAGS) -o sea-battle map.o geometry.o parser.o converter.o interface.o main.o

main.o: main.cpp
	$(COMPILLER) -c $(FLAGS) main.cpp

map.o: map.cpp
	$(COMPILLER) -c $(FLAGS) map.cpp

parser.o: parser.cpp
	$(COMPILLER) -c $(FLAGS) parser.cpp

geometry.o: geometry.cpp
	$(COMPILLER) -c $(FLAGS) geometry.cpp

converter.o: converter.cpp
	$(COMPILLER) -c $(FLAGS) converter.cpp

interface.o: interface.cpp
	$(COMPILLER) -c $(FLAGS) interface.cpp

clean:
	-rm -f *.o *.gch sea-battle
