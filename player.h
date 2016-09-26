#ifndef PLAYER_H
#define PLAYER_H
#include "character.h"
#include "enemy.h"
#include "game.h"
#include "potion.h"
#include <string>
using namespace std;

class Game;
class Enemy;

class Player : public Character {
	protected:
		string Name;
		int gold;	
    public:
		Player(string name, int HP, int atk, int def, char symbol);
		virtual ~Player();

		bool die();
		void check();
		void attack(Enemy &enemy);
		
		void gainHP();
		int getGold();
		void pickupGold(int val);
		void pickupPotion(Potion *p);

		int getChamber();
};

class Knight : public Player {
	public:
		Knight();
		~Knight();
};

class Wizard : public Player {
	public:
		Wizard();
		~Wizard();
};

class Samurai : public Player{
	public:
		Samurai();
		~Samurai();
};

#endif
