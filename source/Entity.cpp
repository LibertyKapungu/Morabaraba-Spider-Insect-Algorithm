#include <iostream>
#include <set>
#include <vector>
#include <algorithm>
#include <cstdlib>
#include <ctime>

#include "GameBoard.h"
#include "Entity.h"

Entity::Entity(GameBoard* gameBoard, char cow, char opponentCow, int numberOfStartingPieces)
: m_gameBoard(gameBoard), m_cow(cow), m_opponentCow(opponentCow), m_numberOfStartingPieces(numberOfStartingPieces){

    srand(static_cast<unsigned int>(time(nullptr)));
    m_moveHistory.clear();
    m_verifiedMills.clear();
    m_piecesTaken.clear();
    m_cannotMove = false;
}


int Entity::randomPlacement(){

    std::vector<int> freeSlots = m_gameBoard -> getFreeSlots();

    int randIndex = rand() % freeSlots.size();

    int randElement = freeSlots[randIndex]; //Picks a random free slot on the board

    //This part of the random movement was intitially developed for the flying phase to make future expansion much easier.
    //This function would have been placed in randomMovement for the flying phase.
    if(!m_moveHistory.empty()){

        while(m_moveHistory[m_moveHistory.size()-1] == randElement || randElement == m_moveHistory[m_moveHistory.size()-2]){ //Discourage moving the same piece already moved

            if(m_moveHistory[m_moveHistory.size()-1] == randElement || randElement == m_moveHistory[m_moveHistory.size()-2]){ //Discourage moving the same cow back and forth

                randIndex = rand() % freeSlots.size();
                randElement = freeSlots[randIndex];
            }
            else{

                break;
            }
        }
    }

    m_gameBoard -> placeCow(randElement, m_cow);
    m_moveHistory.push_back(randElement);

    return randElement;
}


int Entity::strategicPlacement(){

    std::vector <int> freeSlots = m_gameBoard->getFreeSlots();

    std::vector<int> myCows = m_gameBoard -> getCommonCowSlots(m_cow);

    std::vector<int> opponentCows = m_gameBoard ->getCommonCowSlots(m_opponentCow);

    std::vector<int> myPotentialMill = m_gameBoard ->mostLikelySlotToFormMill(m_cow);

    std::vector<int> opponentPotentialMill = m_gameBoard ->mostLikelySlotToFormMill(m_opponentCow);

    std::vector<int> emptyNeighbor;

    std::vector<int> myMoves = m_moveHistory;

    std::vector<int> goodPlace;

    if(m_gameBoard -> getCommonCowSlots(m_cow).size() < 1){ //There are no cows placed on the board, so place randomly

        randomPlacement();
    }
    else if(m_gameBoard -> getCommonCowSlots(m_cow).size() < 2){ //There are cows on the board, so place near any placed cow

        emptyNeighbor = m_gameBoard->getEmptyNeighbor(m_moveHistory[m_moveHistory.size()-1]); //Gets all the slots adjacent to the placed cows

        m_gameBoard -> placeCow(emptyNeighbor[0], m_cow); //Place at the first detected slot near a cow
        m_moveHistory.push_back(emptyNeighbor[0]);       //Update move history
    }

    else if(myPotentialMill.size() != 0 && freeSlots.size() != 0){ //If there is a chance of me making a mill and there are free slots on the board

        for(int element: myPotentialMill){ //Search all my potential mills

            if(element != m_moveHistory[m_moveHistory.size()-1] && element != m_moveHistory[m_moveHistory.size()-2]){ //The potential mill is not the same as the last 2 moves To prevent repetetive motion. This check was intended for the flying phase

                goodPlace.push_back(element);                                                                        
            }
        }

        if(goodPlace.empty()){

            randomPlacement();
        }else{

            m_moveHistory.push_back(goodPlace[0]);
            m_gameBoard -> placeCow(goodPlace[0], m_cow); //Place a piece at the slot where a mill can be formed.
        }

        goodPlace.clear();
    }

    else if(opponentPotentialMill.size() != 0 && myPotentialMill.size() == 0 && freeSlots.size() != 0){ //If the opponent has a potential mill forming, block that mill.

        for(int element: opponentPotentialMill){ //Searches for all the opponents potential mills

            if(element != m_moveHistory[m_moveHistory.size()-1] && element != m_moveHistory[m_moveHistory.size()-2]){

                goodPlace.push_back(element);
            }
        }

        if(goodPlace.empty()){ //If none found, place randomly

            randomPlacement();

        }else{

            m_moveHistory.push_back(goodPlace[0]);
            m_gameBoard -> placeCow(goodPlace[0], m_cow); //Place a piece at the slot where a mill can be formed for opponent.
        }

        goodPlace.clear();
    }

    //Find an empty neighbor algorithm.
    else if(freeSlots.size() != 0 && opponentPotentialMill.size() < 1 && myMoves.size() > 2){

            for(int i = 0; i < static_cast<int>(myMoves.size()) - 2; i++){

                for(int element : m_gameBoard->getEmptyNeighbor(myMoves[i])){

                    if(element != m_moveHistory[m_moveHistory.size()-1] && element != m_moveHistory[m_moveHistory.size()-2]){

                        emptyNeighbor.push_back(element);
                        goodPlace.push_back(element);
                    }
                }
            }

            if(goodPlace.empty()){

                randomPlacement();

            }else{

                m_moveHistory.push_back(goodPlace[0]);
                m_gameBoard -> placeCow(goodPlace[0], m_cow); //Place a piece at the slot where a mill can be formed for opponent
            }

            goodPlace.clear();

    }else{

        randomPlacement();
    }

    return -1;
}


