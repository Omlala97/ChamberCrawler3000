#include "item.h"
#include <iostream>
using namespace std;

Item::Item(string name){
	this->name = name;
};

Item::~Item(){};

string Item::getName(){
	return name;
};

/*****************setItemPos()******************
Purpose: set item position, called in map.
***************************************************/
void Item::setItemPos(vector<int> itemPos){
	this->itemPos = itemPos;
};

vector<int> Item::getItemPos(){
	return itemPos;
};

