#include "cpvm.h"

//=============================================================================

CPvM::CPvM(string name, int dif, char starter) // constructor
{
    board = new CBoard();
    current_player = starter;
    player_name = name;
    difficulty = dif;
}
CPvM::~CPvM() // destructor
{ delete board; }

//=============================================================================

void CPvM::playGame() // main method
{
    while(board->turn+1 < board->SIZE){
        showGame();
        saveGame();
        if(board->checkWin(makePlay(),4))
            break;
        switchPlayer();

        if(board->turn+1 == board->SIZE)
            current_player = 'T'; // tie/draw
    }
    clearFile();
    showGame();
    gameOver();
}
void CPvM::gameOver() // credits
{
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    switch (current_player) {
    case '0':
        SetConsoleTextAttribute(hConsole, 79);
        cout << endl << "Red won!";
        saveScore(false); break;
    case 'O':
        SetConsoleTextAttribute(hConsole, 111);
        cout << endl << "Yellow won!";
        saveScore(true); break;
    case 'T': cout << endl << "It's a tie!";
    }

    SetConsoleTextAttribute(hConsole, 15);
    cout << endl << endl << "THANKS FOR PLAYING!" << endl
         << "Game developed by:" << endl
         << "Ines Ferreira & Andre Neto" << endl << endl;
    system("pause");
}
void CPvM::showGame() const // shows player + turn + board
{
    system("cls");
    cout << "Difficulty: ";
    switch (difficulty) {
    case 1: cout << "Easy"; break;
    case 2: cout << "Medium"; break;
    case 3: cout << "Hard"; break;
    case 4: cout << "Extreme"; break;
    }
    cout << endl << "Turn " << board->turn << endl;
    board->printBoard();
}
void CPvM::switchPlayer() // switches between players
{
    if (current_player == 'O')
        current_player = '0';
    else
        current_player = 'O';
}

//=============================================================================

