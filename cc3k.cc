#include <iostream>
#include <string>
#include <sstream>
#include <cstdlib>
#include <fstream>
#include <sys/types.h>
#include <unistd.h>
#include "PRNG.h"
#include "game.h"
using namespace std;

PRNG prng;

extern ifstream ifs;

void beginning(){
	cout << "Welcome to ChamberCralwer3000!" << endl;
	cout << "What would you like to play as today?" << endl;
	cout << "w) Wizard" << endl;
	cout << "k) Knight" << endl;
	cout << "s) Samurai" << endl;
}

void playerChoice(string choice){
	cout << "You have chosen to play as a ";
	if(choice == "w") cout << "Wizard";
	if(choice == "k") cout << "Knight";
	if(choice == "s") cout << "Samurai";
	cout << ". Good luck." << endl;
}

void quit_f(){
	cout << "Are you sure you want to quit? (y/n)" << endl;
}

void quit_l(){
	cout << "You have chosen to exit. At least you tried." << endl;
}

void restart(){
	cout << "Are you sure you want to restart? (y/n)" << endl;
}

void win(Game *game){
	cout << "At long last, you have outmatched the Great Cavernous Chambers.";
	cout << " Great things await you." << endl;
	cout << "You achieved a score of " << game->playerGP() << "." << endl;
	cout << "Play again? (y/n)" << endl;
}

void lose(Game *game){
	cout << "You have been bested by the Great Cavernous Chambers. Good luck next time!" << endl;
	cout << "You achieved a score of " << game->playerGP() << "." << endl;
	cout << "Play again? (y/n)" << endl;
}

/*****************cc3k******************
Purpose: Reads in optional command line arguments, 
		 and prints corresponding map and stats.
		 Interating game, user can key in instructions,
		 and this game will interact with player.
***************************************************/
int main(int argc, char *argv[]){
	int integer = 0;
	char *infile = NULL;
	Game *game;

	int i = 1;
	while(i < argc){											// read in command line arguments
		stringstream ss(argv[i]);
		if(ss >> integer){
		}else{
			infile = argv[i];
		}
		i++;
	}
	if(integer != 0) {											// give a random seed when not specified.
		prng.seed(integer);
	}else{
		prng.seed(::getpid());
	}

	beginning();
	string choice;
	while(cin >> choice){										// reads in player's character choice.
		if(choice == "w" || choice == "s" || choice == "k") break;
		cout << "Did not recognize input." << endl;
	}
	game = new Game(choice, infile);

	bool flag = true;
	while(!cin.eof()){											// reads in instructions
		string instr;
		string yes_no;
		cin >> instr;
			if(instr == "nw" || instr == "no" || instr == "ne" ||
				instr == "we" || instr == "ea" ||
				instr == "sw" || instr == "so" || instr == "se"){
				game->move(instr);								// move
			}else if(instr == "a"){
				cin >> instr;
				game->attack(instr);							// attack
			}else if(instr == "u"){
				cin >> instr;
				game->use(instr);								// pick up
			}else if(instr == "q"){
				quit_f();
				cin >> yes_no;
				if(yes_no == "y"){
					quit_l();
					break;										// quit game
				}else if(yes_no == "n"){
					continue;
				}else {
					cout << "Did not recognize input." << endl;
				}
			}else if(instr == "r"){
				restart();										// restart game
				cin >> yes_no;
				if(yes_no == "y"){
					delete game;			
					ifs.close();
					beginning();
					cin >> choice;
					game = new Game(choice, infile);
				}else if(yes_no =="n"){
					continue;
				}else{
					cout << "Did not recognize input." << endl;
				}
			}else if(instr == "stopdeath"){								// test
				cout << "Player death can no longer occur." << endl;
				flag = false;
				game->print();
			}else if(instr == "stopwander"){										// test
				cout << "Enemy actions that would result in movement now do nothing instead." << endl;
				game->stopWander();
				game->print();
			}else{
				cout << "Did not recognize input." << endl;
			}
			if(flag){												// check if player dies after each turn
				if(game->isDead()){
					lose(game);
					cin >> yes_no;
					if(yes_no == "y"){
						delete game;
						ifs.close();
						beginning();
						cin >> choice;
						game = new Game(choice, infile);
					}else if(yes_no == "n"){
						break;
					}else{
						cout << "Did not recognize input." << endl;
					}
				}
			}
			if(game->isWon()){											// check if player won after each turn
				win(game);
				cin >> yes_no;
				if(yes_no == "y"){
					delete game;
					ifs.close();
					beginning();
					cin >> choice;
					game = new Game(choice, infile);
				}else if(yes_no == "n"){
					break;
				}else{
					cout << "Did not recognize input." << endl;
				}
			}
	}
	ifs.close();											// while input reaches EOF, close input file.
	delete game;
	return 0;
};
