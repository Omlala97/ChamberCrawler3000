#include "potion.h"
using namespace std;

Potion::Potion(string name, int value) : Item(name){
	this->value = value;
};

Potion::~Potion(){};

RH::RH(): Potion("Restore health", 30) {}

BA::BA(): Potion("Boost Atk", 10) {}

BD::BD(): Potion("Boost Def", 10) {}

PH::PH(): Potion("Poison health", -15) {}

WA::WA(): Potion("Wound Atk", -5) {}

WD::WD(): Potion("Wound Def", -5) {}
