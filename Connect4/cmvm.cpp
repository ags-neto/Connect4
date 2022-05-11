#include "cmvm.h"

//=============================================================================

CMvM::CMvM(int dif1, int dif2, char starter) // constructor
{
    board = new CBoard();
    current_player = starter;
    difficulty1 = dif1;
    difficulty2 = dif2;
}
CMvM::~CMvM() // destructor
{ delete board; }

//=============================================================================

void CMvM::playGame() // main method
{
    while(board->turn+1 < CBoard::SIZE){
        showGame();
        saveGame();
        if(board->checkWin(computerMakePlay(),4))
            break;
        switchPlayer();
        system("pause");

        if(board->turn+1 == CBoard::SIZE)
            current_player = 'T'; // tie/draw
    }
    clearFile();
    showGame();
    gameOver();
}
void CMvM::gameOver() // credits
{
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    switch (current_player) {
    case '0':
        SetConsoleTextAttribute(hConsole, 79);
        cout << endl << "Red won!"; break;
    case 'O':
        SetConsoleTextAttribute(hConsole, 111);
        cout << endl << "Yellow won!"; break;
    case 'T': cout << endl << "It's a tie!";
    }

    SetConsoleTextAttribute(hConsole, 15);
    cout << endl << endl << "THANKS FOR PLAYING!" << endl
         << "Game developed by:" << endl
         << "Ines Ferreira & Andre Neto" << endl << endl;
    system("pause");
}

//=============================================================================

void CMvM::showGame() const // machine + difficulty + turn + board
{
    system("cls");
    switch(current_player){
    case 'O':
        cout << "Machine 1's turn ";

        switch (difficulty1){
        case 1: cout << "(Easy)"; break;
        case 2: cout << "(Medium)"; break;
        case 3: cout << "(Hard)"; break;
        case 4: cout << "(Extreme)"; break;
        }break;

    case '0':
        cout << "Machine 2's turn ";

        switch (difficulty2){
        case 1: cout << "(Easy)"; break;
        case 2: cout << "(Medium)"; break;
        case 3: cout << "(Hard)"; break;
        case 4: cout << "(Extreme)"; break;
        }break;
    }

    cout << endl << "Turn: " << board->turn << endl;

    board->printBoard();
}
void CMvM::switchPlayer()  //  switches between players
{
    if (current_player == 'O')
        current_player = '0';
    else
        current_player = 'O';
}
int CMvM::computerMakePlay() // machine's turn
{

    int play = -1;
    int dif=0;
    switch(current_player){
    case 'O': dif=difficulty1;break;
    case '0': dif=difficulty2;
    }

    switch (dif){
    case 1: // easy
        play = randomPlay();
        break;

    case 2: // medium
        play = winNextPlay();
        if (play == -1)
            play = randomPlay();
        break;

    case 3: // hard
        play = winNextPlay();
        if (play == -1)
            play = defendNextPlay();
        if (play == -1)
            play = randomPlay();
        break;

    case 4: // extreme
        play = winNextPlay();
        if (play == -1)
            play = defendNextPlay();
        if (play == -1)
            play = bestPlay();
        break;
    }

    board->playPiece(play, current_player);
    return play;
}

//=============================================================================

int CMvM::randomPlay() // return a valid random play
{
    int play;
    do{
        srand (time(NULL));
        play = rand() % 7;
    } while (board->getRow(play) == -1);
    return play;
}
int CMvM::winNextPlay() // return winning play, if not -1
{
    for (int col=0; col<CBoard::MAX_C; col++)
        if(board->getRow(col) != -1)
            if(board->simulateWin(col, '0'))
                return col;
    return -1;
}
int CMvM::defendNextPlay() // return defending play, if not -1
{
    for (int col=0; col<CBoard::MAX_C; col++)
        if(board->getRow(col) != -1)
            if(board->simulateWin(col, 'O'))
                return col;
    return -1;
}
int CMvM::bestPlay(){

    int score[7] = {0,0,0,0,0,0,0};

    for (int col = 0; col < CBoard::MAX_C; col++){
        if (board->getRow(col) != -1){
            board->playPiece(col, '0');
            if (col == 0 || col == 6) score[col]--;
            if (col == 2 || col == 4) score[col]++;
            if (col == 3) score[col] += 2;
            for (int s = 2; s < 4; s++)
                score[col] += s*board->checkWin(col,s);
            board->undoPiece(1);
        }
    }

    int max = score[0];
    int maxIndex = 0;

    for (int i = 1; i < 7; i++) {
        if (score[i] > max) {
            maxIndex = i;
            max = score[i];
        }
    }

    return maxIndex;
}

//=============================================================================

void CMvM::saveGame() const // saves game in case of unexpected shutdown
{
    ofstream WriteFile("savedgame_mvm.txt");
    for (int t = 0; t < CBoard::SIZE; t++)
        for (int r = 0; r < CBoard::MAX_R; r++)
            for (int c = 0; c < CBoard::MAX_C; c++)
                WriteFile << board->boards[t][r][c];

    WriteFile << endl << board->turn
              << endl << current_player
              << endl << difficulty1
              << endl << difficulty2;

    WriteFile.close();
}
void CMvM::clearFile () const // clears saved game after its over
{
    ofstream MyFile("savedgame_mvm.txt");
    MyFile.close();
}
bool CMvM::loadGame() // resume game, returns false if there is no game to resume
{

    ifstream MyReadFile("savedgame_mvm.txt");
    if(MyReadFile.peek() == ifstream::traits_type::eof())
        return false; // checks if its empty

    string tmpString;
    getline(MyReadFile, tmpString); // gets board
    int i = 0;
    for(int t = 0; t < CBoard::SIZE; t++)
        for(int r = 0; r < CBoard::MAX_R; r++)
            for(int c = 0; c < CBoard::MAX_C; c++)
                board->boards[t][r][c] = tmpString[i++];

    getline(MyReadFile, tmpString); // gets turn
    board->turn = stoi(tmpString);

    getline(MyReadFile, tmpString); // gets current_player
    current_player = tmpString[0];

    getline(MyReadFile, tmpString); // gets player name1
    difficulty1 = stoi(tmpString);

    getline(MyReadFile, tmpString); // gets player name2
    difficulty2 = stoi(tmpString);

    MyReadFile.close();
    return true; // successful
}

//=============================================================================
