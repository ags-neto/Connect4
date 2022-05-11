#include <iostream>
#include <fstream>
#include <iomanip>
#include <ctime>

#ifndef CPVM_H
#define CPVM_H

#include "cboard.h"
using namespace std;

class CPvM
{
private:
    CBoard *board;
    string player_name;
    char current_player;
    int difficulty;

public:
    CPvM(string = "Guest", int = 1, char = 'O');
    ~CPvM();

    // GAME
    void playGame(); // main method
    void gameOver(); // roll the credits
    void showGame() const; // show game events
    void switchPlayer(); // switch between players

    // PLAYERS MECHANINCS
    int makePlay(); // calls playerMakePlay() or computerMakePlay()
    int playerMakePlay(); // returns & plays a valid move from player
    int computerMakePlay(); // returns & plays a valid move from computer

    // ALGORITHMS
    int randomPlay(); // returns random play
    int winNextPlay(); // returns next win move, if not, -1
    int defendNextPlay(); // returns next defensive move, if not, -1
    int bestPlay(); // returns the best play possible

    // FILES
    void saveGame()const; // saves game into file
    void clearFile() const; // clears file
    bool loadGame(); // loads game from file (if any)

    struct stats;
    void swap(stats *, stats *);
    void selectionSort(stats [], int);
    void saveScore(bool);
    void leaderboard() const;};

#endif // CPVM_H
