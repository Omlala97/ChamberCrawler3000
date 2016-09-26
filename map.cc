#include "map.h"
#include "game.h"
#include "gold.h"
#include "player.h"
#include "character.h"
#include "enemy.h"
#include "item.h"
#include "potion.h"
#include "cell.h"
#include <iostream>
#include <vector>
#include <fstream>
#include <cassert>
#include <string>
#include <sstream>
#include <algorithm>
#include "PRNG.h"
using namespace std;

extern PRNG prng;

ifstream ifs;

vector<int> playerPos;

/*****************Map(player, infile)******************
Purpose: default constructor, initialize player, gold,
		 potions and enemies.
***************************************************/
Map::Map(string player, char* infile): infile(infile), floor(1), turns(1), dragonCount(0), dead(false), won(false), can_move(true){	
	if(infile == NULL){												// when there's no input file
		ifs.open("sample-cc3k-map.txt");
	}else{															// open input file
		ifs.open(infile);
	}
	this->player = NULL;
		// choose player, new Object
	if(player == "w") this->player = new Wizard();
	if(player == "s") this->player = new Samurai();
	if(player == "k") this->player = new Knight();
	cout << "You have chosen to play as a " << this->player->getName() << ". Good luck." << endl;
		// initialize enemies, gold and potionPos
	for(int i = 0; i < 25; i++) {
    	enemies[i] = NULL;
    	if(i < 15) {
    		gold[i] = NULL;
    		potions[i] = NULL;
    	}
  	}
};	

/***********************~Map()***********************
Purpose: dtor
***************************************************/
Map::~Map(){
	delete player;
	for(int i = 0; i < 25; i++){
		delete[] board[i];
	}
	delete[] board;
	for(int j = 0; j < 25; j++){
		delete enemies[j];
		if(j < 15){
			delete gold[j];
			delete potions[j];
		}
	}
};

