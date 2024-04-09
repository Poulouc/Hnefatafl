/**
 * @file fonctions.cpp
 *
 * @brief functions for the Hnefatafl game.
 *
 * This file contains the functions used in the Hnefatafl game, including
 * board initialization, display, piece movement, capturing, and game state checks.
 *
 * @author Guillon Romain - IUT Informatique La Rochelle
 * @date 16/11/2023
 */

#ifndef FUNCTIONS_H
#define FUNCTIONS_H
#include "typeDef.h"
#include "functions.h"
#include <iostream>
#include <random>
#include <fstream>

using namespace std;


PlayerRole randomPlayer()
{
    random_device rd;
    default_random_engine eng(rd());
    uniform_int_distribution<int> distr(0, 1);
    PlayerRole role[2] = {DEFENSE, ATTACK};
    return role[distr(eng)]; //to begin the game with a random player
}


void clearConsole(){
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}


void displayHnefataflLogo()
{
    cout << " __    __  .__   __.  _______  _______    ___   .___________. _______  __      \n"
            "|  |  |  | |  \\ |  | |   ____||   ____|  /   \\  |           ||   ____||  |     \n"
            "|  |__|  | |   \\|  | |  |__   |  |__    /  ^  \\ `---|  |----`|  |__   |  |     \n"
            "|   __   | |  . `  | |   __|  |   __|  /  /_\\  \\    |  |     |   __|  |  |     \n"
            "|  |  |  | |  |\\   | |  |____ |  |    /  _____  \\   |  |     |  |     |  `----.\n"
            "|__|  |__| |__| \\__| |_______||__|   /__/     \\__\\  |__|     |__|     |_______|\n"
            "                                                                               ";
    cout << "by Guillon Romain C2" << endl;
}


bool chooseSizeBoard(BoardSize& aBoardSize)
{
    //input of the value
    string size;
    cout<<"Enter the size of the Board (11, 13): ";
    cin>> size;
    int firstLetter = size[0], secondLetter = size[1];
    //verify if the value is correct and set the size wanted
    if(size.size() == 2 and firstLetter == 49 and secondLetter == 49)
    {
        aBoardSize = LITTLE;
        return true;
    }
    else if(size.size() == 2 and firstLetter == 49 and secondLetter == 51)
    {
        aBoardSize = BIG;
        return true;
    }
    else{
        return false;
    }
}


