#ifndef ENEMY_H
#define ENEMY_H
#include "character.h"
#include "game.h"
#include "PRNG.h"
#include <string>
#include <vector>
using namespace std;

class Game;
class Gold;
class Player;
class Potion;

class Enemy : public Character {
	protected:
		bool hostile;
		bool move;

	public:
		Enemy(string Name, int HP, int atk, int def, char symbol);
		virtual ~Enemy();
		
		void attack(Player &player);
		bool die();						// check if die
		void check();
		void setHostile(bool hostile);
		bool is_hostile();
		void set_move(bool move);
		bool can_move();
		void pickupPotion(Potion *p);
};

class GridBug : public Enemy {
	public:
		GridBug();
		~GridBug();
};

class Goblin : public Enemy {
	public:
		Goblin();
		~Goblin();
};

class Merchant : public Enemy {
	public:
		Merchant();
		~Merchant();
};

class Orc : public Enemy {
	public:
		Orc();
		~Orc();
};

class Dragon : public Enemy {
	public:
		Dragon();
		~Dragon();
};

#endif