int CPvM::makePlay() // decides who makes the play
{
    int play;
    switch (current_player){
    case 'O': play = playerMakePlay(); break;
    case '0': play = computerMakePlay(); break;
    }
    board->playPiece(play, current_player);
    return play;
}
int CPvM::playerMakePlay() // player's turn
{
    int play;
    char cplay;
    int t = board->turn;
    do{
        cout << "Pick a collumn [1..7]: ";
        cin >> cplay;
        if (cplay == '-'){
            if(board->turn >= 2){
                board->undoPiece(2);
                showGame();
            }
        }
        if (cplay == '+'){
            if(board->turn < t){
                board->advancePiece(2);
                showGame();
            }
        }
        if (cplay == '0'){
            exit(0);
        }

        play = (int)cplay - 48 - 1;
    }while (play >= CBoard::MAX_C || play < 0 || board->getRow(play) == -1);

    return play;
}
int CPvM::computerMakePlay() // machine's turn
{
    int play = -1;

    switch (difficulty){
    case 1: // easy
        play = randomPlay();
        break;

    case 2: // medium
        play = winNextPlay();
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

    return play;
}

//=============================================================================

int CPvM::randomPlay() // return a valid random play
{
    int play;
    do{
        srand (time(NULL));
        play = rand() % 7;
    } while (board->getRow(play) == -1);
    return play;
}
int CPvM::winNextPlay() // return winning play, if not -1
{
    for (int col=0; col<CBoard::MAX_C; col++)
        if(board->getRow(col) != -1)
            if(board->simulateWin(col, '0'))
                return col;
    return -1;
}
int CPvM::defendNextPlay() // return defending play, if not -1
{
    for (int col=0; col<CBoard::MAX_C; col++)
        if(board->getRow(col) != -1)
            if(board->simulateWin(col, 'O'))
                return col;
    return -1;
}
int CPvM::bestPlay(){

    int score[7] = {0,0,0,0,0,0,0};

    for (int col = 0; col < CBoard::MAX_C; col++){
        if (board->getRow(col) != -1){
            board->playPiece(col, '0');
            if (col == 0 || col == 6) score[col] -= 3;
            if (col == 1 || col == 5) score[col] -= 2;
            if (col == 2 || col == 4) score[col] += 0;
            if (col == 3) score[col] += 1;
            for (int s = 1; s < 4; s++)
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

void CPvM::saveGame() const // saves game in case of unexpected shutdown
{
    ofstream WriteFile("savedgame_pvm.txt");
    for (int t = 0; t < CBoard::SIZE; t++)
        for (int r = 0; r < CBoard::MAX_R; r++)
            for (int c = 0; c < CBoard::MAX_C; c++)
                WriteFile << board->boards[t][r][c];

    WriteFile << endl << board->turn
              << endl << current_player
              << endl << player_name
              << endl << difficulty;

    WriteFile.close();
}
void CPvM::clearFile () const // clears saved game after its over
{
    ofstream MyFile("savedgame_pvm.txt");
    MyFile.close();
}
bool CPvM::loadGame() // resume game, return false if no game to resume
{

    ifstream MyReadFile("savedgame_pvm.txt");
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

    getline(MyReadFile, tmpString); // gets player
    player_name = tmpString;

    getline(MyReadFile, tmpString); // gets difficulty
    difficulty = stoi(tmpString);

    MyReadFile.close();
    return true; // successful
}

//=============================================================================

struct CPvM::stats // struct to recieve all stats from file
{
    string name;
    int win;
    int loss;
};
void CPvM::swap(stats *xp, stats *yp)
{
    stats temp = *xp;
    *xp = *yp;
    *yp = temp;
}
void CPvM::selectionSort(stats arr[], int n) // score = wins - losses
{
    for(int i = 0; i < n-1; i++){
        int min_idx = i;
        for (int j = i+1; j < n; j++)
            if ((arr[j].win - arr[j].loss)  > (arr[min_idx].win - arr[min_idx].loss))
                min_idx = j;
        swap(&arr[min_idx], &arr[i]);
    }
}
void CPvM::saveScore(bool is_win) // saves player score to leaderboards
{

    ifstream ReadFile("leaderboard_pvm.txt");
    string line;
    int total=0;

    getline(ReadFile, line);
    if(ReadFile.peek() == ifstream::traits_type::eof()){}
    //total = 0;         // checks if its empty
    else
        total = stoi (line); // total number of players registred

    stats all[total+1];
    all[total].win = 0;
    all[total].loss = 0;
    int found = -1;

    // from file to struct stats
    for (int i = 0; i < total; i++){
        getline(ReadFile, line);
        all[i].name = line;
        getline(ReadFile, line);
        all[i].win = stoi(line);
        getline(ReadFile, line);
        all[i].loss = stoi(line);
        //ReadFile >> all[i].win >> all[i].loss;
        // searches for player name in files
        if (all[i].name == player_name)
            found = i;
    }
    ReadFile.close();

    // if player name already exists
    if (found != -1){
        if (is_win) all[found].win++;
        else all[found].loss++;
    }

    // if player name doesn't exists
    else {
        all[total].name = player_name;
        if (is_win) all[total].win++;
        else all[total].loss++;
        total++;
    }

    selectionSort(all, total);
    // put ordered leaderboard in file
    ofstream WriteFile("leaderboard_pvm.txt");
    WriteFile << total;
    for(int i = 0; i < total; i++)
        WriteFile << endl << all[i].name << endl << all[i].win << endl << all[i].loss;
    WriteFile.close();

}
void CPvM::leaderboard() const // show top 10
{
    string line;
    ifstream ReadFile("leaderboard_pvm.txt");
    if(ReadFile.peek() == ifstream::traits_type::eof())
        return; // checks if its empty
    getline(ReadFile, line);

    int n_players, total_players = stoi (line);
    if (total_players < 10) n_players = total_players;
    else n_players = 10;

    cout << "     --PLAYER VS MACHINE LEADERBOARD--" << endl << endl;
    cout << "Top 10" << setw(20) << "Name" << setw(10) << "Wins" << setw(10) << "Losses" << endl;
    for (int i = 0; i < n_players; i++){
        getline(ReadFile, line);
        cout << setw(6) << i+1 << setw(20) << line;
        getline(ReadFile, line);
        cout << setw(10) << line;
        getline(ReadFile, line);
        cout << setw(10) << line << endl;
    }
    cout << endl;
    ReadFile.close();
}

//=============================================================================
