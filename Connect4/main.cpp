#include <ctime>
#include <iostream>
#include "cpvm.h"
#include "cpvp.h"
#include "cmvm.h"

using namespace std;

void mainMenu();
void newGame();
void showLeaderboard();
void showLeaderboardPvM();
void showLeaderboardPvP();
void commands();
void resumeGame();
string setName();

void newPvM();
void newPvP();
void newMvM();

void resumePvM();
void resumePvP();
void resumeMvM();


int main(){
    mainMenu();
}


void mainMenu(){
    char option;
    do{
        system("cls");
        cout << "Welcome to Connect 4" << endl << endl
             << "1) New Game" << endl
             << "2) Resume Game" << endl
             << "3) Leaderboards" << endl
             << "4) Commands" << endl
             << "0) Quit" << endl << endl
             << "Choose an option: ";
        cin >> option;
    }while(option < '0' || option > '4');

    switch(option){
    case '1': newGame(); break;
    case '2': resumeGame(); break;
    case '3': showLeaderboard(); break;
    case '4': commands(); break;
    case '0': exit(0);
    }
}
void newGame(){
    // set game mode
    char mode;
    do{
        system("cls");
        cout << "Choose your game mode:" << endl << endl
             << "1) Player vs Machine" << endl
             << "2) Player vs Player" << endl
             << "3) Machine vs Machine" << endl
             << "0) Exit" << endl << endl
             << "Choose an option: ";

        cin >> mode;
        if(mode == '0'){ mainMenu(); }
        system("cls");
    }while(mode < '0' || mode > '3');

    switch (mode) {
    case '1': newPvM(); break;
    case '2': newPvP(); break;
    case '3': newMvM(); break;
    case '0': mainMenu();

    }

}
void showLeaderboard(){
    char mode;
    do{
        system("cls");
        cout << "Choose a game mode leaderboard: " << endl << endl
             << "1) Player vs Machine" << endl
             << "2) Player vs Player" << endl
             << "0) Quit" << endl << endl
             << "Choose an option: ";
        cin >> mode;
        if(mode == '0'){ mainMenu(); }
        system("cls");
    }while(mode < '0' || mode > '2');

    switch(mode){
    case '1': showLeaderboardPvM(); break;
    case '2': showLeaderboardPvP(); break;
    case '0': mainMenu();
    }

}
void showLeaderboardPvM(){
    CPvM* game = new CPvM ();
    system("cls");
    game->leaderboard();
    system("pause");
    delete game;
    mainMenu();
}
void showLeaderboardPvP(){
    CPvP* game = new CPvP ();
    system("cls");
    game->leaderboard();
    system("pause");
    delete game;
    mainMenu();
}
void commands(){
    system("cls");
    cout << "Game Commands:" << endl << endl
         << "Choose a collumn [1..7] to make your move" << endl
         << "type \"0\" to force close the game" << endl
         << "type \"-\" to undo your last play" << endl
         << "type \"+\" to go forward one play" << endl << endl;
    system("pause");
    mainMenu();
}