/*****************create()******************
Purpose: create a brand new board, spawn player, stair,
		gold, enemies, potions on it.
		Read in from an input file or default map.
***************************************************/
void Map::create(){
	if(infile!=NULL){												// read in from input file
		int goldNum = 0, potionNum = 0, enemyNum = 0;
		string line;
		board = new Cell*[25];										// initilize matrix of Cells
		for(int i = 0; i < 25; i++){
			board[i] = new Cell[79];
			getline(ifs, line);
			for (int j = 0; j < 79; j++){
				board[i][j].setCoords(i, j);
				vector<int> Pos(2);
				Pos.at(0) = i;
				Pos.at(1) = j;										// set Cells for enemies,potions,gold,stair,player's original symbol as .
				if(line[j] != '.' && line[j] != '+' && line[j] != '-' && line[j] != '|' && line[j] != ' ' && line[j] != '#'){
					board[i][j].setOriSymbol('.');
						// RH 
					if(line[j] == '0') {							// for gold, potions and enemies, allocate memory for them when reading
						potions[potionNum] = new RH();
						board[i][j].setSymbol('!');
						potions[potionNum]->setItemPos(Pos);
						potionNum++;
					}else if(line[j] == '1'){						// BA
						potions[potionNum] = new BA();
						board[i][j].setSymbol('!');
						potions[potionNum]->setItemPos(Pos);
						potionNum++;
					}else if(line[j] == '2'){						// BD
						potions[potionNum] = new BD();
						board[i][j].setSymbol('!');
						potions[potionNum]->setItemPos(Pos);
						potionNum++;
					}else if(line[j] == '3'){						// PH
						potions[potionNum] = new PH();
						board[i][j].setSymbol('!');
						potions[potionNum]->setItemPos(Pos);
						potionNum++;
					}else if(line[j] == '4'){						// WA
						potions[potionNum] = new WA();
						board[i][j].setSymbol('!');
						potions[potionNum]->setItemPos(Pos);
						potionNum++;
					}else if(line[j] == '5'){						// WD
						potions[potionNum] = new WD();
						board[i][j].setSymbol('!');
						potions[potionNum]->setItemPos(Pos);
						potionNum++;
					}else if(line[j] == '6'){						// Gold Pile
						gold[goldNum] = new Gold(10);
						board[i][j].setSymbol('$');
						gold[goldNum]->setItemPos(Pos);
						goldNum++;
					}else if(line[j] == '7'){						// Dragon Hoard
						gold[goldNum] = new Gold(50);
						board[i][j].setSymbol('$');
						gold[goldNum]->setItemPos(Pos);
						gold[goldNum]->set_pickup(false);
						goldNum++;
					}else if(line[j] == '@'){						// player
						player->setCharPos(Pos);
						board[i][j].setSymbol('@');
						playerPos = player->getCharPos();
					}else if(line[j] == '>'){						// stair
						board[i][j].setSymbol('>');
					}else if(line[j] == 'X'){						// Grid Bug
						enemies[enemyNum] = new GridBug();
						board[i][j].setSymbol('X');
						enemies[enemyNum]->setCharPos(Pos);
						enemyNum++;
					}else if(line[j] == 'g'){						// Goblin
						enemies[enemyNum] = new Goblin();
						board[i][j].setSymbol('g');
						enemies[enemyNum]->setCharPos(Pos);
						enemyNum++;
					}else if(line[j] == 'M'){						// Merchant
						enemies[enemyNum] = new Merchant();
						board[i][j].setSymbol('M');
						enemies[enemyNum]->setCharPos(Pos);
						enemyNum++;
					}else if(line[j] == 'O'){						// Orc
						enemies[enemyNum] = new Orc();
						board[i][j].setSymbol('O');
						enemies[enemyNum]->setCharPos(Pos);
						enemyNum++;
					}else if(line[j] == 'D'){						// Dragon
						enemies[enemyNum] = new Dragon();
						board[i][j].setSymbol('D');
						enemies[enemyNum]->setCharPos(Pos);
						enemyNum++;
						dragonCount++;
					}
				}else{
					board[i][j].setOriSymbol(line[j]);				// store original symbol
				}
			} 
		}	
		for(int i = 3; i < 22; i++){								// add neighbours for every cell
			for (int j = 3; j < 76; j++){
				for (int dx=-1;dx<2;dx++) {
					for (int dy=-1;dy<2;dy++) {
						if(!(dx==0&&dy==0)) {
							board[i][j].addNeighbour(&board[i+dx][j+dy]);
						}		
					}
				}
			}
		}
	}else{															// read in default map and random generate game objects
		string line;
		board = new Cell*[25];
		for(int i = 0; i < 25; i++){
			board[i] = new Cell[79];
			getline(ifs, line);
			for (int j = 0; j < 79; j++){
				board[i][j].setCoords(i, j);
				board[i][j].setOriSymbol(line[j]);
			} 
		}							
		for(int i = 0; i < 25; i++){								// add neighbours for every cell
			for (int j = 0; j < 79; j++){
				if(getSymbol(i,j) == '.'){
					for (int dx=-1;dx<2;dx++) {
						for (int dy=-1;dy<2;dy++) {
							if(!(dx==0&&dy==0)) board[i][j].addNeighbour(&board[i+dx][j+dy]);			
						}
					}
				}
			}
		}
		// put player onto the board
		playerPos = randomPos();
		player->setCharPos(playerPos);
		int playerChamber = player->getChamber();
		board[playerPos.at(0)][playerPos.at(1)].setSymbol('@');
		// put stair onto the board
		int stairChamber = prng(3);
		if(stairChamber >= playerChamber) stairChamber += 1;
		vector<int> stairPos = randomPos(stairChamber);
		board[stairPos.at(0)][stairPos.at(1)].setSymbol('>');
			
		generatePotions();
		generateGold();
		generateEnemies();
	}
	print();
	if(!can_move) stopWander();
};

