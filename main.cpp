#include <iostream>
#include <ctime>
#include "interface.h"
using namespace std;
int main()
{
	srand(time(NULL));
	Interface interface;
	interface.Start();

	return 0;
}