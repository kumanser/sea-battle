FLAGS=-pedantic -Wall -Werror -Wno-sign-compare -Wno-long-long -lm -pthread
COMPILLER=g++

all: start

start: map.o geometry.o main.o 
	$(COMPILLER) $(FLAGS) -o sea-battle map.o geometry.o main.o

main.o: main.cpp
	$(COMPILLER) -c $(FLAGS) main.cpp

map.o: map.cpp
	$(COMPILLER) -c $(FLAGS) map.cpp

geometry.o: geometry.cpp
	$(COMPILLER) -c $(FLAGS) geometry.cpp

clean:
	-rm -f *.o *.gch sea-battle
