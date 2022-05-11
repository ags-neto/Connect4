#include "cboard.h"

//=============================================================================

CBoard::CBoard() // constructor
{
    //Dynamically allocate memory for 3D array
    boards = new char**[SIZE];
    for (int s = 0; s < SIZE; s++){
        boards[s] = new char*[MAX_R];
        for (int r = 0; r < MAX_R; r++)
            boards[s][r] = new char[MAX_C];
    }
    // assign value ' ' to the allocated memory
    for (int s = 0; s < SIZE; s++)
        for (int c = 0; c < MAX_C; c++)
            for (int r = 0; r < MAX_R; r++)
                boards[s][r][c] = ' ';
    turn = 0;
}
CBoard::~CBoard() // destructor
{
    // deallocate 3D array
    for (int s = 0; s < SIZE; s++){
        for (int r = 0; r < MAX_R; r++)
            delete []boards[s][r];
        delete []boards[s];
    }
    delete []boards;
}

//=============================================================================

// returns closest free (' ') row of col to the bottom, if not, -1
int CBoard::getRow(int col) const{
    for(int r = MAX_R-1; r >= 0; r--)
        if(boards[turn][r][col] == ' ')
            return r;
    return -1;
}

//=============================================================================

// puts player in the col and increments turn
void CBoard::playPiece(int col, char player) {
    for(int r = 0; r < MAX_R; r++)
        for(int c = 0; c < MAX_C; c++)
            boards[turn+1][r][c] = boards[turn][r][c];
    boards[++turn][getRow(col)][col] = player;
}
// undoes n turns
void CBoard::undoPiece(int undo){
    turn -= undo;
}
// advances n turns
void CBoard::advancePiece(int adv){
    turn += adv;
}

//=============================================================================

// gets the player who played on that col, checks for a win
bool CBoard::checkWin(int col, int num) const{
    int row = getRow(col)+1;
    char player = boards[turn][row][col];

    // checks horizontal
    for (int c = 0, count = 0; c < MAX_C; c++){
        if (boards[turn][row][c] == player)
            count ++;
        else count = 0;

        if (count == num)
            return true;
    }

    // checks vertical
    for (int r = 0, count = 0; r < MAX_R; r++){
        if (boards[turn][r][col] == player)
            count ++;
        else count = 0;

        if (count == num)
            return true;
    }

    // checks diagonal NE
    // left half
    for(int i=3, count=0; i <= 5; i++, count=0)
        for(int r=i, c=0; r >= 0; r--, c++){
            if (boards[turn][r][c] == player) count++;
            else count = 0;

            if (count == num) return true;
        }
    // right half
    for(int i=2, count=0; i >= 0; i--, count=0)
        for(int r=i, c=6; r <= 5; r++, c--){
            if (boards[turn][r][c] == player) count++;
            else count = 0;

            if (count == num) return true;
        }


    // checks diagonal SE
    // left half
    for(int i=3, count=0; i <= 5 ; i++, count=0)
        for(int c=i, r=5; c >= 0 ; c--, r--){
            if (boards[turn][r][c] == player) count++;
            else count = 0;

            if (count == num) return true;
        }
    // right half
    for(int i=3, count=0; i > 0; i--, count=0)
        for(int c=i, r=0; c <= 6 ; c++, r++){
            if (boards[turn][r][c] == player) count++;
            else count = 0;

            if (count == num) return true;
        }

    return false;
}
// simulate a play and checks if it's a win
bool CBoard::simulateWin(int col, char player){
    playPiece(col, player);
    bool win = checkWin(col, 4);
    undoPiece(1);
    return win;
}

//=============================================================================

// prints board all fancy
void CBoard::printBoard() const{
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hConsole, 15);

    cout << "| 1 | 2 | 3 | 4 | 5 | 6 | 7 |" << endl;
    for(int r = 0; r < MAX_R; r++){
        for(int c = 0; c < MAX_C; c++){
            cout << "| ";
            switch (boards[turn][r][c]){
            case '0': SetConsoleTextAttribute(hConsole, 4); break;
            case 'O': SetConsoleTextAttribute(hConsole, 6); break;
            }
            cout << boards[turn][r][c] << " ";
            SetConsoleTextAttribute(hConsole, 15);
        }
        cout << "|" << endl;
    }
}
// TODO
void CBoard::printBoardAdvanced() const{

    for(int header = 0; header <= turn; header++)
        cout << "| 1 | 2 | 3 | 4 | 5 | 6 | 7 |         " ;
    cout << endl;

    for(int r = 0 ; r < MAX_R; r++){
        for(int s = turn; s >= 0; s--){
            for(int c = 0 ; c < MAX_C; c++)
                cout << "| " << boards[s][r][c] << " ";
            cout << "|         ";
        }
        cout << endl;
    }
    cout << endl;
}

//=============================================================================