string setName(string n){
    string name;
    do{
        getline(cin, name);
        if(name == n) cout << "Invalid name" << endl << "Try again: ";
    } while (name.size() > 18 || name == n || name == "");
    return name;
}
void newPvM(){
    CPvM* game = new CPvM ();
    if(game->loadGame()){
        delete game;
        char aux;
        do{
            system("cls");
            cout << "Your previous game is still unfinished." << endl
                 << "If you continue, your progress will be lost." << endl << endl
                 << "1) Start a new game anyways" << endl
                 << "2) Resume saved game" << endl
                 << "0) Back" << endl << endl
                 << "Choose an option: ";
            cin >> aux;
        } while (aux <'0' || aux >'2');
        switch (aux){
        case '1': break;
        case '2': resumePvM(); break;
        case '0': mainMenu();
        }
    } else delete game;

    // set name
    string name;
    system("cls");
    cout << "Your Name: ";
    cin.ignore();
    name = setName(" ");

    // set dificulty
    char difficulty;

    do{
        system("cls");
        cout << "Hello " << name;
        cout << endl;

        cout << "Choose your difficulty:" << endl << endl
             << "1) Easy" << endl
             << "2) Medium" << endl
             << "3) Hard" << endl
             << "4) Extreme" << endl
             << "0) Exit" << endl << endl
             << "Choose an option: ";

        cin >> difficulty;
        if(difficulty == '0'){ mainMenu(); }
        system("cls");
    }while(difficulty < '0' || difficulty > '4');

    // set first player
    char starter_char;
    int starter_int;
    do{
        system("cls");
        switch (difficulty) {
        case '1': cout << "Difficulty: Easy" << endl; break;
        case '2': cout << "Difficulty: Medium" << endl; break;
        case '3': cout << "Difficulty: Hard" << endl; break;
        case '4': cout << "Difficulty: Extreme" << endl;
        }
        cout << "Who makes the first play?" << endl << endl
             << "1) Human" << endl
             << "2) Computer" << endl
             << "3) It's whatever" << endl
             << "0) Exit" << endl << endl
             << "Choose an option: ";
        cin >> starter_char;
        if(starter_char == '0'){ mainMenu(); }
    } while(starter_char < '0' || starter_char > '3');

    switch (starter_char) {
    case '1': starter_int = 1; break;
    case '2': starter_int = 2; break;
    case '3': srand (time(NULL));
        starter_int = rand() % 2 + 1;
    }

    switch (starter_int) {
    case 1: starter_char = 'O'; break;
    case 2: starter_char = '0'; break;
    }

    CPvM *g = new CPvM (name, (int)difficulty-48, starter_char);
    g->playGame();
    delete g;
    mainMenu();

}
void newPvP(){
    CPvP* game = new CPvP ();
    if(game->loadGame()){
        delete game;
        char aux;
        do{
            system("cls");
            cout << "Your previous game is still unfinished." << endl
                 << "If you continue, your progress will be lost." << endl << endl
                 << "1) Start a new game anyways" << endl
                 << "2) Resume saved game" << endl
                 << "0) Back" << endl << endl
                 << "Choose an option: ";
            cin >> aux;
        } while (aux <'0' || aux >'2');
        switch (aux){
        case '1': break;
        case '2': resumePvP(); break;
        case '0': mainMenu();
        }
    } else delete game;

    // set name
    system("cls");
    cout << "Player 1 name: ";
    cin.ignore();
    string name1 = setName(" ");

    cout << "Player 2 name: ";
    string name2 = setName(name1);

    char starter_char;
    int starter_int;
    do{
        system("cls");
        cout << "Who wants to play first?" << endl << endl
             << "1) " << name1 << endl
             << "2) " << name2 << endl
             << "3) It's whatever" << endl
             << "0) Exit" << endl << endl
             << "Choose an option: ";
        cin >> starter_char;
        if(starter_char == '0'){ mainMenu(); }
        system("cls");
    } while(starter_char < '0' || starter_char > '3');


    switch (starter_char) {
    case '1': starter_int = 1; break;
    case '2': starter_int = 2; break;
    case '3': srand (time(NULL));
        starter_int = rand() % 2 + 1;
    }

    switch (starter_int) {
    case 1: starter_char = 'O'; break;
    case 2: starter_char = '0'; break;
    }
    cout << starter_char << endl;
    CPvP* game_pvm = new CPvP (name1, name2, starter_char);
    game_pvm->playGame();
    delete game_pvm;
    mainMenu();


}
void newMvM(){
    CMvM* game = new CMvM ();
    if(game->loadGame()){
        delete game;
        char aux;
        do{
            system("cls");
            cout << "Your previous game is still unfinished." << endl
                 << "If you continue, your progress will be lost." << endl << endl
                 << "1) Start a new game anyways" << endl
                 << "2) Resume saved game" << endl
                 << "0) Back" << endl << endl
                 << "Choose an option: ";
            cin >> aux;
        } while (aux <'0' || aux >'2');
        switch (aux){
        case '1': break;
        case '2': resumeMvM(); break;
        case '0': mainMenu();
        }
    } else delete game;

    char dif1, dif2;

    // set dificulty of Machine 1
    do{
        system("cls");
        cout << "Choose Machine's 1 difficulty:" << endl << endl
             << "1) Easy" << endl
             << "2) Medium" << endl
             << "3) Hard" << endl
             << "4) Extreme" << endl
             << "0) Exit" << endl << endl
             << "Choose an option: ";

        cin >> dif1;
        if(dif1 == '0'){ mainMenu(); }
        system("cls");
    }while(dif1 < '0' || dif1 > '4');

    // set dificulty of Machine 2
    do{
        system("cls");
        cout << "Choose Machine's 2 difficulty:" << endl << endl
             << "1) Easy" << endl
             << "2) Medium" << endl
             << "3) Hard" << endl
             << "4) Extreme" << endl
             << "0) Exit" << endl << endl
             << "Choose an option: ";

        cin >> dif2;
        if(dif2 == '0'){ mainMenu(); }
        system("cls");
    }while(dif2 < '0' || dif2 > '4');

    // set first player
    char starter_char;
    int starter_int;
    do{
        system("cls");
        cout << "Who makes the first play?" << endl << endl
             << "1) Machine 1" << endl
             << "2) Machine 2" << endl
             << "3) It's whatever" << endl
             << "0) Exit" << endl << endl
             << "Choose an option: ";
        cin >> starter_char;
        if(starter_char == '0'){ mainMenu(); }
    } while(starter_char < '0' || starter_char > '3');

    switch (starter_char) {
    case '1': starter_int = 1; break;
    case '2': starter_int = 2; break;
    case '3': srand (time(NULL));
        starter_int = rand() % 2 + 1;
    }

    switch (starter_int) {
    case 1: starter_char = 'O'; break;
    case 2: starter_char = '0'; break;
    }

    CMvM *g = new CMvM ((int)dif1-48, (int)dif2-48, starter_char);
    g->playGame();
    delete g;
    mainMenu();
}

void resumeGame(){
    char mode;
    do{
        system("cls");
        cout << "Choose your game mode to resume:" << endl << endl
             << "1) Player vs Machine" << endl
             << "2) Player vs Player" << endl
             << "3) Machine vs Machine" << endl
             << "0) Exit" << endl << endl
             << "Choose an option: ";

        cin >> mode;
        if(mode == '0'){ mainMenu(); }
        system("cls");
    }while(mode < '0' || mode > '3');

    switch (mode) {
    case '1': resumePvM(); break;
    case '2': resumePvP(); break;
    case '3': resumeMvM(); break;
    case '0': mainMenu();

    }
}
void resumePvM(){
    CPvM* game = new CPvM ();
    if(game->loadGame())
        game->playGame();
    else{
        system("cls");
        cout << "No game to resume." << endl << endl;
        system("pause");
        mainMenu();
    }
    delete game;
}
void resumePvP(){
    CPvP* game = new CPvP ();
    if(game->loadGame())
        game->playGame();
    else{
        system("cls");
        cout << "No game to resume." << endl << endl;
        system("pause");
        mainMenu();
    }
    delete game;

}
void resumeMvM(){
    CMvM* game = new CMvM ();
    if(game->loadGame())
        game->playGame();
    else{
        system("cls");
        cout << "No game to resume." << endl << endl;
        system("pause");
        mainMenu();
    }
    delete game;
}