/*****************clear()******************
Purpose: clear current board. delete all other game objects,
		 only preserve player.
***************************************************/
void Map::clear(){
	player->resetAtk();
	player->resetDef();
	for(int i = 0; i < 25; i++){
		delete[] board[i];
	}
	delete[] board;
	for(int j = 0; j < 25; j++){
		delete enemies[j];
		enemies[j] = 0;
		if(j < 15){
			delete gold[j];
			delete potions[j];
			gold[j] = 0;
			potions[j] = 0;
		}
	}
};

/*****************stopWander()******************
Purpose: stop all enemies, testing purpose.
***************************************************/
void Map::stopWander(){
	for(int i = 0; i < 20+dragonCount; i++){
		if(enemies[i]){
			enemies[i]->set_move(false);
		}
	}
	can_move = false;
};

/*****************isDead()******************
Purpose: check if player has died.
***************************************************/
bool Map::isDead(){ return dead; };	

/*****************isWon()******************
Purpose: check if player has won.
***************************************************/
bool Map::isWon(){ return won; };

/*****************getGold()******************
Purpose: get player's GP.
***************************************************/
int Map::getGold(){ return player->getGold(); };

/*****************randomPos()******************
Purpose: Generate random position inside chambers.
		 Available to all characters AND items AND stair.
***************************************************/
vector<int> Map::randomPos(int chamber){
	int row, col;
	bool flag = false;
	while(!flag){										
		if(chamber == -1) chamber = prng(4);					// random choose chamber
		switch(chamber){
			case 0:
			// random choose row and column
			row = prng(3) + 3;
			col = prng(25) + 3;
			// if the choose cell is valid
			if(tile(row, col)) flag = true;
			break;
			case 1:
			row = prng(9) + 3;
			if(row > 6){
				col = prng(14) + 61;
			}else{
				col = prng(36) + 39;
			}
			if(tile(row, col)) flag = true;
			break;
			case 2:
			row = prng(2) + 10;
			col = prng(11) + 38;
			if(tile(row, col)) flag = true;
			break;
			case 3:
			row = prng(6) + 15;
			col = prng(20) + 4;
			if(tile(row, col)) flag = true;
			break;
			case 4:
			row = prng(5) + 16;
			if(row < 19){
				col = prng(10) + 65;
			}else{
				col = prng(38) + 37;
			}
			if(tile(row, col)) flag = true;
			break;		
		}
	}
	// create a new random_pos
	vector<int> random_pos;	
	random_pos.push_back(row);
	random_pos.push_back(col);
	return random_pos;
};

/*****************generatePotions()******************
Purpose: Generate random potions.
***************************************************/
void Map::generatePotions(){
	for(int i = 0; i < 10; i++){
		int type = prng(5);							// random choose potion number
		switch(type){
			case 5:
			potions[i] = new WD();					// allocate new object
			break;
			case 4:
			potions[i] = new WA();
			break;
			case 3:
			potions[i] = new PH();
			break;	
			case 2:
			potions[i] = new BD();
			break;
			case 1:
			potions[i] = new BA();
			break;
			case 0:
			potions[i] = new RH();
			break;		
		}
		vector<int> potionPos = randomPos();
		board[potionPos.at(0)][potionPos.at(1)].setSymbol('!');					// notify board to set symbol
		potions[i]->setItemPos(potionPos);	
	}
};

