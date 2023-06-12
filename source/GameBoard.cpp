#include <iostream>
#include <set>
#include <algorithm>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <fstream>

#include "GameBoard.h"

GameBoard::GameBoard(std::ofstream& boardFile) : m_boardFile(boardFile){

    for(int i = 0; i < 49; i++){

        m_board[i] = m_defaultLayout[i];
    }
}


GameBoard::~GameBoard(){

    resetBoard();
}


void GameBoard::displayGameBoard(){

    m_boardFile <<std::endl;

    int columnCount = 0;

    for(char slot : m_board){

        m_boardFile << slot << " ";

        columnCount++;

        if(columnCount == 7){

            m_boardFile <<std::endl;

            columnCount -= 7;
        }
    }

    m_boardFile <<std::endl;
}


//Function returns all the open slots available on the board
std::vector<int> GameBoard::getFreeSlots(){

    std::vector<int> freeSlots;

    freeSlots.clear();

    for(int i = 0; i < 49; i++){

        if(m_board[i] == m_freeSlot){

            freeSlots.push_back(i);
        }
    }

    return freeSlots;
}


void GameBoard::placeCow(int slot, char cow){

    m_board[slot] = cow;
}


void GameBoard::removeCow(int slot){

    m_board[slot] = m_freeSlot;
}


//Returns all the slots containing the requested cow
std::vector<int> GameBoard::getCommonCowSlots(char cow){

    std::vector<int> commonCowSlots;

    commonCowSlots.clear();

    for(int i = 0; i < 49; i++){

        if(m_board[i] == cow){

            commonCowSlots.push_back(i);
        }
    }

    return commonCowSlots;
}


//Returns the cow at a specific slot
char GameBoard::getSlotChar(int slot){

    return m_board[slot];
}


//Checks if the specified slot is empty
bool GameBoard::isFreeSlot(int slot){

    if(m_board[slot] == m_freeSlot){

        return true;

    }else{

        return false;
    }
}


void GameBoard::resetBoard(){

    for(int i = 0; i < 49; i++){

        m_board[i] = m_defaultLayout[i];
    }
}


//Checks if Three slots have the same cow
bool GameBoard::m_threeMatchingSlots(int slot1, int slot2, int slot3, char cow){

    if( getSlotChar(slot1) == cow &&  getSlotChar(slot1) ==  getSlotChar(slot2) && getSlotChar(slot2) ==  getSlotChar(slot3)){

        return true;
    }
    else{

        return false;
    }
}


//Checks if Two slots have the same cow and the Third slot is free
bool GameBoard::m_twoMatchingSlots(int slot1, int slot2, int slot3, char cow){

    if(( getSlotChar(slot1) == cow) && ( getSlotChar(slot1) ==  getSlotChar(slot2)) && (getSlotChar(slot2) !=  getSlotChar(slot3)) && (isFreeSlot(slot3))){

        return isFreeSlot(slot3);

    }else{

        return false;
    }
}


