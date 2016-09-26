#include "gold.h"
using namespace std;

Gold::Gold(int value) : Item("gold"){
	gold = value;
	pickup = true;
};

Gold::~Gold(){};

bool Gold::can_pickup(){
	return pickup;
};

/*****************set_pickup()******************
Purpose: set the state of if the can be picked up or not.
***************************************************/
void Gold::set_pickup(bool pp){
	this->pickup = pp;
};

int Gold::getGold(){
	return gold;
};