/*****************generateGold()******************
Purpose: Generate normal gold.
		 Generate Hoard, along with the Dragon.
***************************************************/
void Map::generateGold(){
	for(int i = 0; i < 10; i++){
		int type = prng(7);													// random choose gold number
		if(type < 7){
			gold[i] = new Gold(10);
		}else{
			gold[i] = new Gold(50);
		}
		if(type != 6){	
			vector<int> goldPos = randomPos();							// when random generate a Dragon Hoard
			gold[i]->setItemPos(goldPos);
			board[goldPos.at(0)][goldPos.at(1)].setSymbol('$');		
			gold[i]->set_pickup(true);	
		}else{
			Enemy *dragon = new Dragon();								// allocate for new Dragon
			vector<int> goldPos;
			vector<int> dragonPos;
			while(true){
				goldPos = randomPos();
				int dragonNum;
				dragonNum = prng(7);
				switch(dragonNum){										// random choose dragon position	
					case 7:
					dragonPos.push_back(goldPos.at(0)+1);
					dragonPos.push_back(goldPos.at(1)+1);
					break;
					case 6:
					dragonPos.push_back(goldPos.at(0)+1);
					dragonPos.push_back(goldPos.at(1));
					break;
					case 5:
					dragonPos.push_back(goldPos.at(0)+1);
					dragonPos.push_back(goldPos.at(1)-1);					
					break;
					case 4:
					dragonPos.push_back(goldPos.at(0));
					dragonPos.push_back(goldPos.at(1)+1);					
					break;
					case 3:
					dragonPos.push_back(goldPos.at(0)+1);
					dragonPos.push_back(goldPos.at(1)-1);					
					break;
					case 2:
					dragonPos.push_back(goldPos.at(0)-1);
					dragonPos.push_back(goldPos.at(1)+1);	
					break;
					case 1:
					dragonPos.push_back(goldPos.at(0)-1);
					dragonPos.push_back(goldPos.at(1));					
					break;
					case 0:
					dragonPos.push_back(goldPos.at(0)-1);
					dragonPos.push_back(goldPos.at(1)-1);					
					break;
				}
				if(!tile(dragonPos.at(0), dragonPos.at(1))) {
					dragonPos.pop_back();
					dragonPos.pop_back();
				}else{ break; }
			}
			gold[i]->setItemPos(goldPos);
			board[goldPos.at(0)][goldPos.at(1)].setSymbol('$');							// notify board of Dragon Hoard
			gold[i]->set_pickup(false);
			enemies[dragonCount] = dragon;
			enemies[dragonCount]->setCharPos(dragonPos);								// set position of dragon
																					// notify board of Dragon 
			board[enemies[dragonCount]->getCharPos().at(0)][enemies[dragonCount]->getCharPos().at(1)].setSymbol('D');
			dragonCount++;												// count dragon numbers
		}
	}
};

/*****************generateEnemies()******************
Purpose: Generate different enemies.
***************************************************/
void Map::generateEnemies(){
	for (int i = dragonCount; i < 20+dragonCount; i++){
		int type = prng(5);											// random generate number for enemies
		if(type < 2){
			enemies[i] = new GridBug();
		}else if(type < 4){
			enemies[i] = new Goblin();
		}else if(type < 5){
			enemies[i] = new Orc();
		}else{
			enemies[i] = new Merchant();
		}
		vector<int> enemyPos = randomPos();
		enemies[i]->setCharPos(enemyPos);									// set positions of enemies
		board[enemyPos.at(0)][enemyPos.at(1)].setSymbol(enemies[i]->getSymbol());			// notify board of enemies
	}
};

/*****************tile()******************
Purpose: return true if is tile.
***************************************************/
bool Map::tile(int row, int col){
	if(getSymbol(row, col) == '.'){
		return true;
	}else{
		return false;
	}
};

/*****************passages()******************
Purpose: return true if is passage.
***************************************************/
bool Map::passages(int row, int col){
	if(board[row][col].getSymbol() == '#'){
		return true;
	}else{
		return false;
	}
};

/*****************doorway()******************
Purpose: return true if is door.
***************************************************/
bool Map::doorway(int row, int col){
	if(board[row][col].getSymbol() == '+'){
		return true;
	}else{
		return false;
	}
};

/*****************validMove()******************
Purpose: valid for player to move.
***************************************************/
bool Map::validMove(int row, int col){
	bool value = tile(row, col)||passages(row, col)||doorway(row, col);
	return value;
};

/*****************getSymbol()******************
Purpose: get symbol for each cell.
***************************************************/
char Map::getSymbol(int row, int col){
	assert(0 <= row && row < 25 && 0 <= col && col < 79);
	return board[row][col].getSymbol();
};

/*****************changeBack()******************
Purpose: change cell back to original symbol
***************************************************/
void Map::changeBack(int row, int col){
	board[row][col].setSymbol(board[row][col].getMySymbol());
}

