#include <iostream>
#include <set>
#include <vector>
#include <algorithm>
#include <cstdlib>
#include <ctime>
#include <fstream>
#include <iomanip>

#include "GameBoard.h"
#include "Entity.h"

using namespace std;

void writeToFile(Entity& player, GameBoard& board, ofstream& outputFile, char entity, int startingPieces, string phase, int *endCount, ofstream& boardFile, char player1Char);
bool endGame(GameBoard& board, int player1Pieces, int player2Pieces, int endCount, bool cannotMove1, bool cannotMove2 ,ofstream& outputFile, ofstream& logFile, ofstream& boardFile);


int main(){

    string inputFilename = "startingCows.txt";
    string outputFilename = "morabarabaResults.txt";
    string logFilename = "statsLog.txt";
    string boardFilename = "boardLog.txt";

    ifstream inputFile;
    ofstream outputFile;
    ofstream logFile;
    ofstream boardFile;

    inputFile.open(inputFilename);
    outputFile.open(outputFilename);
    logFile.open(logFilename);
    boardFile.open(boardFilename);

    char entity1 = 'X';
    char entity2 = 'Y';

    int startingPieces = 0;
    int piecesOffBoard = startingPieces;

    if(!inputFile.is_open()){

        return -1;
    }

    while(inputFile >> startingPieces){

        if(startingPieces > 12 || startingPieces < 3){

            outputFile << "ERROR: Invalid number of pieces ~ " << startingPieces;
            return -1;
        }

        int endCount = 0; //Checks for potential mills

        GameBoard board(boardFile);

        Entity player1(&board, entity1, entity2, startingPieces); //smart player
        Entity player2(&board, entity2, entity1, startingPieces); //random player

        piecesOffBoard = startingPieces;

        outputFile << startingPieces << '\n';

        boardFile << "###############################################" << '\n'
                  << "##                                           ##" << '\n'
                  << "##               NEW GAME                    ##" << '\n'
                  << "##       Number of starting cows:   "<< piecesOffBoard<<"        ##" << '\n'
                  << "##                                           ##" << '\n'
                  << "###############################################" << '\n';

        board.displayGameBoard();

        boardFile << "\n--------------------    Placement Phase" << endl;

        //Placement phase
        while(piecesOffBoard > 0){

            player1.strategicPlacement();

            writeToFile(player1, board,outputFile, entity1, startingPieces, "placement", &endCount, boardFile, entity1);

            player2.randomPlacement();

            writeToFile(player2,board,outputFile,entity2,startingPieces, "placement", &endCount, boardFile, entity1);

            piecesOffBoard--;
        }

        boardFile << "--------------------    Movement Phase\n" << endl;

        //Movement phase
        while(!endGame(board,startingPieces - player1.getPiecesTaken().size(), startingPieces - player2.getPiecesTaken().size(), endCount, player1.cannotMakeMove(), player2.cannotMakeMove(),outputFile,logFile,boardFile)){

            player1.moveStrategicPiece();

            writeToFile(player1, board,outputFile, entity1, startingPieces, "movement", &endCount, boardFile, entity1);

            if(endGame(board,startingPieces - player1.getPiecesTaken().size(), startingPieces - player2.getPiecesTaken().size(), endCount, player1.cannotMakeMove(), player2.cannotMakeMove(),outputFile,logFile,boardFile)){

                break;
            }

            player2.moveRandomPiece();

            writeToFile(player2,board,outputFile,entity2,startingPieces, "movement", &endCount, boardFile, entity1);

            if(endGame(board,startingPieces - player1.getPiecesTaken().size(), startingPieces - player2.getPiecesTaken().size(), endCount, player1.cannotMakeMove(), player2.cannotMakeMove(),outputFile,logFile,boardFile)){

                break;
            }

            //Check for events that may cause a draw
            if((startingPieces - player1.getPiecesTaken().size()) == (startingPieces - player2.getPiecesTaken().size())){
                endCount++;
            }

            if((startingPieces - player1.getPiecesTaken().size() == 3) && (startingPieces - player2.getPiecesTaken().size() == 3)){
                endCount++;
            }
        }

        //Displays the number of moves each player has made to the logFile for convenient analysis
        logFile << "Player 1 moves: "<< (startingPieces + player1.getMoveHistory().size()/2) - 2 << '\n'  //Divide by two, since removed cows are counted during the movement phase
             << "Player 2 moves: "<< (startingPieces + player2.getMoveHistory().size()/2) - 2 << '\n'
             << endl;

        endCount = 0;
        board.resetBoard();
    }

    inputFile.close();
    outputFile.close();
    logFile.close();
    boardFile.close();

    return 0;
}


