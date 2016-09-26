#ifndef CHARACTER_H
#define CHARACTER_H
#include <iostream>
#include <vector>
using namespace std;

class Character {
    protected:
        vector<int> charPos;
        int cur_HP;
        int max_HP;
        int cur_atk;
        int max_atk;
        int cur_def;
        int max_def;
        string Name;
        char symbol;    // for single alphabet

    public:
        Character(string Name, int HP, int atk, int def, char symbol);
        virtual ~Character();

        int getHP();
        int getAtk();
        int getDef();
        int getMaxHP();

        int changeHP(int change);           // change character HP when being attacked.

        void resetAtk();       // get into another floor
        void resetDef();

        string getName();
        char getSymbol();

        void setCharPos(vector<int> charPos);
        vector<int> getCharPos();
};

#endif