//Move a random piece to a random adjacent open slot
int Entity::moveRandomPiece(){

    int recalculate = 0;

    std::vector<int> freeSlots = m_gameBoard -> getFreeSlots(); //All free slots on the board
    std::vector<int> myCows = m_gameBoard -> getCommonCowSlots(m_cow);
    std::vector<int> moveHistory = m_moveHistory;
    std::vector<int> possibleMoves;

    int randIndex = rand() % myCows.size();

    int randElement = myCows[randIndex]; //Picks a random cow of mine

    while(randElement == m_moveHistory[m_moveHistory.size() - 1]){ //Discourage moving the same cow back and forth unless necesary

        if(((randElement == m_moveHistory[m_moveHistory.size() - 1]) || (randElement == m_moveHistory[m_moveHistory.size() - 2])) && recalculate < 5 ){

            randIndex = rand() % myCows.size();

            randElement = myCows[randIndex]; //Picks a random cow of mine

        }else{

            break;
        }

        recalculate++; //Give a chance to select a different element
    }

    possibleMoves = m_gameBoard -> getEmptyNeighbor(randElement); // Gets an empty neighbor of the random cow of mine

    int possibilityCount = 0;

    while(possibleMoves.empty()){

        if(possibleMoves.empty()){

            randIndex = rand() % myCows.size();

            randElement = myCows[randIndex]; //Picks a random cow of mine

            possibleMoves = m_gameBoard -> getEmptyNeighbor(randElement); // Gets an empty neighbor of the random cow of mine

            if(possibilityCount > 12){

                m_cannotMove = true;

            }else{

                m_cannotMove = false;
            }

        }else{

            break;
        }

        possibilityCount++;
    }


    int randIndex2 = rand() % possibleMoves.size();

    int randNeighbor = possibleMoves[randIndex2]; //Picks a random cow

    recalculate = 0;

    while(randNeighbor == m_moveHistory[m_moveHistory.size() - 1]){ //Discourage moving to previous position unless there is no other option.

        if(randNeighbor == m_moveHistory[m_moveHistory.size() - 1] && recalculate < 5){

            randIndex2 = rand() % possibleMoves.size();

            randNeighbor = possibleMoves[randIndex2];

        }else{

            break;
        }

        recalculate++;
    }

    m_gameBoard -> removeCow(randElement);

    m_moveHistory.push_back(randElement);

    m_gameBoard -> placeCow(randNeighbor, m_cow); //Place on an adjacent empty neighbor

    m_moveHistory.push_back(randNeighbor);

    m_verifiedMills.erase(randElement);

    return 0; //Return value of 0 used during the testing phase
}


bool Entity::cannotMakeMove(){

    return m_cannotMove;
}