void initializeBoard(Cell aBoard[][BOARD_SIZE_MAX], const BoardSize& aBoardSize)
{
    //initialize the board with his default value
    for(int i = 0; i < aBoardSize; i++){
        for(int j = 0; j < aBoardSize; j++){
            aBoard[i][j].itsCellType = NORMAL;
            aBoard[i][j].itsPieceType = NONE;
        }
    }
    const int NBSWORD = 5;
    const int GAP = aBoardSize / 2 - NBSWORD / 2; // center the sword pieces lines
    for(int i = 0; i < NBSWORD; i++)
    {
        aBoard[0][i + GAP].itsPieceType = SWORD; // place the upper line of sword pieces
        aBoard[aBoardSize - 1][i + GAP].itsPieceType = SWORD; //place the lower line of sword pieces
        aBoard[i + GAP][aBoardSize - 1].itsPieceType = SWORD; //place the right
        // line of sword pieces
        aBoard[i + GAP][0].itsPieceType = SWORD; // places the left line of sword pieces
    }
    aBoard[1][aBoardSize/2].itsPieceType = SWORD; //place the upper central sword pieces
    aBoard[aBoardSize/2][1].itsPieceType =SWORD; //place the lower central sword pieces
    aBoard[aBoardSize/2][aBoardSize - 2].itsPieceType = SWORD; //place the right central sword pieces
    aBoard[aBoardSize-2][aBoardSize/2].itsPieceType = SWORD; //place the left central sword pieces
    const int SHIELDSIZE = 3 + (aBoardSize == BIG);
    for(int i = 0; i <= aBoardSize/2; i++)
    {
        for(int j = 0; j < SHIELDSIZE-i; j++)
        {
            if(aBoardSize == BIG and j != i) //to make a cross
            {
                aBoard[aBoardSize/2 - i][aBoardSize/2].itsPieceType = SHIELD; // constitutes the lower right part of the shields
                aBoard[aBoardSize/2][aBoardSize/2 - j].itsPieceType = SHIELD; // constitutes the upper left part of the shields
                aBoard[aBoardSize/2 + i][aBoardSize/2].itsPieceType = SHIELD; // constitutes the upper right part of the shields
                aBoard[aBoardSize/2][aBoardSize/2 + j].itsPieceType = SHIELD; // constitutes the upper right part of the shields
            }
            else if(aBoardSize == LITTLE)//to make the diament
            {
                aBoard[aBoardSize/2 + i][aBoardSize/2 + j].itsPieceType = SHIELD; // constitutes the upper right part of the shields
                aBoard[aBoardSize/2 - i][aBoardSize/2 + j].itsPieceType = SHIELD; // constitutes the lower right part of the shields
                aBoard[aBoardSize/2 + i][aBoardSize/2 - j].itsPieceType = SHIELD; // constitutes the upper left part of the shields
                aBoard[aBoardSize/2 - i][aBoardSize/2 - j].itsPieceType = SHIELD; // constitutes the lower right part of the shields
            }
        }
    }
    aBoard[aBoardSize/2][aBoardSize/2].itsPieceType = KING;
    aBoard[aBoardSize/2][aBoardSize/2].itsCellType = CASTLE;
    for(int i = 1; i <= aBoardSize; i = i + aBoardSize-1)
    {
        for(int j = 1; j <= aBoardSize; j = j + aBoardSize-1)
        {
            aBoard[i-1][j-1].itsCellType = FORTRESS; //to put fortress in the four angle
        }
    }
}

/**
 * @brief separator_line make a line separator between each line of the board
 * @param SIZE the width of the board
 */
void separator_line(const int SIZE)
{
    for(int l = 0; l < SIZE; l++)
    {
        cout<<"+---";
    }
    cout<<"+"<<endl;
}


void displayBoard(const Cell aBoard[][BOARD_SIZE_MAX], const BoardSize& aBoardSize)
{
    cout<<" ";
    for(int k = 1;k <= 9; k++) //display the number on the top of the board
    {
        cout<<"   "<<k;// 3 spaces and then the number
    }
    for(int k = 1;k <= aBoardSize - 9; k++)
    {
        cout<<"  "<<k+9;// 2 spaces and then the number when his bigger than 9
    }
    for(int i = 0; i < aBoardSize; i++)
    {
        cout<<endl<<"  ";
        separator_line(aBoardSize);// display the line between each line of pieces
        cout<<static_cast<char>(65+i)<< " |";//display the letters on the left side of the board
        for(int j = 0; j < aBoardSize; j++)
        {
            if(aBoard[i][j].itsPieceType == SWORD) // display in priority pieces and then fortress and castle
            {
                cout<<" + ";
            }
            else if(aBoard[i][j].itsPieceType == SHIELD)
            {
                cout<<" o ";
            }
            else if(aBoard[i][j].itsPieceType == KING)
            {
                cout<<" W ";
            }
            else if(aBoard[i][j].itsCellType == NORMAL and aBoard[i][j].itsPieceType == NONE)
            {
                cout<< "   ";
            }
            else if(aBoard[i][j].itsCellType == FORTRESS)
            {
                cout<<" # ";
            }
            else if(aBoard[i][j].itsCellType == CASTLE)
            {
                cout<<" x ";
            }
            else
            {
                cout<<" ! ";//ERROR
            }
            cout<<"|";
        }
    }
    cout<<endl<<"  ";
    separator_line(aBoardSize);//put the last line
}


