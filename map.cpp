#include <iostream>
using namespace std;

const char MAP_WIDTH = 10;
const char MAP_HEIGHT = MAP_WIDTH;
const char MAP_ELEMENT_EMPTY = '.';
const char MAP_ELEMENT_SPACE = ' ';
const char MAP_ELEMENT_DAMAGED='X';
int NumCount(int num){
	int cnt=0;
	while(num != 0){
		num/=10;
		cnt++;
	}
	return cnt;
}

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
		int max_num_length = NumCount(MAP_HEIGHT);
		for (int i = 0; i < max_num_length; i++) {
			cout << MAP_ELEMENT_SPACE;
		}
		cout << MAP_ELEMENT_SPACE;
		for(int i=0;i<MAP_WIDTH;i++){
			cout<< (char)('A'+i)<< MAP_ELEMENT_SPACE;
		}
		cout<<endl;
		for(int i=0; i<MAP_HEIGHT; i++){
			int current_num_length = NumCount(i+1);
			cout << 1 + i << MAP_ELEMENT_SPACE;
			for(int j=0; j<max_num_length-current_num_length; j++){
				cout<<MAP_ELEMENT_SPACE;
			} 
			for(int j=0; j<MAP_WIDTH; j++){
				cout<<Matrix[i][j]<<MAP_ELEMENT_SPACE;
			}
			cout<<endl;
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