//Make a strategic move. Spider-insect algorithm
int Entity::moveStrategicPiece(){

    int spider = -1; //The desired cow

    int insect = -1; //The target slot for the desired cow.

    std::vector<int> myPotentialMill = m_gameBoard->mostLikelySlotToFormMill(m_cow);

    std::vector<int> myCows = m_gameBoard->getCommonCowSlots(m_cow);

    std::vector<int> freeSlots = m_gameBoard->getFreeSlots();

    bool canSelectStrat = true;

    //Check if the move is repeating itself. Rather make a random move if the move is repetetive.
    if (m_moveHistory.size() >= 4 && (m_moveHistory[m_moveHistory.size() - 4] == m_moveHistory[m_moveHistory.size() - 1] || m_moveHistory[m_moveHistory.size() - 3] == m_moveHistory[m_moveHistory.size() - 2])) {

        canSelectStrat = false;

    } else {

        canSelectStrat = true;
    }

    if (myPotentialMill.size() > 0 && canSelectStrat){

        insect = myPotentialMill[0];

        std::vector<int> cowsNearTarget;

        // Find any cows near the insect
        for (int cow : myCows){

            if (abs(cow - insect) < (48/ 2)){

                cowsNearTarget.push_back(cow);
            }
        }

        std::vector<int> freeNearTarget;

        for (int free : freeSlots){

            if (abs(free - insect) < (48 / 2)){

                freeNearTarget.push_back(free);
            }
        }

        // Move a cow/spider to an empty slot near the target/insect
        for (int cow : myCows){

            if (cow == m_moveHistory.back()) { //Skip all cows that were used in the previous move to prevent the constant repetition of moves

                continue;
            }

            std::vector<int> favourableSlots = m_gameBoard->getEmptyNeighbor(cow);

            for (int slot : favourableSlots){

                if (slot == insect) { //All empty slots near the insect that overlap the empty slots near spiders

                    m_gameBoard->removeCow(cow);
                    m_verifiedMills.erase(cow);
                    m_moveHistory.push_back(cow);
                    m_gameBoard->placeCow(slot, m_cow);
                    m_moveHistory.push_back(slot);

                    return cow; //Return value used in testing
                }
            }
        }

        // Find the 'spider' that can move to an empty slot near the 'insect'
        std::vector<int> bestMove;

        for (int cow : cowsNearTarget){

            if (cow == m_moveHistory.back() || cow == m_moveHistory[m_moveHistory.size() - 2] || cow == m_moveHistory[m_moveHistory.size() - 3]) {

                continue;
            }

            std::vector<int> favourableSlots = m_gameBoard->getEmptyNeighbor(cow);

            for (int slot : favourableSlots){

                if (find(freeNearTarget.begin(), freeNearTarget.end(), slot) != freeNearTarget.end()) {

                    spider = cow;
                    bestMove.push_back(slot);
                    break;
                }
            }

            if (spider != -1){ //stop searching for spiders if one has been found

                break;
            }
        }

        //Check to prevent repetative movements
        if (spider != -1 && bestMove.size() > 0 && bestMove[0] != m_moveHistory[m_moveHistory.size()-1] && bestMove[0] != m_moveHistory[m_moveHistory.size()-2] && spider != m_moveHistory[m_moveHistory.size() - 3]) {

            m_gameBoard->removeCow(spider);
            m_verifiedMills.erase(spider);
            m_moveHistory.push_back(spider);
            m_gameBoard->placeCow(bestMove[0], m_cow);
            m_moveHistory.push_back(bestMove[0]);

        }else{

            moveRandomPiece();
        }

    }else{

        moveRandomPiece();
    }

    return -1;
}


//Attack a random cow of the opponent
int Entity::randomAttack(){

    std::vector<int> unprotectedCows; //All the opponents cows not in mills

    bool opponentAllMilled = m_gameBoard -> allCowsInMills(m_opponentCow); //Checks if all the opponents pieces are in mills

    std::vector<int> opponentCows = m_gameBoard -> getCommonCowSlots(m_opponentCow); //All the locationns of oponent cows

    std::vector<int> opponentMilledCows = m_gameBoard -> getUniqueCowsInMills(m_opponentCow); //all opponents milled cows

    if(opponentAllMilled){ //If all the opponents cows are in mills, take any random cow from opponent

        int randIndex = rand() % opponentCows.size();
        int randElement = opponentCows[randIndex];

        m_piecesTaken.push_back(randElement);

        m_gameBoard->removeCow(randElement);

        m_verifiedMills.erase(randElement); //Remove from valid milled cow since there is nolonger a valid mill

        return randElement;
    }

    else if(!opponentAllMilled){ //unprotected cows

        // Check if cow is not in mills
        for(int cow : opponentCows){

            if (std::find(opponentMilledCows.begin(), opponentMilledCows.end(), cow) == opponentMilledCows.end()) {

                unprotectedCows.push_back(cow);
            }
        }

        if (unprotectedCows.empty()){ // All opponent cows are in mills, attack any random cow

            int randIndex = rand() % opponentCows.size();
            int randElement = opponentCows[randIndex];
            m_piecesTaken.push_back(randElement);
            m_gameBoard->removeCow(randElement);
            return randElement;

        } else{ //Take a random unprotected cow

            int randomIndexUnprotected = rand() % unprotectedCows.size();
            int unProtectedElement = unprotectedCows[randomIndexUnprotected];
            m_piecesTaken.push_back(unProtectedElement);
            m_gameBoard->removeCow(unProtectedElement);
            return unProtectedElement;
        }
    }

    return -1;
}


std::vector<int> Entity::getPiecesTaken(){

    return m_piecesTaken;
}


bool Entity::verifyNewMillFormed(){

    int newMills = 0;

    std::vector<int> myCows = m_gameBoard ->getCommonCowSlots(m_cow);
    std::vector<int> opponentCows = m_gameBoard->getCommonCowSlots(m_opponentCow);
    std::vector<int> opponentMilledCows = m_gameBoard->getUniqueCowsInMills(m_opponentCow);
    std::vector<int> myMilledCows = m_gameBoard ->getUniqueCowsInMills(m_cow);

    if(myMilledCows.size() < 3 || myMilledCows.empty()){ //There is no possible mill with only 2 cows

        return false;
    }

    for(int cowMill : myMilledCows){

        if(find(m_verifiedMills.begin(), m_verifiedMills.end(), cowMill) == m_verifiedMills.end()){ //Ensures only one mill is registered at a time

            newMills++;
            m_verifiedMills.insert(cowMill);
            m_currentMill.push_back(cowMill);
        }
    }

    if(newMills > 0){

        randomAttack();
        return true;
    }

    return false;

}


std::vector<int> Entity::getMoveHistory(){

    return m_moveHistory;
}


std::vector<int> Entity::getCurrentMill(){

    return m_currentMill;
}