/*****************eraseGold()******************
Purpose: erase gold on map;
		 set gold to NULL;
		 player pickup the gold and gain GP.
		 cannot erase hoard when dragon is not slained.
***************************************************/
void Map::eraseGold(int row, int col){
	for(int i = 0; i < 15; i++){												// find gold in gold list
		if(gold[i] && gold[i]->getItemPos().at(0) == row && gold[i]->getItemPos().at(1) == col){
			if(!gold[i]->can_pickup()){										// dragon hoard cannot be taken before Dragon being slained
				cout << "It would be foolhardy to take the dragon's gold right now." << endl;
				break;
			}
			int value = gold[i]->getGold();
			if(value == 10){												// pick up 10GP gold pile.
				cout << "You pick up the Gold Pile worth " << value << "GP."<< endl;
			}else{															// pick up the dragon hoard.
				cout << "You successfully take the Dragon Hoard!" << endl;
			}
			gold[i] = NULL;
			player->pickupGold(value);
			changeBack(row, col);											// change the Cell back to original symbol.
			break;
		}
	}
};

/*****************erasePotion()******************
Purpose: pick up potion
		 erase potion on map
		 set potion to NULL
***************************************************/
void Map::erasePotion(int row, int col){
	for(int i = 0; i < 15; i++){												// find potion in potion list
		if(potions[i] && potions[i]->getItemPos().at(0) == row && potions[i]->getItemPos().at(1) == col){
			player->pickupPotion(potions[i]);								// player pick up potion
			string name = potions[i]->getName();
			cout << "You chug the " << name << " potion." << endl;
			potions[i] = NULL;												// erase potion
			changeBack(row, col);											// change the Cell back to original symbol.
			break;
		}
	}
};

/*****************randomPotion()******************
Purpose: Grid Bug drop random potion when slained.
***************************************************/
Potion *Map::randomPotion(){
	Potion *p = NULL;
	int type = prng(5);									// random choose potion number.
	switch(type){
		case 5:
		p = new WD();
		break;
		case 4:
		p = new WA();
		break;
		case 3:
		p = new PH();
		break;	
		case 2:
		p = new BD();
		break;
		case 1:
		p = new BA();
		break;
		case 0:
		p = new RH();
		break;		
	}
	return p;
};

/**********************eraseEnemy(row, col)****************
Purpose: Erase enemy on board, add new potion OR
		 gold, notify board to change symbol
		 When a dragon is slained, the hoard it protected, becomes
		 available.
*******************************************************************/
void Map::eraseEnemy(int row, int col){
	for(int i = 0; i < 20+dragonCount; i++){									// find the enemy in enemy list
		if(enemies[i] && enemies[i]->getCharPos().at(0) == row && enemies[i]->getCharPos().at(1) == col){
			if(enemies[i]->getSymbol() == 'X'){									// Grid bug drops potion
				Potion *dropPotion = randomPotion();
				for(int i = 0; i < 15; i++){
					if(!potions[i]){											// insert the droped potion into the potion list
						potions[i] = dropPotion;
						vector<int> potionPos;
						potionPos.push_back(row);
						potionPos.push_back(col);
						potions[i]->setItemPos(potionPos);						// set potion position
						board[row][col].setSymbol('!');							// notify board
						break;
					}
				}
			}else if(enemies[i]->getSymbol() == 'D'){							// when dragon dies
				vector<Cell *> c = board[row][col].getNeighbours();				// find a gold in dragon's neighbours
				int rr = 0, cc = 0;
				for(unsigned int i = 0; i < c.size(); i++){
					if(c.at(i)->getSymbol() == '$'){
						rr = c.at(i)->getCellPos().at(0);						// record this gold's position info
						cc = c.at(i)->getCellPos().at(1);
						break;
					}
				}
				for(int i = 0; i < 15; i++){									// find the gold in gold list
					if(gold[i] && gold[i]->getItemPos().at(0)==rr && gold[i]->getItemPos().at(1)==cc){
						gold[i]->set_pickup(true);								// make the gold be available to be picked up
						break;
					}
				}
				changeBack(row, col);											// change the Dragon cell back to original symbol
			}else{
				Gold *dropGold = new Gold(10);									// other enemies drop GP
				for(int i = 0; i < 15; i++){
					if(!gold[i]){												// insert droped GP into gold list
						gold[i] = dropGold;
						vector<int> goldPos;
						goldPos.push_back(row);
						goldPos.push_back(col);
						gold[i]->setItemPos(goldPos);							// set gold position
						board[row][col].setSymbol('$');							// notify board
						break;
					}
				}
			}
			enemies[i] = NULL;													// set enemy to NULL(erase enemy)	
			break;
		}
	}	
};