//Function checks if the game is over and outputs the relevant information to the files, such as the winner
bool endGame(GameBoard& board, int player1Pieces, int player2Pieces, int endCount, bool cannotMove1, bool cannotMove2 ,ofstream& outputFile, ofstream& logFile, ofstream& boardFile){

    if(player1Pieces == 2 || cannotMove2){

        outputFile << '\n';
        outputFile << "P1 wins" << '\n';
        outputFile << endl;

        logFile << "P1 wins" << endl;

        boardFile << '\n';
        boardFile << "P1 wins " << '\n';
        boardFile << endl;

        return true;
    }

    else if(player2Pieces == 2 || cannotMove1){

        outputFile << '\n';
        outputFile << "P2 wins" << '\n';
        outputFile << endl;

        logFile << "P2 wins" << endl;

        boardFile << '\n';
        boardFile << "P2 wins" << '\n';
        boardFile << endl;

        return true;
    }

    //Players each have 3 cows and neither form a mill within 10 moves. This is a draw
    else if(player1Pieces == 3 && player1Pieces == player2Pieces && endCount >= 10){

        outputFile << '\n';
        outputFile << "its a draw" << '\n';
        outputFile << endl;

        logFile << "its a draw" << endl;

        boardFile << '\n';
        boardFile << "its a draw" << '\n';
        boardFile << endl;

        return true;
    }

    else if(board.getFreeSlots().size() == 0){

        outputFile << '\n';
        outputFile << "its a draw" << '\n';
        outputFile << endl;

        logFile << "its a draw" << endl;

        boardFile << '\n';
        boardFile << "its a draw" << '\n';
        boardFile << endl;

        return true;
    }

    //If Players each have equal number of cows and neither form a mill within 700 moves. consider it a draw.
    else if(player1Pieces == player2Pieces && endCount >= 700){

        outputFile << '\n';
        outputFile << "its a draw" << '\n';
        outputFile << endl;

        logFile << "Draw ~ moves: " << endCount << " ~ max moves exceeded" << endl;

        boardFile << '\n';
        boardFile << "Draw ~ moves: " << endCount << " ~ max moves exceeded" << endl;

        return true; //Game over
    }

    return false;
}


//Writes data to the relevant files in the desired format.
void writeToFile(Entity& player, GameBoard& board, ofstream& outputFile, char entity, int startingPieces, string phase, int *endCount, ofstream& boardFile, char player1Char){

    string attackedChar;

    string placementChar;

    vector<int> moveHistory = player.getMoveHistory();

    vector<int> millsOnBoard = board.mills(entity);

    if(entity == player1Char){

        placementChar = "P1";
        attackedChar = "P2";

    }else{

        placementChar = "P2";
        attackedChar = "P1";
    }

    if(phase == "movement"){

        outputFile << placementChar
                   << " "
                   << board.convertToIntersection(moveHistory[moveHistory.size() - 2])
                   << "-"
                   << board.convertToIntersection(moveHistory[moveHistory.size() - 1]);
    }

    else if(phase == "placement"){

        outputFile << placementChar
                   << " 00-"
                   << board.convertToIntersection(moveHistory[moveHistory.size() - 1]);

    }

    bool newValidMill = player.verifyNewMillFormed();

    boardFile << "Mills formed: " << newValidMill << '\n';

    int cowInMill1;
    int cowInMill2;
    int cowInMill3;

    int playerLastMove = moveHistory[moveHistory.size() - 1];

    if(newValidMill){

        for(int i = 0; i < static_cast<int>(millsOnBoard.size()); i = i + 3){

            if(playerLastMove == millsOnBoard[i] || playerLastMove == millsOnBoard[i+1] || playerLastMove == millsOnBoard[i+2]){

                cowInMill1 = millsOnBoard[i];
                cowInMill2 = millsOnBoard[i+1];
                cowInMill3 = millsOnBoard[i+2];
                break;
            }
        }


        outputFile << " (mill = "   << board.convertMillToIntersection(cowInMill1)
                                    << ", "
                                    << board.convertMillToIntersection(cowInMill2)
                                    << ", "
                                    << board.convertMillToIntersection(cowInMill3)
                                    << ") "
                                    << attackedChar << " losses cow (" << board.convertToIntersection(player.getPiecesTaken()[player.getPiecesTaken().size()-1])
                                    << ") | "
                                    << startingPieces-player.getPiecesTaken().size() << " left";

        *endCount = 0; //Reset the game over counter, since a valid mill has been made

    }

    board.displayGameBoard();

    outputFile << endl;
}