//Returns the best possible positions to form a mill, where 2 are in a row and the third slot is empty
std::vector<int> GameBoard::mostLikelySlotToFormMill(char cow){

    std::set<int> favouriblePositions;

    std::vector<int> favePosVect;

    favouriblePositions.clear();

    favePosVect.clear();

    //All horizontal rows of board
    if(m_twoMatchingSlots(0,3,6,cow)){
        favouriblePositions.insert(6);
    }else if(m_twoMatchingSlots(0, 6, 3, cow)){
        favouriblePositions.insert(3);
    }else if(m_twoMatchingSlots(3,6, 0, cow)){
        favouriblePositions.insert(0);
    }

    if(m_twoMatchingSlots(8,10,12,cow)){
        favouriblePositions.insert(12);
    }else if(m_twoMatchingSlots(8, 12, 10, cow)){
        favouriblePositions.insert(10);
    }else if(m_twoMatchingSlots(10,12, 8, cow)){
        favouriblePositions.insert(8);
    }

    if(m_twoMatchingSlots(16,17,18,cow)){
        favouriblePositions.insert(18);
    }else if(m_twoMatchingSlots(16, 18, 17, cow)){
        favouriblePositions.insert(17);
    }else if(m_twoMatchingSlots(17,18, 6, cow)){
        favouriblePositions.insert(6);
    }

    if(m_twoMatchingSlots(30, 31, 32,cow)){
        favouriblePositions.insert(32);
    }else if(m_twoMatchingSlots(30, 32, 31, cow)){
        favouriblePositions.insert(31);
    }else if(m_twoMatchingSlots(31,32, 30, cow)){
        favouriblePositions.insert(30);
    }

    if(m_twoMatchingSlots(36, 38, 40,cow)){
        favouriblePositions.insert(40);
    }else if(m_twoMatchingSlots(36, 40, 38, cow)){
        favouriblePositions.insert(38);
    }else if(m_twoMatchingSlots(38,40, 36, cow)){
        favouriblePositions.insert(36);
    }

    if(m_twoMatchingSlots(42, 45, 48,cow)){
        favouriblePositions.insert(48);
    }else if(m_twoMatchingSlots(42, 48, 45, cow)){
        favouriblePositions.insert(45);
    }else if(m_twoMatchingSlots(45,48, 42, cow)){
        favouriblePositions.insert(42);
    }

    //All vertical columns of board
    if(m_twoMatchingSlots(0, 21, 42,cow)){
        favouriblePositions.insert(42);
    }else if(m_twoMatchingSlots(0, 42, 21, cow)){
        favouriblePositions.insert(21);
    }else if(m_twoMatchingSlots(42,21, 0, cow)){
        favouriblePositions.insert(0);
    }

    if(m_twoMatchingSlots(8, 22, 36,cow)){
        favouriblePositions.insert(36);
    }else if(m_twoMatchingSlots(8, 36, 22, cow)){
        favouriblePositions.insert(22);
    }else if(m_twoMatchingSlots(22,36, 8, cow)){
        favouriblePositions.insert(8);
    }

    if(m_twoMatchingSlots(16, 23, 30,cow)){
        favouriblePositions.insert(30);
    }else if(m_twoMatchingSlots(16, 30, 23, cow)){
        favouriblePositions.insert(23);
    }else if(m_twoMatchingSlots(30,23, 16, cow)){
        favouriblePositions.insert(16);
    }

    if(m_twoMatchingSlots(3, 10, 17,cow)){
        favouriblePositions.insert(17);
    }else if(m_twoMatchingSlots(3, 17, 10, cow)){
        favouriblePositions.insert(10);
    }else if(m_twoMatchingSlots(10,17, 3, cow)){
        favouriblePositions.insert(3);
    }

    if(m_twoMatchingSlots(45, 38, 31,cow)){
        favouriblePositions.insert(31);
    }else if(m_twoMatchingSlots(45, 31, 38, cow)){
        favouriblePositions.insert(38);
    }else if(m_twoMatchingSlots(31,38, 45, cow)){
        favouriblePositions.insert(45);
    }

    if(m_twoMatchingSlots(32, 25, 18,cow)){
        favouriblePositions.insert(18);
    }else if(m_twoMatchingSlots(32, 18, 25, cow)){
        favouriblePositions.insert(25);
    }else if(m_twoMatchingSlots(25,18, 32, cow)){
        favouriblePositions.insert(32);
    }

    if(m_twoMatchingSlots(12, 26, 40,cow)){
        favouriblePositions.insert(40);
    }else if(m_twoMatchingSlots(12, 40, 26, cow)){
        favouriblePositions.insert(26);
    }else if(m_twoMatchingSlots(26,40, 12, cow)){
        favouriblePositions.insert(12);
    }

    if(m_twoMatchingSlots(6, 27, 48,cow)){
        favouriblePositions.insert(48);
    }else if(m_twoMatchingSlots(6, 48, 27, cow)){
        favouriblePositions.insert(27);
    }else if(m_twoMatchingSlots(48,27, 6, cow)){
        favouriblePositions.insert(6);
    }

    //All diagonals Of board
    if(m_twoMatchingSlots(0, 8, 16,cow)){
        favouriblePositions.insert(16);
    }else if(m_twoMatchingSlots(0, 16, 8, cow)){
        favouriblePositions.insert(8);
    }else if(m_twoMatchingSlots(16,8, 0, cow)){
        favouriblePositions.insert(0);
    }

    if(m_twoMatchingSlots(48, 40, 32,cow)){
        favouriblePositions.insert(32);
    }else if(m_twoMatchingSlots(48, 32, 40, cow)){
        favouriblePositions.insert(40);
    }else if(m_twoMatchingSlots(32,40, 48, cow)){
        favouriblePositions.insert(48);
    }

    if(m_twoMatchingSlots(6, 12, 18,cow)){
        favouriblePositions.insert(18);
    }else if(m_twoMatchingSlots(6, 12, 18, cow)){
        favouriblePositions.insert(12);
    }else if(m_twoMatchingSlots(12,18, 6, cow)){
        favouriblePositions.insert(6);
    }

    if(m_twoMatchingSlots(42, 36, 30,cow)){
        favouriblePositions.insert(30);
    }else if(m_twoMatchingSlots(42, 30, 36, cow)){
        favouriblePositions.insert(36);
    }else if(m_twoMatchingSlots(36,30, 42, cow)){
        favouriblePositions.insert(42);
    }

    for(int element: favouriblePositions){
        favePosVect.push_back(element);
    }
    return favePosVect;
}