/*****************notifyMerchant()******************
Purpose: all merchant starts to be hostile when attacked by player
***************************************************/
void Map::notifyMerchant(){
	for(int i = 0; i < 20+dragonCount; i++){
		if(enemies[i] && enemies[i]->getSymbol() == 'M'){
			enemies[i]->setHostile(true);										// make them hostile to attack
		}
	}
};

int Map::getFloor(){
	return floor;
};

/*****************goUpstairs()******************
Purpose: go downstairs actually..
		 clear current board, create a new board
***************************************************/
void Map::goUpstairs(){
	floor++;
	cout << "You descend down to floor " << floor << "." << endl;
	clear();
	create();
};

/*****************isOccupied()******************
Purpose: if the cell is occupied, return true, otherwise false.
***************************************************/
static bool isOccupied(Cell *c){
	bool flag = false;
	if(c->getSymbol() != '.') flag = true;
	return flag;
};

/*****************enemyMove()******************
Purpose: enemies random move.
***************************************************/
void Map::enemyMove(Enemy *enemy){
	int row = enemy->getCharPos().at(0);
	int col = enemy->getCharPos().at(1);
	// copy
	vector<Cell*> tmpNeighbours(board[row][col].getNeighbours());
	// add Cell if not occupied, to tmpNeighbours2
	vector<Cell*> tmpNeighbours2;
	for (std::vector<Cell*>::iterator it = tmpNeighbours.begin(); it != tmpNeighbours.end(); ++it) {
		if (!isOccupied(*it)) {
			tmpNeighbours2.push_back(*it);
		}
	}
	int numNeighbours = tmpNeighbours2.size(); 										// if there're positions around enemy not occupied
	if(numNeighbours > 0){
		int pos = prng(numNeighbours-1);											// random choose a positon for enemy to move
		vector<int> enemyPos(tmpNeighbours2.at(pos)->getCellPos());					// get its position
		enemy->setCharPos(enemyPos);												// set new Pos for enemy
		changeBack(row,col);														// change previous enemy cell back to original symbol
    	board[enemyPos.at(0)][enemyPos.at(1)].setSymbol(enemy->getSymbol());		// notify board
	}
};

/*****************doEnemyAction()******************
Purpose:Enemies will move or attack or stay.
***************************************************/
void Map::doEnemyAction(Enemy *enemy){
	vector<int> enemyPos = enemy->getCharPos();										// check if there's player around!
	bool playerIs_Around = board[enemyPos.at(0)][enemyPos.at(1)].playerAround();
	if(playerIs_Around){
		if(enemy->is_hostile())	{													// check if the enemy is hostile to attack!
			enemy->attack(*player);
		}else{
			if(enemy->getSymbol()!='D' && enemy->can_move()) enemyMove(enemy);		// check if the enemy is available to move!
		}
	}else{
		if(enemy->getSymbol()!='D' && enemy->can_move()) {
			if(enemy->getSymbol() == 'g' && drinkPotion(enemy)){					// goblin will auto-drink potion if no player's around
			}else{
				enemyMove(enemy);
			}
		}
	}
};

