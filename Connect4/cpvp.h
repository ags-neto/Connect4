#include <iostream>
#include <fstream>
#include <iomanip>

#ifndef CPVP_H
#define CPVP_H

#include "cboard.h"
using namespace std;

class CPvP
{
private:
    CBoard *board;
    string player_name1;
    string player_name2;
    char current_player;

public:
    CPvP(string = "Guest1", string = "Guest2", char = 'O');
    ~CPvP();

    // GAME
    void playGame(); // main method
    void gameOver(); // roll the credits
    void showGame(); // show player turn, board
    void switchPlayer(); // switch between players

    // PLAYERS MECHANINCS
    int playerMakePlay(); // returns & plays a valid move from player

    // FILES
    void saveGame(); // saves game into file
    bool loadGame(); // loads game from file (if any)
    void clearFile() const; // clears file

    struct stats;
    void swap(stats *, stats *);
    void selectionSort(stats [], int);
    void saveScore(bool, string);
    void leaderboard() const;
};


#endif // CPVP_H