//Returns all the slots that are part of mills
std::vector<int> GameBoard::mills(char cow){

    std::vector<int> millCows;

    if(m_threeMatchingSlots(0,3,6,cow)){
        millCows.push_back(0);
        millCows.push_back(3);
        millCows.push_back(6);
    }

    if(m_threeMatchingSlots(8, 10, 12, cow)){
        millCows.push_back(8);
        millCows.push_back(10);
        millCows.push_back(12);
    }

    if(m_threeMatchingSlots(16, 17, 18,cow)){
        millCows.push_back(16);
        millCows.push_back(17);
        millCows.push_back(18);
    }

    if(m_threeMatchingSlots(21, 22, 23,cow)){
        millCows.push_back(21);
        millCows.push_back(22);
        millCows.push_back(23);
    }

    if(m_threeMatchingSlots(25, 26, 27,cow)){
        millCows.push_back(25);
        millCows.push_back(26);
        millCows.push_back(27);
    }

    if(m_threeMatchingSlots(30, 31, 32,cow)){
        millCows.push_back(30);
        millCows.push_back(31);
        millCows.push_back(32);
    }

    if(m_threeMatchingSlots(36, 38, 40,cow)){
        millCows.push_back(36);
        millCows.push_back(38);
        millCows.push_back(40);
    }

    if(m_threeMatchingSlots(42, 45, 48,cow)){
        millCows.push_back(42);
        millCows.push_back(45);
        millCows.push_back(48);
    }

    //Vertical columns of board
    if(m_threeMatchingSlots(0,21,42,cow)){
        millCows.push_back(0);
        millCows.push_back(21);
        millCows.push_back(42);
    }

    if(m_threeMatchingSlots(8, 22, 36, cow)){
        millCows.push_back(8);
        millCows.push_back(22);
        millCows.push_back(36);
    }

    if(m_threeMatchingSlots(16, 23, 30,cow)){
        millCows.push_back(16);
        millCows.push_back(23);
        millCows.push_back(30);
    }

    if(m_threeMatchingSlots(3, 10, 17,cow)){
        millCows.push_back(3);
        millCows.push_back(10);
        millCows.push_back(17);
    }

    if(m_threeMatchingSlots(31, 38, 45, cow)){
        millCows.push_back(31);
        millCows.push_back(38);
        millCows.push_back(45);
    }

    if(m_threeMatchingSlots(32, 25, 18,cow)){
        millCows.push_back(32);
        millCows.push_back(25);
        millCows.push_back(18);
    }

    if(m_threeMatchingSlots(12, 26, 40,cow)){
        millCows.push_back(12);
        millCows.push_back(26);
        millCows.push_back(40);
    }

    if(m_threeMatchingSlots(6, 27, 48,cow)){
        millCows.push_back(6);
        millCows.push_back(27);
        millCows.push_back(48);
    }

    //All diagonals on board
    if(m_threeMatchingSlots(0, 8, 16,cow)){
        millCows.push_back(0);
        millCows.push_back(8);
        millCows.push_back(16);
    }

    if(m_threeMatchingSlots(48, 40, 32,cow)){
        millCows.push_back(48);
        millCows.push_back(40);
        millCows.push_back(32);
    }

    if(m_threeMatchingSlots(6, 12, 18,cow)){
        millCows.push_back(6);
        millCows.push_back(12);
        millCows.push_back(18);
    }

    if(m_threeMatchingSlots(42, 36, 30,cow)){
        millCows.push_back(42);
        millCows.push_back(36);
        millCows.push_back(30);
    }

    return millCows;
}


//Returns all the unique cows that are in mills
std::vector<int> GameBoard::getUniqueCowsInMills(char cow){

    std::set<int> uniqueCowsInMills;

    std::vector<int> uniqMillCowVec;

    uniqMillCowVec.clear();

    uniqueCowsInMills.clear();

    for(int element : mills(cow)){

        uniqueCowsInMills.insert(element);
    }

    for(int element : uniqueCowsInMills){

        uniqMillCowVec.push_back(element);
    }

    return uniqMillCowVec;
}


//Checks if all the cows are in mills
bool GameBoard::allCowsInMills(char cow){

    std::vector<int> cowsInMills = getUniqueCowsInMills(cow);

    std::vector<int> cowSlots = getCommonCowSlots(cow);

    if(cowsInMills >= cowSlots){

        return false;

    }else{

        return true;
    }
}


