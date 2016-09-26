#include "enemy.h"
#include "gold.h"
#include "potion.h"
#include "PRNG.h"
#include <vector>
#include <iostream>
#include <string>
#include <cstdlib>
#include <cmath>
using namespace std;

/*****************Enemy()******************
Purpose: default ctor, set Merchant's hostile to false when creating Merchant object.
***************************************************/
Enemy::Enemy(string name, int HP, int atk, int def, char symbol): Character(name, HP, atk, def, symbol), move(true){
	if(this->Name == "Merchant"){
		hostile = false;
	}else{
		hostile = true;
	}
};

Enemy::~Enemy(){};

/*****************attack(&player)******************
Purpose: Enemy attack. 
		 Modify player's stats.
***************************************************/
void Enemy::attack(Player &player){
	double damage = ceil(getAtk() * (100.0 - player.getDef())/100.0);
	player.changeHP(-damage);
	cout << "The evil " << Name << " strikes you for " << damage << " damage!" << endl;
};

bool Enemy::die(){
	return cur_HP == 0;
};

/*****************check()******************
Purpose: set every stats to normal situation.
***************************************************/
void Enemy::check(){
	if(cur_HP < 0) cur_HP = 0;
	if(cur_HP > getMaxHP()) cur_HP = getMaxHP();
	if(cur_atk < 0) cur_atk = 0;
	if(cur_def < 0) cur_def = 0;
	if(cur_def > 100) cur_def = 100;
}

/*****************setHostile()******************
Purpose: change hostile state of enemy, called in map.
***************************************************/
void Enemy::setHostile(bool hostile){
	this->hostile = hostile;
};

bool Enemy::is_hostile(){
	return hostile;
};

/*****************set_move()******************
Purpose: set the state of enemy if they can move or not
		 Testing purpose.
***************************************************/
void Enemy::set_move(bool move){
	this->move = move;
};

bool Enemy::can_move(){
	return move;
};

void Enemy::pickupPotion(Potion *p){
	string potionName = p->getName();
	if(potionName == "Restore health"){
		cur_HP += 30;
	}else if(potionName == "Boost Atk"){
		cur_atk += 10;
	}else if(potionName == "Boost Def"){
		cur_def += 10;
	}else if(potionName == "Poison health"){
		cur_HP -= 15;
	}else if(potionName == "Wound Atk"){
		cur_atk -= 5;
	}else{
		cur_def -= 5;
	}
};

GridBug::GridBug(): Enemy("Grid Bug", 50, 20, 10, 'X'){};
GridBug::~GridBug() {};

Goblin::Goblin(): Enemy("Goblin", 75, 30, 20, 'g'){};
Goblin::~Goblin() {};

Merchant::Merchant(): Enemy("Merchant", 100, 75, 5, 'M'){};
Merchant::~Merchant() {};

Orc::Orc(): Enemy("Orc", 120, 30, 30, 'O'){};
Orc::~Orc() {};

Dragon::Dragon(): Enemy("Dragon", 150, 50, 10, 'D'){};
Dragon::~Dragon() {};
