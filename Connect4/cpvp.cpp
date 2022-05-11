#include "cpvp.h"

//=============================================================================

CPvP::CPvP(string name1, string name2, char starter) // constructor
{
    board = new CBoard();
    player_name1 = name1; // 'U'
    player_name2 = name2; // 'C'
    current_player = starter;
}
CPvP::~CPvP() // destructor
{ delete board; }

//=============================================================================

void CPvP::playGame() // main method
{
    while(board->turn+1 < CBoard::SIZE){
        showGame();
        saveGame();
        if(board->checkWin(playerMakePlay(),4))
            break;
        switchPlayer();

        if(board->turn+1 == board->SIZE)
            current_player = 'T'; // tie/draw
    }
    clearFile();
    showGame();
    gameOver();
}
void CPvP::gameOver() // credits
{
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    switch (current_player) {
    case 'O':
        SetConsoleTextAttribute(hConsole, 111);
        cout << endl << "Yellow won!";
        saveScore(false, player_name2);
        saveScore(true, player_name1);
        break;
    case '0':
        SetConsoleTextAttribute(hConsole, 79);
        cout << endl << "Red won!";
        saveScore(true, player_name2);
        saveScore(false, player_name1);
        break;

    case 'T': cout << endl << "It's a tie!";
    }

    SetConsoleTextAttribute(hConsole, 15);
    cout << endl << endl << "THANKS FOR PLAYING!" << endl
         << "Game developed by:" << endl
         << "Ines Ferreira & Andre Neto" << endl << endl;
    system("pause");
}

//=============================================================================

void CPvP::showGame() // shows player + turn + board
{
    system("cls");
    switch(current_player){
    case 'O': cout << player_name1 << "'s turn"
                   << endl << "Turn " << board->turn << endl; break;
    case '0': cout << player_name2 << "'s turn"
                   << endl << "Turn " << board->turn << endl;
    }

    board->printBoard();
}
void CPvP::switchPlayer() // switches between players
{
    if (current_player == 'O')
        current_player = '0';
    else
        current_player = 'O';
}
int CPvP::playerMakePlay(){
    int play;
    char cplay;
    int t = board->turn;
    do{
        cout << "Pick a collumn [1..7]: ";
        cin >> cplay;
        if (cplay == '-'){
            if(board->turn >= 1){
                board->undoPiece(1);
                switchPlayer();
                showGame();
            }
        }
        if (cplay == '+'){
            if(board->turn < t){
                board->advancePiece(1);
                switchPlayer();
                showGame();
            }
        }
        if (cplay == '0'){
            exit(0);
        }

        play = (int)cplay - 48 - 1;
    }while (play >= CBoard::MAX_C || play < 0 || board->getRow(play) == -1);

    board->playPiece(play, current_player);
    return play;
}

//=============================================================================

void CPvP::saveGame() // saves game in case of unexpected shutdown
{
    ofstream WriteFile("savedgame_pvp.txt");
    for (int t = 0; t < CBoard::SIZE; t++)
        for (int r = 0; r < CBoard::MAX_R; r++)
            for (int c = 0; c < CBoard::MAX_C; c++)
                WriteFile << board->boards[t][r][c];

    WriteFile << endl << board->turn
              << endl << current_player
              << endl << player_name1
              << endl << player_name2;

    WriteFile.close();
}
void CPvP::clearFile () const // clears saved game after its over
{
    ofstream MyFile("savedgame_pvp.txt");
    MyFile.close();
}
bool CPvP::loadGame() // resume game, returns false if there is no game to resume
{

    ifstream MyReadFile("savedgame_pvp.txt");
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
    player_name1 = tmpString;

    getline(MyReadFile, tmpString); // gets player name2
    player_name2 = tmpString;

    MyReadFile.close();
    return true; // successful
}

//=============================================================================

struct CPvP::stats{
    string name;
    int win;
    int loss;
};
void CPvP::swap(stats *xp, stats *yp) // selection sort: score = wins - losses
{
    stats temp = *xp;
    *xp = *yp;
    *yp = temp;
}
void CPvP::selectionSort(stats arr[], int n){
    for(int i = 0; i < n-1; i++){
        int min_idx = i;
        for (int j = i+1; j < n; j++)
            if ((arr[j].win - arr[j].loss)  > (arr[min_idx].win - arr[min_idx].loss))
                min_idx = j;
        swap(&arr[min_idx], &arr[i]);
    }
}
void CPvP::saveScore(bool is_win, string name) // saves player score to player vs machine leaderboards
{

    ifstream ReadFile("leaderboard_pvp.txt");
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
        if (all[i].name == name)
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
        all[total].name = name;
        if (is_win) all[total].win++;
        else all[total].loss++;
        total++;
    }

    selectionSort(all, total);
    // put ordered leaderboard in file
    ofstream WriteFile("leaderboard_pvp.txt");
    WriteFile << total;
    for(int i = 0; i < total; i++)
        WriteFile << endl << all[i].name << endl << all[i].win << endl << all[i].loss;
    WriteFile.close();

}
void CPvP::leaderboard() const // show top 10
{

    string line;
    ifstream ReadFile("leaderboard_pvp.txt");
    if(ReadFile.peek() == ifstream::traits_type::eof())
        return; // checks if its empty
    getline(ReadFile, line);

    int n_players, total_players = stoi (line);
    if (total_players < 10) n_players = total_players;
    else n_players = 10;

    cout << "     --PLAYER VS PLAYER LEADERBOARD--" << endl << endl;
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