//Convert the slot (integer) to the relevant intersection, for example slot 17 = S0,I7
std::string GameBoard::convertToIntersection(int slot){

    switch(slot){

    case 0:
        return "S2,i6";
        break;

    case 3:
        return "S2,i7";
        break;

    case 6:
        return "S2,i0";
        break;

    case 8:
        return "S1,i6";
        break;

    case 10:
        return "S1,i7";
        break;

    case 12:
        return "S1,i0";
        break;

    case 16:
        return "S0,i6";
        break;

    case 17:
        return "S0,i7";
        break;

    case 18:
        return "S0,i0";
        break;

    case 21:
        return "S2,i5";
        break;

    case 22:
        return "S1,i5";
        break;

    case 23:
        return "S0,i5";
        break;

    case 25:
        return "S0,i1";
        break;

    case 26:
        return "S1,i1";
        break;

    case 27:
        return "S2,i1";
        break;

    case 30:
        return "S0,i4";
        break;

    case 31:
        return "S0,i3";
        break;

    case 32:
        return "S0,i2";
        break;

    case 36:
        return "S1,i4";
        break;

    case 38:
        return "S1,i3";
        break;

    case 40:
        return "S1,i2";
        break;

    case 42:
        return "S2,i4";
        break;

    case 45:
        return "S2,i3";
        break;

    case 48:
        return "S2,i2";
        break;

    default:
        return "Error: Run the program again";
    }
}


//Format the conversion as specified for mills, for example for example cow on slot 17 = S0I7
std::string GameBoard::convertMillToIntersection(int slot){

    switch(slot){

    case 0:
        return "S2i6";
        break;

    case 3:
        return "S2i7";
        break;

    case 6:
        return "S2i0";
        break;

    case 8:
        return "S1i6";
        break;

    case 10:
        return "S1i7";
        break;

    case 12:
        return "S1i0";
        break;

    case 16:
        return "S0i6";
        break;

    case 17:
        return "S0i7";
        break;

    case 18:
        return "S0i0";
        break;

    case 21:
        return "S2i5";
        break;

    case 22:
        return "S1i5";
        break;

    case 23:
        return "S0i5";
        break;

    case 25:
        return "S0i1";
        break;

    case 26:
        return "S1i1";
        break;

    case 27:
        return "S2i1";
        break;

    case 30:
        return "S0i4";
        break;

    case 31:
        return "S0i3";
        break;

    case 32:
        return "S0i2";
        break;

    case 36:
        return "S1i4";
        break;

    case 38:
        return "S1i3";
        break;

    case 40:
        return "S1i2";
        break;

    case 42:
        return "S2i4";
        break;

    case 45:
        return "S2i3";
        break;

    case 48:
        return "S2i2";
        break;

    default:
        return "Error: Run the program again";
    }
}


//Returns all the open playable slots adjacent to the specified cow
std::vector<int> GameBoard::getEmptyNeighbor(int slot){

    std::vector<int> freeNeighbors;

    switch(slot){
        case 0:
            freeNeighbors = { 21, 8, 3};
            break;

        case 3:
            freeNeighbors = { 0, 6, 10};
            break;

        case 6:
            freeNeighbors = { 27, 3, 12};
            break;

        case 8:
            freeNeighbors = { 0, 22, 16, 10};
            break;

        case 10:
            freeNeighbors = { 3, 8, 12, 17};
            break;

        case 12:
            freeNeighbors = { 10, 6, 18, 26};
            break;

        case 16:
            freeNeighbors = { 8, 23, 17};
            break;

        case 17:
            freeNeighbors = { 10, 16, 18};
            break;

        case 18:
            freeNeighbors = { 12, 17, 25};
            break;

        case 21:
            freeNeighbors = { 0, 42, 22};
            break;

        case 22:
            freeNeighbors = { 8, 36, 21, 23};
            break;

        case 23:
            freeNeighbors = { 16, 30, 22};
            break;

        case 25:
            freeNeighbors = { 26, 18, 32};
            break;

        case 26:
            freeNeighbors = { 25, 12, 40, 27};
            break;

        case 27:
            freeNeighbors = { 6, 48, 26};
            break;

        case 30:
            freeNeighbors = { 31, 23, 36};
            break;

        case 31:
            freeNeighbors = { 30, 32, 38};
            break;

        case 32:
            freeNeighbors = { 25, 31, 40};
            break;

        case 36:
            freeNeighbors = { 30, 42, 38, 22};
            break;

        case 38:
            freeNeighbors = { 31, 45, 36, 40};
            break;

        case 40:
            freeNeighbors = { 48, 32, 26, 38};
            break;

        case 42:
            freeNeighbors = { 36, 21, 45};
            break;

        case 45:
            freeNeighbors = { 42, 48, 38};
            break;

        case 48:
            freeNeighbors = { 27, 40, 45};
            break;
    }

    // Only add empty slots to the vector. A size of 0 means no available slots.
    std::vector<int> result;

    result.clear();

    for (int element : freeNeighbors){

        if (isFreeSlot(element)){

            result.push_back(element);
        }
    }

    return result;
}
