#ifndef MAP_H
#define MAP_H
#include "game.h"
#include "gold.h"
#include "player.h"
#include "cell.h"
#include "enemy.h"
#include "potion.h"
using namespace std;

static const int CHAMBER = -1;

class Game;
class Player;
class Enemy;
class Map {
	Player *player;
	Cell** board;

    Potion *potions[15];			// Objects on board
    Gold *gold[15];
    Enemy *enemies[25];

    char* infile;

    int floor;
    int turns;
    int dragonCount;
    bool dead;
    bool won;
    bool can_move;									 // for testing purposes: stopWander
public:
	Map(string player, char* infile);									// map ctor, takes a player choice and input file
	~Map();																// dtor
	void create();														// create a new board
	void clear();														// clear board, except for player

	int getFloor();
	void goUpstairs();
	bool isDead();														// check if player has died
	bool isWon();														// check if player has won
	int getGold();														// get player's GP
	void routine();									// do enemy random move, attack, pickup, do player gainHP and turn after each turn

	void generatePotions();												// random generate potions
    void generateGold();												// random gnerate gold
    void generateEnemies();												// random generate enemies
	vector<int> randomPos(int chamber = CHAMBER);						// gives a random Position, CHAMBER for choosing stair Pos
	Potion *randomPotion();												// when Grid Bug dies, drop random potion

    bool tile(int row, int col);										// if a cell is a valid tile
    bool passages(int row, int col);									// if a cell is a valid passage
    bool doorway(int row, int col);										// if a cell is a valid doorway
    bool validMove(int row, int col);									// if this cell is a valid move for a player

    char getSymbol(int row, int col);									// get symbol for cell
 	void changeBack(int row, int col);									// change back the cell to its original symbol
 	void eraseGold(int row, int col);									// erase gold on map
 	void erasePotion(int row, int col);									// erase potion on map
 	void eraseEnemy(int row, int col);									// erase enemy on map

 	void doEnemyAction(Enemy *enemy);									// do movement/attack for every enemy
 	void notifyMerchant();												// notify merchant 

	void playerMove(string direction);									// make player move
	void playerAttack(string direction);								// make player attack
	void playerUse(string direction);									// make player pickup items

	void enemyMove(Enemy *enemy);										// enemy random movement
	bool drinkPotion(Enemy *enemy);										// goblin drinks potion

	void stopWander();													// stop all enemies movement

	void print();														// print map and stats
};


#endif
