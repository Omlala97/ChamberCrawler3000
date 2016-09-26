#ifndef GAME_H
#define GAME_H
#include <iostream>
#include <string>
#include <vector>
#include "map.h"
using namespace std;

class Map;

class Game {
	Map *map;
public:
	Game(string player, char* infile);				// ctor, takes player choice and input file
	~Game();										// dtor

	bool isDead();									// check if player has died
	bool isWon();									// check if player has won
	int playerGP();									// get player's GP

	void move(string direction);					// make player move
	void attack(string direction);					// make player attack
	void use(string direction);						// make player pickup items

	void stopWander();								// testing purposes

	void print();									// print Map and stats
};

#endif
