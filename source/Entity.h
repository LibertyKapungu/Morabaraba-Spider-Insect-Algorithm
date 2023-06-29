#ifndef ENTITY_H
#define ENTITY_H

#include <iostream>
#include <set>
#include <vector>
#include <algorithm>
#include <cstdlib>
#include <ctime>

#include "GameBoard.h"

class Entity{

public:

    Entity(GameBoard* gameBoard, char cow, char opponentCow, int numberOfStartingPieces);

    //Placement Phase
    int randomPlacement();
    int strategicPlacement();

    //Flying Phase
    void randomFly();
    void strategicFly();

    //Movement Phase
    int moveRandomPiece();
    int moveStrategicPiece();

    int randomAttack();

    bool verifyNewMillFormed();
    bool cannotMakeMove();

    std::vector<int> getPiecesTaken();
    std::vector<int> getMoveHistory();
    std::vector<int> getCurrentMill();

private:

    GameBoard* m_gameBoard;

    char m_cow;
    char m_opponentCow;

    int m_numberOfStartingPieces;

    bool m_cannotMove;

    std::vector<int> m_moveHistory;
    std::set<int> m_verifiedMills;
    std::vector<int> m_piecesTaken;
    std::vector<int> m_currentMill;
};
#endif // ENTITY_H
