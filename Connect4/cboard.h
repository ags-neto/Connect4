#include <iostream>
#include <cstring>
#include <windows.h>
using namespace std;

#ifndef CBOARD_H
#define CBOARD_H

class CBoard{
    friend class CPvM;
    friend class CPvP;
    friend class CMvM;

private:
    char ***boards;
    int turn;

public:
    constexpr static const int MAX_C = 7;
    constexpr static const int MAX_R = 6;
    constexpr static const int SIZE = MAX_R*MAX_C + 1;

    CBoard();
    ~CBoard();

    int getRow(int) const;

    void playPiece(int,char);
    void undoPiece(int);
    void advancePiece(int);

    bool checkWin(int,int) const;
    bool simulateWin(int,char);

    void printBoard() const;
    void printBoardAdvanced() const;
};

#endif // CBOARD_H