/*****************drinkPotion()******************
Purpose: goblin will drink potion when there's no player
 		 around.
***************************************************/
bool Map::drinkPotion(Enemy *enemy){
	vector<int> enemyPos = enemy->getCharPos();
	int row = enemyPos.at(0);
	int col = enemyPos.at(1);
	int prow = 0, pcol = 0;
		// get enemy's cell's neighbours
	vector<Cell*> tmpNeighbours(board[row][col].getNeighbours());
	for(std::vector<Cell*>::iterator it = tmpNeighbours.begin(); it != tmpNeighbours.end(); ++it){
		if((*it)->getSymbol() == '!'){												// if there're potion around
			prow = (*it)->getCellPos().at(0);							// store potion information
			pcol = (*it)->getCellPos().at(1);
		}
		for(int i = 0; i < 15; i++){									// find the potion in potion list
			if(potions[i] && potions[i]->getItemPos().at(0)==prow && potions[i]->getItemPos().at(1)==pcol){
				enemy->pickupPotion(potions[i]);						// enemy pick up potions
				potions[i] = NULL;										// erase this potion
				changeBack(prow, pcol);									// change potion cell back to original symbol
				return true;
			}
		}
	}
	return false;
};

/*****************routine()******************
Purpose: loop around actions on each enemy
		 Also other regular check and modification after each turn.
***************************************************/
void Map::routine(){
	player->gainHP();										// player gain HP
	for(int i = 0; i < dragonCount+20; i++){				// do enemy action for every non-NULL enemey
		if(enemies[i]) doEnemyAction(enemies[i]);
	}
    turns++;
   	player->check();										// return all stats of player back to normal situation
   	if(player->die()){										// check if player dies.
   		dead = true;
   	}	
};

/*****************playerMove()******************
Purpose: Reads in direction and move accordingly.
***************************************************/
void Map::playerMove(string direction){
    int row = player->getCharPos().at(0);
    int col = player->getCharPos().at(1);
    int modifyRow = 0;
    int modifyCol = 0;										// give modify to row and col according to given instruction
    if(direction == "nw" || direction == "we" || direction == "sw") modifyCol = -1;
    if(direction == "ne" || direction == "ea" || direction == "se") modifyCol = 1;
    if(direction == "nw" || direction == "no" || direction == "ne") modifyRow = -1;
    if(direction == "sw" || direction == "so" || direction == "se") modifyRow = 1;
    // move
    if(validMove(row+modifyRow, col+modifyCol)){					// if the cell is valid to move
    	changeBack(row, col);										// change previous player postion back to original symbol
    	board[row+modifyRow][col+modifyCol].setSymbol('@');			// notify board
		playerPos.at(0) = row+modifyRow;
		playerPos.at(1) = col+modifyCol;
		player->setCharPos(playerPos);								// set player's new position
		cout << "You move ";
		if(direction == "nw") cout << "north-west.";
		if(direction == "we") cout << "west.";
		if(direction == "sw") cout << "south-west.";
		if(direction == "ne") cout << "north-east.";
		if(direction == "ea") cout << "east.";
		if(direction == "se") cout << "south-east.";
		if(direction == "no") cout << "north.";
		if(direction == "so") cout << "south.";
		cout << endl;
		routine();													// do routine after every turn
		print();													// print board and stats
    }else{
    	// Pick up Gold or get downstairs.
    	if(getSymbol(row+modifyRow, col+modifyCol) == '$'){			// pickup gold
    		eraseGold(row+modifyRow, col+modifyCol);
    		routine();												// do routine after every Turn
    		print();												// print board and stats
    	}else if(getSymbol(row+modifyRow, col+modifyCol) == '>'){
    		if(floor != 5){											// go downstaris
				goUpstairs();
			}else{
				won = true;											// win
			}
    	}else{														// can't move in that direction
    		cout << "You can't move in that direction." << endl;
    	}
    }
};