Position getPositionFromInput(){
    string input;
    cout<<"Enter a coordinate (ex: A11) : ";
    cin>> input;
    //set this default position
    Position Pos;
    Pos.itsCol = 0;
    Pos.itsRow = 0;
    int pow = 1, i = input.size() - 1, col = 0;
    bool itsletter = !(toupper(input[0]) >= 65 and toupper(input[0]) <= 90); //allows you to check if the first character is indeed a letter
    while(i >= 0 and not itsletter){
        if(i == 0){
            Pos.itsRow = toupper(input[i]) - 65;
        }
        else{
            itsletter = (toupper(input[i]) >= 65 and toupper(input[i]) <= 90); //allows you to check if another letter has been found
            col += (input[i] - 48)*pow;
            pow *= 10;
        }
        i--; //must start at the end if we want to add the powers to the digit of tens, hundreds, etc.
    }
    Pos.itsCol = col - 1;//because the first box of the board have for coordinate(0,0)
    if(itsletter){ //if an other letter as been found
        Pos.itsCol = -1;
        Pos.itsRow = -1;
    }
    return Pos;
}


bool isValidPosition(const Position& aPos, const BoardSize& aBoardSize)
{
    return (0 <= aPos.itsRow and aPos.itsRow < aBoardSize) and (0 <= aPos.itsCol and aPos.itsCol < aBoardSize);
    // if the entered is on the board
}


bool isEmptyCell(const Cell aBoard[][BOARD_SIZE_MAX], const Position& aPos)
{
    return aBoard[aPos.itsRow][aPos.itsCol].itsPieceType == NONE;
}


bool isValidMovement(const PlayerRole& aPlayer, const Cell aBoard[][BOARD_SIZE_MAX],const Position& aStartPos,const Position& aEndPos){
    bool isValid = true;
    if(!(((aStartPos.itsRow == aEndPos.itsRow) and (aStartPos.itsCol != aEndPos.itsCol)) or ((aStartPos.itsRow != aEndPos.itsRow)
                                                                                              and (aStartPos.itsCol == aEndPos.itsCol)))){
        //check if the start row is equal to end row or if the start col is equal to start row(can be one of those but not both)
        cout<<"that's not a valid movement";
        isValid = false;
    }
    else if(isValid and aPlayer == ATTACK and aBoard[aStartPos.itsRow][aStartPos.itsCol].itsPieceType != SWORD){ //check if the attack selected a SWORD
        cout<<"that's not your piece"<<endl;
        isValid = false;
    }
    else if(aPlayer == DEFENSE and !(aBoard[aStartPos.itsRow][aStartPos.itsCol].itsPieceType == SHIELD
                                      or aBoard[aStartPos.itsRow][aStartPos.itsCol].itsPieceType == KING)){ // check if the defense selected is a king or a shield
        cout<<"that's not your piece"<<endl;
        isValid = false;
    }
    else if(isValid and aStartPos.itsRow == aEndPos.itsRow and aStartPos.itsCol < aEndPos.itsCol){//check if this loop check the right path, here it's the top
        for(int i = aStartPos.itsCol + 1; i <= aEndPos.itsCol; i++){
            if(((aBoard[aEndPos.itsRow][i].itsPieceType != NONE or aBoard[aEndPos.itsRow][i].itsCellType != NORMAL)
                 and aBoard[aStartPos.itsRow][aStartPos.itsCol].itsPieceType != KING) or (aBoard[aStartPos.itsRow][aStartPos.itsCol].itsPieceType == KING
                    and aBoard[aEndPos.itsRow][i].itsPieceType != NONE)){
                // check if their not a pieces between the start and the end of the path
                cout<<"something is on your way"<<endl;
                isValid = false;
            }
        }
    }
    else if(isValid and aStartPos.itsRow == aEndPos.itsRow and aStartPos.itsCol > aEndPos.itsCol){ //here it's the bottom
        for(int i = aStartPos.itsCol - 1; i >= aEndPos.itsCol; i--){
            if(((aBoard[aEndPos.itsRow][i].itsPieceType != NONE or aBoard[aEndPos.itsRow][i].itsCellType != NORMAL)
                 and aBoard[aStartPos.itsRow][aStartPos.itsCol].itsPieceType != KING) or (aBoard[aStartPos.itsRow][aStartPos.itsCol].itsPieceType == KING
                    and aBoard[aEndPos.itsRow][i].itsPieceType != NONE)){
                cout<<"something is on your way"<<endl;
                isValid = false;
            }
        }
    }
    else if(isValid and aStartPos.itsCol == aEndPos.itsCol and aStartPos.itsRow < aEndPos.itsRow){//here it's right
        for(int i = aStartPos.itsRow + 1; i <= aEndPos.itsRow; i++){
            if(((aBoard[i][aEndPos.itsCol].itsPieceType != NONE or aBoard[i][aEndPos.itsCol].itsCellType != NORMAL)
                 and aBoard[aStartPos.itsRow][aStartPos.itsCol].itsPieceType != KING) or (aBoard[aStartPos.itsRow][aStartPos.itsCol].itsPieceType == KING
                    and aBoard[i][aEndPos.itsCol].itsPieceType != NONE)){
                cout<<"something is on your way"<<endl;
                isValid = false;
            }
        }
    }
    else if(isValid and aStartPos.itsCol == aEndPos.itsCol and aStartPos.itsRow > aEndPos.itsRow){//here it's left
        for(int i = aStartPos.itsRow - 1; i >= aEndPos.itsRow; i--){
            if(((aBoard[i][aEndPos.itsCol].itsPieceType != NONE or aBoard[i][aEndPos.itsCol].itsCellType != NORMAL)
                 and aBoard[aStartPos.itsRow][aStartPos.itsCol].itsPieceType != KING) or (aBoard[aStartPos.itsRow][aStartPos.itsCol].itsPieceType == KING
                    and aBoard[i][aEndPos.itsCol].itsPieceType != NONE)){
                cout<<"something is on your way"<<endl;
                isValid = false;
            }
        }
    }
    return isValid;
}


