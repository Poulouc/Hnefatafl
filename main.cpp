#include <iostream>
#include "functions.h"
#include "typeDef.h"
#include "tests.h"

using namespace std;


/**
* @file main.cpp
*
* @brief Implementation of the Hnefatafl game.
*
* This file contains the implementation of the Hnefatafl game, including setup, gameplay, and outcome
determination.
* It also provides a testing suite for various game functions.
*
* @author Guillon Romain
* @date 06/11/2023
*/



/**
* @brief Function to play the Hnefatafl game.
*
* This function orchestrates the Hnefatafl game, including setup, gameplay, and outcome determination.
*/
void playGame()
{
    // initialisation du tableau
    clearConsole();
    displayHnefataflLogo();
    Cell aBoard[BIG][BIG];
    PlayerRole Player;
    BoardSize size;
    startUp(Player, size, aBoard);
    const BoardSize aBoardSize = size;
    bool isEnreg = false;
    //boucle de jeu
    while(!isEnreg and !isKingCaptured(aBoard, aBoardSize) and !isKingEscaped(aBoard, aBoardSize) and isSwordLeft(aBoard, aBoardSize))
    {
        //display
        clearConsole();
        displayHnefataflLogo();
        if(Player == DEFENSE){
            cout<<"\n  -----------That's the turn of Shield to play-----------\n"<<endl;
        }
        else{
            cout<<"\n  -----------That's the turn of Sword to play-----------\n"<<endl;
        }
        displayBoard(aBoard, aBoardSize);
        //asks what the user wants to play
        Position aStartPos;
        aStartPos.itsRow = -1;
        aStartPos.itsCol = -1;
        Position aEndPos;
        aEndPos.itsRow = -1;
        aEndPos.itsCol = -1;
        do{
            do{
                cout<<"\nChoose a piece"<<endl;
                aStartPos = getPositionFromInput();
            }while(!isValidPosition(aStartPos, aBoardSize));//if the Start position is not on the board
            do{
                cout<<"\nChoose a destination"<<endl;
                aEndPos = getPositionFromInput();
            }while(!isValidPosition(aEndPos, aBoardSize));//if the Start position is not on the board
        }while(!isValidMovement(Player, aBoard, aStartPos, aEndPos));
        //move and see if he must capturs piece
        movePiece(aBoard,aStartPos, aEndPos);
        capturePieces(Player, aBoard, aBoardSize, aEndPos);
        //change the player
        if(Player == DEFENSE){
            Player = ATTACK;
        }
        else{
            Player = DEFENSE;
        }
        //ask if he want to register the board
        isEnreg = enregBoard(aBoard, aBoardSize, Player);
    }
    //end game messages
    clearConsole();
    displayHnefataflLogo();
    if(isEnreg){
        cout<<"\nThe game has been saved ;)";
    }
    else if(isKingCaptured(aBoard, aBoardSize)){
        clearConsole();
        displayHnefataflLogo();
        cout<<"\nThe Sword win UwU"<<endl;
    }
    else{
        cout<<"\nThe Shield win ^_____^"<<endl;
    }
    //permit to see the end game message
    string stop;
    cout<<"\nEnter anything you want to close the game"<<endl;
    cin>>stop;
}


/**
* @brief Launches the testing suite.
*
* This function runs a suite of tests to validate various game functions.
*/
void launchTests(){
    cout << endl << "********* Start testing *********" << endl << endl;
    //clearConsole();
    //test_chooseSizeBoard();
    //test_initializeBoard();
    //test_isKingEscaped();
    //test_getPositionFromInput();
    //test_isValidPosition();
    //test_isValidMovement();
    //test_capturePieces(); //correction necessaire
    //test_movePiece();
    //test_isSwordLeft();
    //test_getKingPosition();
    //test_isKingEscaped();
    test_isKingCaptured();
    //test_isKingCapturedV2(); // pas fait
    cout << endl << "********** End testing **********" << endl << endl;
}


/**
* @brief Main function of the game.
*
* The main function serves as the entry point for the Hnefatafl game. It can be used to start the game or run tests.
*
* @return 0 for successful execution.
*/
int main() {
    // Uncomment the line below to run tests
    //launchTests();
    // Start the game
    // set the size of the board
    playGame();
    return 0;
}
