#include <iostream>
using namespace std;

const char MAP_WIDTH = 10;
const char MAP_HEIGHT = MAP_WIDTH;
const char MAP_ELEMENT_EMPTY = '.';
const char MAP_ELEMENT_SPACE = ' ';
const char MAP_ELEMENT_DAMAGED='X';

class Map {
	char Matrix[MAP_HEIGHT][MAP_WIDTH];
public:
	Map(){
		for(int i=0; i<MAP_HEIGHT; i++){
			for(int j=0; j<MAP_WIDTH; j++){
				Matrix[i][j]=MAP_ELEMENT_EMPTY;
			}
		}
	}
	void Print(){
		cout << MAP_ELEMENT_SPACE << MAP_ELEMENT_SPACE << MAP_ELEMENT_SPACE;
		for(int i=0;i<MAP_WIDTH;i++){
			cout<< (char)('A'+i)<< MAP_ELEMENT_SPACE;
		}
		cout<<endl;
		for(int i=0; i<MAP_HEIGHT-1; i++){
			cout << 1 + i << MAP_ELEMENT_SPACE << MAP_ELEMENT_SPACE;
			for(int j=0; j<MAP_WIDTH; j++){
				cout<<Matrix[i][j]<<MAP_ELEMENT_SPACE;
			}
			cout<<endl;
		}
		cout << 10 << MAP_ELEMENT_SPACE;
		for(int i=0; i<MAP_WIDTH; i++){
			cout<<Matrix[i][0]<<MAP_ELEMENT_SPACE;
		}
		cout << endl;
	}
	void Shoot(int x, int y){
		Matrix[x][y]=MAP_ELEMENT_DAMAGED;
	}
};
int main()
{	
	Map map;
	map.Print();
	map.Shoot(1, 2);
	map.Shoot(5, 9);
	map.Print();
	return 0;
}