void movePiece(Cell aBoard[][BOARD_SIZE_MAX], const Position& aStartPos, const Position& aEndPos)
{
    aBoard[aEndPos.itsRow][aEndPos.itsCol].itsPieceType = aBoard[aStartPos.itsRow][aStartPos.itsCol].itsPieceType;
    aBoard[aStartPos.itsRow][aStartPos.itsCol].itsPieceType = NONE; // clear the start
}


void capturePieces(const PlayerRole& aPlayer, Cell aBoard[][BOARD_SIZE_MAX], const BoardSize &aBoardSize, const Position& aEndPos){
    const int nbSeedVerif = 4;
    Position alliePos;
    //the direction board to see where it's necessary(top, bottom, right, and left)
    int direction[nbSeedVerif][2] = {{ -1, 0}, { 1, 0}, {0, -1}, {0,  1}};
    for(int j = 0; j < nbSeedVerif; j++){
        //set the supposed position of the allies
        alliePos.itsRow = direction[j][0] * 2 + aEndPos.itsRow;
        alliePos.itsCol = direction[j][1] * 2 + aEndPos.itsCol;
        if(aPlayer == DEFENSE and aBoard[direction[j][0] + aEndPos.itsRow][direction[j][1] + aEndPos.itsCol].itsPieceType == SWORD) {
            // if an adverse pieces was next to the piece who moved
            if(isValidPosition(alliePos, aBoardSize) and (aBoard[alliePos.itsRow][alliePos.itsCol].itsPieceType == SHIELD
                                                           or aBoard[alliePos.itsRow][alliePos.itsCol].itsPieceType == KING or aBoard[alliePos.itsRow][alliePos.itsCol].itsCellType != NORMAL)) {
                //if an allie peace is next to the opposite peace with the peace who moved
                aBoard[direction[j][0] + aEndPos.itsRow][direction[j][1] + aEndPos.itsCol].itsPieceType = NONE;
            }
        }
        else if(aPlayer == ATTACK and aBoard[direction[j][0] + aEndPos.itsRow][direction[j][1] + aEndPos.itsCol].itsPieceType == SHIELD) {
            if(isValidPosition(alliePos, aBoardSize) and (aBoard[alliePos.itsRow][alliePos.itsCol].itsPieceType == SWORD
                                                           or (aBoard[alliePos.itsRow][alliePos.itsCol].itsCellType != NORMAL and aBoard[alliePos.itsRow][alliePos.itsCol].itsPieceType != KING))) {
                aBoard[direction[j][0] + aEndPos.itsRow][direction[j][1] + aEndPos.itsCol].itsPieceType = NONE;
            }
        }
    }
}


