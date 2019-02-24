#include <iostream>
#include "map.h"
using namespace std;
int main()
{	
	Map map;
	map.Print();
	map.Shoot(1, 2);
	map.Shoot(5, 9);
	map.Print();
	return 0;
}