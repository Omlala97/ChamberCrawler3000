#include "game.h"
#include "PRNG.h"
#include <iostream>
#include <vector>
using namespace std;

/*****************Game()******************
Purpose: game ctor, will call map ctor, and create a new map.
***************************************************/
Game::Game(string player, char* infile){
	map = new Map(player, infile);
	map->create();
};

Game::~Game(){
	delete map;
};

bool Game::isDead(){
	return map->isDead();
};

bool Game::isWon(){
	return map->isWon();
};

int Game::playerGP(){
	return map->getGold();
};

void Game::move(string direction){
	map->playerMove(direction);
};

void Game::attack(string direction){
	map->playerAttack(direction);
};

void Game::use(string direction){
	map->playerUse(direction);
};

void Game::stopWander(){
	map->stopWander();
};

void Game::print(){
	map->print();
};
