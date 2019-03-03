#include <iostream>
#include "map.h"
using namespace std;
int main()
{	
	Map map;
	map.Shoot(0, 0);
	map.Shoot(0, 1);
	//map.Shoot(1, 1);
	cout << "For Me" << endl;
	map.PrintForMe();
	cout << "For Enemy" << endl;
	map.PrintForEnemy();
	return 0;
}