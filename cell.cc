#include "cell.h"
#include <iostream>
#include <vector>
#include <iostream>
using namespace std;


Cell::Cell() : symbol('0'), my_symbol('0'), numNeighbours(0){};

Cell::~Cell(){};

/*****************setCoords()******************
Purpose: set coords of each cell, called in map when initializing.
***************************************************/
void Cell::setCoords(int row, int col){
	cellPos.erase(cellPos.begin(),cellPos.end());
	this->cellPos.push_back(row);
	this->cellPos.push_back(col);
};

int Cell::getNumNeighbours(){
	return numNeighbours;
};

vector<int> Cell::getCellPos(){
	return cellPos;	
};

vector<Cell*> Cell::getNeighbours(){
	return neighbours;
};

/*****************playerAround()******************
Purpose: return true when there's player's symbol around 
	 	 THE cell.
***************************************************/
bool Cell::playerAround(){
	for(int i = 0; i < numNeighbours; i++){
		if(neighbours.at(i)->getSymbol() == '@'){
			return true;
		}
	}
	return false;
};     

/*****************addNeighbour()******************
Purpose: Add neighbours for each cell, called in map.
***************************************************/
void Cell::addNeighbour(Cell* neighbour){
	neighbours.push_back(neighbour);
	numNeighbours++;
//	showNeighbours();
};

/*void Cell::showNeighbours(){
	cout << "Current state of the neighbours for " << cellPos.at(0) << "," << cellPos.at(1) << " is" << endl;
	for (int i=0;i<numNeighbours;i++) {
		cout << "Neighbour " << i << " is " << neighbours[i]->getCellPos().at(0) << "," << neighbours[i]->getCellPos().at(1) << endl;
	}
}*/

/*****************setOriSymbol()******************
Purpose: Store original symbol of each cell, called in map.
***************************************************/
void Cell::setOriSymbol(char symbol){
	my_symbol = symbol;
};

/*****************setSymbol()******************
Purpose: Set current symbol for each cell, called in map.
***************************************************/
void Cell::setSymbol(char symbol){
	this->symbol = symbol;	
};

/*****************getSymbol()******************
Purpose: Return original symbol when nothing is occupied on 
		 this cell, return gameobject's symbol otherwise.
***************************************************/
char Cell::getSymbol(){
	if(symbol == '0') {
		return my_symbol;
	}else{
		return symbol;
	}
};

char Cell::getMySymbol(){
	return my_symbol;
};

/*****************getChamber()******************
Purpose: return chamber info for each cell.(not used)
***************************************************/
int Cell::getChamber(){
	int row = getCellPos().at(0);
	int col = getCellPos().at(1);
	if(row<=8){
		if(col<=28) return 0;
		return 1;
	}else if(row<=12){
		if(col<=49) return 2;
		return 1;
	}else{
		if(col<=24) return 3;
		return 4;
	}
};
