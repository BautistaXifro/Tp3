#include "board.h"
#include <iostream>
#include <string>
#include <vector>
#include <utility>

Board::Board(){
    this->firstLine = "    1 . 2 . 3 .\n";
    this->separator = "  +---+---+---+\n";
    std::string firstRow= "1 |   |   |   |\n";
    std::string secondRow= "2 |   |   |   |\n";
    std::string thirdRow = "3 |   |   |   |\n";
    this->rows.push_back(firstRow);
    this->rows.push_back(secondRow);
    this->rows.push_back(thirdRow);
    for (int i = 0; i < 3; i++){
        std::vector<char> vector;
        this->matrix.push_back(vector);
        for (int j = 0; j < 3; j++){
            this->matrix[i].push_back(' ');
        }
    }
    this->isMyTurn = true;
    this->allowedToPrint = true;
}

Board::Board(Board&& other){
    this->firstLine = std::move(other.firstLine);
    this->separator = std::move(other.separator);
    this->rows = std::move(other.rows);
    this->matrix = std::move(other.matrix);
    this->isMyTurn = true;
    this->allowedToPrint = true;
}

int Board::put(const int& column, const int& row, const char& charToInsert){
    this->matrix[row][column] = charToInsert;
    this->rows[row].replace(this->positions[column], 1, 1, charToInsert);
    if (scan(charToInsert) != "") return 1;
    return 0;
}


std::string Board::scan(const char& simbol){
    int Winner, blankSpace, totalBlancks;
    Winner = blankSpace = totalBlancks = 0;
    for (int i = 0; i < 3; i++){
        for (int j = 0; j < 3; j++){
            if (this->matrix[i][j] == simbol){
                Winner++;
            } else if (this->matrix[i][j] == ' '){
                blankSpace++;
                totalBlancks++;
                break;
            }
        }
        if (Winner == 3) return "Felicitaciones! Ganaste!\n";
        if (blankSpace == 0 && Winner == 0)
             return "Has perdido. Segui intentando!\n";
        Winner = 0;
        blankSpace = 0;
    }

    for (int i = 0; i < 3; i++){
        for (int j = 0; j < 3; j++){
            if (this->matrix[j][i] == simbol){
                Winner++;
            }else if (this->matrix[j][i] == ' '){
                blankSpace++;
                totalBlancks++;
                break;
            }
        }
        if (Winner == 3) return "Felicitaciones! Ganaste!\n";
        if (blankSpace == 0 && Winner == 0)
             return "Has perdido. Segui intentando!\n";
        Winner = 0;
        blankSpace = 0;
    }

    for (int i = 0; i < 3; i++){
        if (this->matrix[i][i] == simbol){
            Winner++;
        }else if (this->matrix[i][i] == ' '){
            blankSpace++;
            totalBlancks++;
            break;
        }
    }
    if (Winner == 3) return "Felicitaciones! Ganaste!\n";
    if (blankSpace == 0 && Winner == 0)
         return "Has perdido. Segui intentando!\n";
    Winner = 0;
    blankSpace = 0;
    int j = 2;
    for (int i = 0; i < 3; i++){
        if (this->matrix[i][j] == simbol){
            Winner++;
        }else if (this->matrix[i][j] == ' '){
            blankSpace++;
            totalBlancks++;
            break;
        }
        j--;
    }
    if (Winner == 3) return "Felicitaciones! Ganaste!\n";
    if (blankSpace == 0 && Winner == 0) 
        return "Has perdido. Segui intentando!\n";
    if (totalBlancks == 0) return "La partida ha terminado en empate\n";

    return "";
}

int Board::print(std::string& buffer, const char& simbol){
    std::string end_line = scan(simbol);
    buffer = buffer + this->firstLine + this->separator 
    +  this->rows[0] + this->separator
    + this->rows[1] + this->separator + this->rows[2] 
    + this->separator + end_line;
    if (end_line != ""){
        return 1;
    }
    return 0;
}

Board::~Board(){}
