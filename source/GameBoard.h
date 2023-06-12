#ifndef GAME_BOARD_H
#define GAME_BOARD_H

#include <iostream>
#include <vector>
#include <algorithm>
#include <set>
#include <cstdlib>
#include <ctime>
#include <fstream>

class GameBoard{

public:

    GameBoard(std::ofstream& boardFile);

    ~GameBoard();

    void displayGameBoard();
    void placeCow(int slot, char cow);
    void removeCow(int slot);
    void resetBoard();

    char getSlotChar(int slot);

    bool allCowsInMills(char cow);
    bool isFreeSlot(int slot);

    std::vector<int> getCommonCowSlots(char cow);
    std::vector<int> getFreeSlots();
    std::vector<int> mostLikelySlotToFormMill(char cow);
    std::vector<int> mills(char cow);
    std::vector<int> getUniqueCowsInMills(char cow);
    std::vector<int> getEmptyNeighbor(int slot);

    std::string convertToIntersection(int slot);
    std::string convertMillToIntersection(int slot);

private:

    const char m_freeSlot = 'o';

    const char m_defaultLayout[49] = {
         'o', '.', '.', 'o', '.', '.', 'o' ,
         '.', 'o', '.', 'o', '.', 'o', '.' ,
         '.', '.', 'o', 'o', 'o', '.', '.' ,
         'o', 'o', 'o', '.', 'o', 'o', 'o' ,
         '.', '.', 'o', 'o', 'o', '.', '.' ,
         '.', 'o', '.', 'o', '.', 'o', '.' ,
         'o', '.', '.', 'o', '.', '.', 'o'
        };

    char m_board[49];

    bool m_threeMatchingSlots(int slot1, int slot2, int slot3, char cow);
    bool m_twoMatchingSlots(int slot1, int slot2, int slot3, char cow);

    std::ofstream& m_boardFile;

};
#endif // GAME_BOARD_H
