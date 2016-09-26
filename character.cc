#include "character.h"
#include <string>
#include <vector>
using namespace std;

/*****************Character()******************
Purpose: ctor
***************************************************/
Character::Character(string name, int HP, int atk, int def, char symbol){
	this->Name = name;
	cur_HP = HP;
	max_HP = HP;
	cur_atk = atk;
	max_atk = atk;
	cur_def = def;
	max_def = def;
	this->symbol = symbol;
};
        
Character::~Character(){};

int Character::getMaxHP(){
	return max_HP;
};

int Character::getHP(){
	return cur_HP;
};
        
int Character::getAtk(){
	return cur_atk;
};
        
int Character::getDef(){
	return cur_def;
};

/*****************changeHP()******************
Purpose: change character HP when being attacked.
***************************************************/
int Character::changeHP(int change){
	return cur_HP += change;
};

/*****************resetAtk()******************
Purpose: Used for going downstairs
***************************************************/
void Character::resetAtk(){
	cur_atk = max_atk;
};

/*****************resetDef()******************
Purpose: Used for going downstairs.
***************************************************/ 
void Character::resetDef(){
	cur_def = max_def;
};

string Character::getName(){
	return Name;
};

char Character::getSymbol(){
	return symbol;
};

/*****************setCharPos()******************
Purpose: set character position, called in map.
***************************************************/
void Character::setCharPos(vector<int> charPos){
	this->charPos = charPos;
};

vector<int> Character::getCharPos(){
	return charPos;
};
