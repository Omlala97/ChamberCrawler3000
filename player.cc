#include "player.h"
#include <vector>
#include <iostream>
#include <cmath>
#include "character.h"
#include "enemy.h"
#include "game.h"
#include <string>
#include <cstdlib>
#include <cmath>
using namespace std;


Player::Player(string name, int HP, int atk, int def, char symbol): Character(name, HP, atk, def, symbol), gold(0) {};

Player::~Player(){};

bool Player::die(){
	return cur_HP==0;
};

/*****************check()******************
Purpose: set every stats to normal situation.
***************************************************/
void Player::check(){
	if(cur_HP < 0) cur_HP = 0;
	if(cur_HP > getMaxHP()) cur_HP = getMaxHP();
	if(cur_atk < 0) cur_atk = 0;
	if(cur_def < 0) cur_def = 0;
	if(cur_def > 100) cur_def = 100;
}

/*****************attack(&enemy)******************
Purpose: Player attack.
		 modify enemy's each stats.
***************************************************/
void Player::attack(Enemy &enemy){
	double damage = ceil(getAtk() * (100.0 - enemy.getDef())/100.0);
	enemy.changeHP(-damage);
	enemy.check();	
	cout << "You attack the " << enemy.getName() << " with your ";
	if(getName() == "Knight"){
		cout << "Sword of Segfault";
	}else if(getName() == "Wizard"){
		cout << "Infinite Loop Laser";
	}else {
		cout << "Memory Corruption Katana";
	}
	cout << " for " << damage << " damage!" << endl;
};

/*****************gainHP()******************
Purpose: Player gain 5HP for every turn.
***************************************************/
void Player::gainHP(){
	cur_HP += 5;
	check();
};
		
int Player::getGold(){
	return gold;
};

void Player::pickupGold(int val){
		gold+=val;
};

void Player::pickupPotion(Potion *p){
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

/*****************getChamber()******************
Purpose: return chamber number for current player position.
		 called in map, used for spawning stair position.
***************************************************/
int Player::getChamber(){
	int row = getCharPos().at(0);
	int col = getCharPos().at(1);
	if(row<=8){
		if(col<=28) return 0;
		return 1;
	}else if(row<=12){
		if(col<=49) return 2;
		return 1;
	}else{
		if(col<=24) return 3;
		return 4;
	}
};

Knight::Knight(): Player("Knight", 100, 50, 50, '@') {};

Knight::~Knight() {};

Wizard::Wizard(): Player("Wizard", 60, 25, 0, '@') {};

Wizard::~Wizard() {};

Samurai::Samurai(): Player("Samurai", 80, 50, 15, '@') {};

Samurai::~Samurai() {};
