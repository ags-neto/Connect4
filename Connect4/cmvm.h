#include <iostream>
#include <fstream>
#include <ctime>

#ifndef CMVM_H
#define CMVM_H

#include "cboard.h"
using namespace std;

class CMvM
{
private:
    CBoard *board;
    char current_player;
    int difficulty1;
    int difficulty2;

public:
    CMvM(int = 1, int = 1, char = 'O');
    ~CMvM();

    // GAME
    void playGame(); // main method
    void gameOver(); // roll the credits
    void showGame() const; // show game events
    void switchPlayer(); // switch between players

    // PLAYERS MECHANINCS
    int computerMakePlay();

    // ALGORITHMS
    int randomPlay(); // returns random play
    int winNextPlay(); // returns next win move, if not, -1
    int defendNextPlay() ; // returns next defensive move, if not, -1
    int bestPlay(); // returns the best play possible

    // FILES
    void saveGame() const; // saves game into file
    bool loadGame(); // loads game from file (if any)
    void clearFile() const; // clears file



};

#endif // CMVM_H
