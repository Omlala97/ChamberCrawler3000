#ifndef POTION_H
#define POTION_H
#include "item.h"
#include <string>
#include <vector>
#include <iostream>
using namespace std;

class Potion : public Item {
	int value;                       // the value of this kind of potion.
	string name;

public:
	Potion(string name, int value);
	~Potion();
};

class RH : public Potion {
  public:
    RH();
};

class BA : public Potion {
  public:
    BA();
};

class BD : public Potion {
  public:
    BD();
};

class PH : public Potion {
  public:
    PH();
};

class WA : public Potion {
  public:
    WA();
};

class WD : public Potion {
  public:
    WD();
};

#endif