bool isSwordLeft(const Cell aBoard[][BOARD_SIZE_MAX], const BoardSize& aBoardSize)
{
    bool isLeft = false;
    int i = 0, j = 0;
    while(aBoard[i][j].itsPieceType != SWORD and i < aBoardSize) // while sword is not found and i,j is not at the end of the board
    {
        j++;
        if(j == aBoardSize)
        {
            j = 0;
            i += 1;
        }
    }
    if(i < aBoardSize) // if this not the end of the board the i will be bigger than the size of the board which means the board was completly parcoured
    {
        isLeft = true;
    }
    return isLeft;
}


Position getKingPosition(const Cell aBoard[][BOARD_SIZE_MAX], const BoardSize& aBoardSize){
    //set default position
    Position kingPosition;
    kingPosition.itsRow = -1;
    kingPosition.itsCol = -1;
    int i = 0, j;
    while(i < aBoardSize and kingPosition.itsRow == -1 and kingPosition.itsCol == -1){ // search the king while is not find or it's the end of the board
        j = 0;
        while(j < aBoardSize and (kingPosition.itsRow == -1 and kingPosition.itsCol == -1)){ // search the king while is not find or it's the end of the board
            if(aBoard[i][j].itsPieceType == KING){
                kingPosition.itsRow = i;
                kingPosition.itsCol = j;
            }
            j++;
        }
        i++;
    }
    return kingPosition;
}


bool isKingEscaped(const Cell aBoard[][BOARD_SIZE_MAX], const BoardSize& aBoardSize){
    const int nbFortress = 4;
    Position kingSeed = getKingPosition(aBoard, aBoardSize);
    bool isEscaped = false;
    int fortressSeed[nbFortress][2] = {{0,0},{0, aBoardSize -1},{aBoardSize-1, 0},{aBoardSize-1, aBoardSize-1}}, i = 0; // the coordinate of the 4 angles
    while(!isEscaped and i < nbFortress){
        if(kingSeed.itsRow == fortressSeed[i][0] and kingSeed.itsCol == fortressSeed[i][1]){ // if the king is in one of the 4 angles
            isEscaped = true;
        }
        i++;
    }
    return isEscaped;
}


bool isKingCaptured(const Cell aBoard[][BOARD_SIZE_MAX], const BoardSize& aBoardSize){
    Position kingSeed = getKingPosition(aBoard, aBoardSize);
    if((kingSeed.itsRow - 1 < 0 or aBoard[kingSeed.itsRow - 1][kingSeed.itsCol].itsPieceType == SWORD or aBoard[kingSeed.itsRow - 1][kingSeed.itsCol].itsCellType == FORTRESS or aBoard[kingSeed.itsRow - 1][kingSeed.itsCol].itsCellType == CASTLE) and (kingSeed.itsRow + 1 >= aBoardSize or aBoard[kingSeed.itsRow + 1][kingSeed.itsCol].itsPieceType == SWORD or aBoard[kingSeed.itsRow + 1][kingSeed.itsCol].itsCellType == FORTRESS or aBoard[kingSeed.itsRow + 1][kingSeed.itsCol].itsCellType == CASTLE)
        and (kingSeed.itsCol - 1 < 0 or aBoard[kingSeed.itsRow][kingSeed.itsCol-1].itsPieceType == SWORD or aBoard[kingSeed.itsRow][kingSeed.itsCol-1].itsCellType == FORTRESS or aBoard[kingSeed.itsRow][kingSeed.itsCol-1].itsCellType == CASTLE) and (kingSeed.itsCol + 1 >= aBoardSize or aBoard[kingSeed.itsRow][kingSeed.itsCol + 1].itsPieceType == SWORD or aBoard[kingSeed.itsRow][kingSeed.itsCol + 1].itsCellType == FORTRESS or aBoard[kingSeed.itsRow][kingSeed.itsCol + 1].itsCellType == CASTLE)){
        return true; //if the king was blocked on the front, the left side, right side, and behind by a sword, the end of the board, a castle, or a fortress
    }
    else{
        return false;
    }
}


