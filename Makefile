FLAGS=-pedantic -Wall -Werror -Wno-sign-compare -Wno-long-long -lm -pthread
COMPILLER=g++

all: start

start: map.o main.o
	$(COMPILLER) $(FLAGS) -o sea-battle map.o main.o

main.o: main.cpp
	$(COMPILLER) -c $(FLAGS) main.cpp

map.o: map.cpp
	$(COMPILLER) -c $(FLAGS) map.cpp

clear:
	-rm -f *.o *.gch sea-battle
