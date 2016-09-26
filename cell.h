#ifndef CELL_H
#define CELL_H
#include <iostream>
#include <vector>
//#include "game.h"
//#include "display.h"
using namespace std;

static const int MAX_NEIGHBOURS = 8;

class Cell {
protected:
    char symbol;
    char my_symbol;

    vector<int> cellPos;
    int numNeighbours;
    vector<Cell*> neighbours;
public:
        Cell();
        ~Cell();

        void addNeighbour(Cell *neighbour);
        int getNumNeighbours();
        vector<Cell*> getNeighbours();
        vector<int> getCellPos();
        void setCoords(int row, int col);
        void setOriSymbol(char symbol);
        void setSymbol(char symbol);
        char getSymbol();
        char getMySymbol();
        bool playerAround();         // check if player is around
        int getChamber();
//      void showNeighbours();

};

#endif