/*****************playerAttack()******************
Purpose: attack according to given direction.
***************************************************/
void Map::playerAttack(string direction){
	int row = player->getCharPos().at(0);
    int col = player->getCharPos().at(1);
    int modifyRow = 0;
    int modifyCol = 0;												// get modification info on row and column
    if(direction == "nw" || direction == "we" || direction == "sw") modifyCol = -1;
    if(direction == "ne" || direction == "ea" || direction == "se") modifyCol = 1;
    if(direction == "nw" || direction == "no" || direction == "ne") modifyRow = -1;
    if(direction == "sw" || direction == "so" || direction == "se") modifyRow = 1;
    char c = getSymbol(row+modifyRow,col+modifyCol);
    	// when there is enemy to attack
    if(c=='D' || c=='X' || c=='g' || c=='M' || c=='O'){
    	for(int i = 0; i < 20+dragonCount; i++){								// find The enemy in enemy list.
    		if(enemies[i] && (enemies[i]->getCharPos().at(0) == row+modifyRow) &&
    			(enemies[i]->getCharPos().at(1) == col+modifyCol)){
    			if(enemies[i]->getSymbol() == 'M') {							// notify merchants
    				cout << "This is an act of war to every Merchant!" << endl;
    				notifyMerchant();	
    			}
    			player->attack(*enemies[i]);									// player attack(modify enemy's stats)																
    			if(enemies[i]->getHP() == 0) {									// if enemy dies.
    				cout << "The " << enemies[i]->getName() << " has died." << endl;
    				eraseEnemy(row+modifyRow,col+modifyCol);
    			}
    			break;
    		}
    	}
    	routine();																// do routine after every turn
    	print();																// print board and stats
    }else{
    	cout << "There is nothing there to attack!" << endl;
    }
};

/*****************playerUse()******************
Purpose: Pick up items according to given direction.
***************************************************/
void Map::playerUse(string direction){
    int row = player->getCharPos().at(0);
    int col = player->getCharPos().at(1);
    int modifyRow = 0;
    int modifyCol = 0;															// get modification on row and column
    if(direction == "nw" || direction == "we" || direction == "sw") modifyCol = -1;
    if(direction == "ne" || direction == "ea" || direction == "se") modifyCol = 1;
    if(direction == "nw" || direction == "no" || direction == "ne") modifyRow = -1;
    if(direction == "sw" || direction == "so" || direction == "se") modifyRow = 1;
    char c = getSymbol(row+modifyRow,col+modifyCol);
    if(c == '!'){																// pick up potion
    	erasePotion(row+modifyRow,col+modifyCol);
    	routine();
    	print();
    }else if(c == '$'){															// pick up gold pile or dragon hoard
    	eraseGold(row+modifyRow,col+modifyCol);
      	routine();
    	print();
    }else{
    	cout << "There is nothing there to use!" << endl;
    }
};

/*****************print()******************
Purpose: print map and stats.
***************************************************/
void Map::print(){
  	for(int i = 0; i < 25; i++) {
  	  	for(int j = 0; j < 79; j++) {
      		cout << board[i][j].getSymbol();
    	}
    	cout << endl;
  	}	
  	stringstream convert;									// define 5 stringstream to convert integer into string
  	stringstream convert2;
  	stringstream convert3;
  	stringstream convert4;
  	stringstream convert5;

  	string name = player->getName();
  	name.resize(12, ' ');

  	int g = player->getGold();
  	convert << g;
  	string gold = convert.str();
  	gold.resize(15, ' ');

  	int hp = player->getHP();
  	convert2 << hp;
  	int mhp = player->getMaxHP();
  	convert3 << mhp;
  	string HP = convert2.str()+"/"+convert3.str();
  	HP.resize(15, ' ');

  	int a = player->getAtk();
  	convert4 << a;
  	string atk = convert4.str();
  	atk.resize(14, ' ');

  	int d = player->getDef();
  	convert5 << d;
  	string def = convert5.str()+"%";
  	def.resize(8, ' ');

  	cout << "        Class: " << name;
  	cout << "GP: " << gold;
  	cout << "Floor " << floor << endl;
 	cout << "        HP: " << HP;
 	cout << "Atk: " << atk;
 	cout << "Def: " << def;
 	cout << "Turn: " << turns << endl;
 	cout << "What will you do?" << endl;
};
