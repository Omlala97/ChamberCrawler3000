#ifndef ITEM_H
#define ITEM_H
#include <iostream>
#include <string>
#include <vector>
using namespace std;

class Item {
	string name;
	vector<int> itemPos;
public:
	Item(string name);
	~Item();
	void setItemPos(vector<int> itemPos);
	vector<int> getItemPos();
	string getName();
};

#endif