bool enregBoard(const Cell aBoard[][BOARD_SIZE_MAX], const BoardSize& aBoardSize, const PlayerRole& aPlayer){
    //input
    bool isEnreg = false;
    string isR;
    cout<<"Do you want to save (0,1): ";
    cin>> isR;
    int jsp = isR[0] - 48;
    isEnreg = jsp;
    //register the board
    if(isEnreg) {
        //choose which save the user want
        clearConsole();
        displayHnefataflLogo();
        string save[] = {"sauvegarde1.txt", "sauvegarde2.txt", "sauvegarde3.txt", "sauvegarde4.txt"}, chooseSave = "5";
        //menu
        cout<<"+-------------------------------------------------------+\n"<<
            "|                      Choose a save                    |\n"<<
            "+-------------------------------------------------------+\n"<<
            "|                      deplacement 1                    |\n"<<
            "+-------------------------------------------------------+\n"<<
            "|                    Mouvement valide 2                 |\n"<<
            "+-------------------------------------------------------+\n"<<
            "|                  Captures/Fin de Partie 3             |\n"<<
            "+-------------------------------------------------------+\n"<<
            "|                      Fin de Partie 4                  |\n"<<
            "+-------------------------------------------------------+\n"<<
            "|                          exit 5                       |\n"<<
            "+-------------------------------------------------------+\n"<<
            "Enter which save you want (1, 2, 3, 4): ";
        cin >> chooseSave;
        int num_save = chooseSave[0] - 49;
        ofstream ofile(save[num_save]);
        string boardtxt;
        //write in file
        if (ofile.is_open() and (num_save == 0 or num_save == 1 or num_save == 2 or num_save == 3)) {
            for (int i = 0; i < aBoardSize; i++) {
                for (int j = 0; j < aBoardSize; j++) {
                    //transform the board in the minimum information required
                    if (aBoard[i][j].itsPieceType == SHIELD) {
                        boardtxt += "1";
                    } else if (aBoard[i][j].itsPieceType == SWORD) {
                        boardtxt += "2";
                    } else if (aBoard[i][j].itsPieceType == KING) {
                        boardtxt += "3";
                    } else {
                        boardtxt += "0";
                    }
                }
                boardtxt += ";";
            }
            //put the information in the file
            ofile << aBoardSize << endl << boardtxt << endl << aPlayer;
            ofile.close();
            cout << "saved";
            return true;
        }
    }
    return false;//if anythings is wrong(ex: save doesn't exist)
}


