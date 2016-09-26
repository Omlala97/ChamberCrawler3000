#ifndef GOLD_H
#define GOLD_H
#include "item.h"
#include <vector>
#include <iostream>
using namespace std;

class Gold : public Item{
	int gold;
	bool pickup;
public:
	Gold(int value);
	~Gold();
	bool can_pickup();
	void set_pickup(bool pp);
	int getGold();
};

#endif