void loadBoard(PlayerRole &aPlayer, BoardSize &aBoardSize, Cell aBoard[][BOARD_SIZE_MAX]){
    //input wanted
    clearConsole();
    displayHnefataflLogo();
    string save[] = {"sauvegarde1.txt", "sauvegarde2.txt", "sauvegarde3.txt", "sauvegarde4.txt"}, chooseSave = "5";
    //menu
    cout<<"+-------------------------------------------------------+\n"<<
        "|                      Choose a save                    |\n"<<
        "+-------------------------------------------------------+\n"<<
        "|                      deplacement 1                    |\n"<<
        "+-------------------------------------------------------+\n"<<
        "|                    Mouvement valide 2                 |\n"<<
        "+-------------------------------------------------------+\n"<<
        "|                  Captures/Fin de Partie 3             |\n"<<
        "+-------------------------------------------------------+\n"<<
        "|                      Fin de Partie 4                  |\n"<<
        "+-------------------------------------------------------+\n"<<
        "|                          exit 5                       |\n"<<
        "+-------------------------------------------------------+\n"<<
        "Enter which save you want (1, 2, 3, 4): ";
    cin>> chooseSave;
    int numSave = chooseSave[0] - 49;
    ifstream ifile( save[numSave]);
    //read the file
    if(ifile.is_open() and (numSave == 0 or numSave == 1 or numSave == 2 or numSave == 3)){//if this is an existant file
        //set the size of the board
        string size;
        cout<<size;
        getline(ifile, size);
        int firstLetter = size[0], secondLetter = size[1];
        if(size.size() == 2 and firstLetter == 49 and secondLetter == 49)
        {
            aBoardSize = LITTLE;
        }
        else if(size.size() == 2 and firstLetter == 49 and secondLetter == 51)
        {
            aBoardSize = BIG;
        }
        //set the board with default value
        for(int i = 0; i < aBoardSize; i++){
            for(int j = 0; j < aBoardSize; j++){
                aBoard[i][j].itsCellType = NORMAL;
                aBoard[i][j].itsPieceType = NONE;
            }
        }
        aBoard[aBoardSize/2][aBoardSize/2].itsCellType = CASTLE;
        for(int i = 1; i <= aBoardSize; i = i + aBoardSize-1)
        {
            for(int j = 1; j <= aBoardSize; j = j + aBoardSize-1)
            {
                aBoard[i-1][j-1].itsCellType = FORTRESS; //to put fortress in the four angle
            }
        }
        string aBoardtxt;
        getline(ifile, aBoardtxt);
        int i = 0, j = 0, cp;
        for(int itxt = 0; itxt < aBoardtxt.size(); itxt++){
            cp = aBoardtxt[itxt];
            if(j == aBoardSize + 1){//if this is equal to ";"
                i++;
                j = 0;
            }
            if(cp == 49){ //if this is equal to shield
                aBoard[i][j].itsPieceType = SHIELD;
            }
            else if(cp == 50){ //if this is equal to sword
                aBoard[i][j].itsPieceType = SWORD;
            }
            else if(cp == 51){ //if this is equal to KING
                aBoard[i][j].itsPieceType = KING;
            }
            j++;
        }
        //set the next Player
        ifile>>cp;
        if(cp == 0){
            aPlayer = ATTACK;
        }
        else{
            aPlayer= DEFENSE;
        }
        cout<<"loaded";
        ifile.close();
    }
    else{
        //load normally the board
        bool isValidAnswer;
        do{
            isValidAnswer = chooseSizeBoard(aBoardSize);
        }while(!isValidAnswer);
        const BoardSize aTempBoardSize = aBoardSize;
        aPlayer = ATTACK;//random_player();
        initializeBoard(aBoard, aTempBoardSize);
    }
}


void startUp(PlayerRole &aPlayer, BoardSize &aBoardSize, Cell aBoard[][BOARD_SIZE_MAX]){
    //input wanted
    bool isWantedToLoad = false;
    string cpStr;
    cout<<"Do you want to load a Save(0,1): ";
    cin >> cpStr;
    int cpInt = cpStr[0];
    isWantedToLoad = cpInt - 48;
    if(isWantedToLoad){
        //load the save
        loadBoard(aPlayer, aBoardSize, aBoard);
    }
    else{
        //load normally the board
        bool isValidAnswer;
        do{
            isValidAnswer = chooseSizeBoard(aBoardSize);
        }while(!isValidAnswer);
        const BoardSize aTempBoardSize = aBoardSize;
        aPlayer = ATTACK;//random_player();
        initializeBoard(aBoard, aTempBoardSize);
    }
}


bool isKingCapturedV2(const Cell aBoard[][BOARD_SIZE_MAX], const BoardSize& aBoardSize, Position aKingPos = {-1, -1}){

}

#endif // fonctions.cpp
