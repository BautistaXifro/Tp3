#include "board.h"
#include <iostream>
#include <string>
#include <vector>


Board::Board(){
    this->firstLine = "    1 . 2 . 3 .\n";
    this->separator = "  +---+---+---+\n";
    std::string firstRow= "1 |   |   |   |\n";
    std::string secondRow= "2 |   |   |   |\n";
    std::string thirdRow = "3 |   |   |   |\n";
    this->rows.push_back(firstRow);
    this->rows.push_back(secondRow);
    this->rows.push_back(thirdRow);
    for(int i = 0; i < 3; i++){
        std::vector<char> vector;
        this->matrix.push_back(vector);
        for(int j = 0; j < 3; j++){
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

void Board::put(int& column, int& row, const char& charToInsert){
    std::unique_lock<std::mutex> lock(this->board_mutex);

    this->matrix[row][column] = charToInsert;
    this->rows[row].replace(this->positions[column], 1, 1, charToInsert);

    this->allowedToPrint = true;
    this->isMyTurn = true;
    this->cond_var.notify_all();
    while (this->isMyTurn.load()){
       this->cond_var.wait(lock);
    }
    this->isMyTurn = true;
}


std::string Board::scan(const char& simbol){
    int Winner, blankSpace, totalBlancks;
    std::string finish_line;
    Winner = blankSpace = totalBlancks = 0;
    for (int i = 0; i < 3; i++){
        for(int j = 0; j < 3; j++){
            if (this->matrix[i][j] == simbol){
                Winner++;
            } else if(this->matrix[i][j] == ' '){
                blankSpace++;
                totalBlancks++;
                break;
            }
        }
        if (Winner == 3) return "Felicitaciones! Ganaste!\n";
        if (blankSpace == 0 && Winner == 0) return "Has perdido. Segui intentandolo!\n";
        Winner = 0;
        blankSpace = 0;
    }

    for(int i = 0; i < 3; i++){
        for(int j = 0; j < 3; j++){
            if(this->matrix[j][i] == simbol){
                Winner++;
            
            }else if(this->matrix[j][i] == ' '){
                blankSpace++;
                totalBlancks++;
                break;
            }
        }
        if (Winner == 3) return "Felicitaciones! Ganaste!\n";
        if (blankSpace == 0 && Winner == 0) return "Has perdido. Segui intentandolo!\n";
        Winner = 0;
        blankSpace = 0;
    }

    for(int i = 0; i < 3; i++){
        if (this->matrix[i][i] == simbol){
            Winner++;
        }else if (this->matrix[i][i] == ' '){
            blankSpace++;
            totalBlancks++;
            break;
        }
    }
    if (Winner == 3) return "Felicitaciones! Ganaste!\n";
    if (blankSpace == 0 && Winner == 0) return "Has perdido. Segui intentandolo!\n";
    Winner = 0;
    blankSpace = 0;
    int j = 2;
    for(int i = 0; i < 3; i++){
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
    if (blankSpace == 0 && Winner == 0) return "Has perdido. Segui intentandolo!\n";
    if (totalBlancks == 0) return "La partida ha terminado en empate\n";

    return "";
}

int Board::print(std::string& buffer, const char& simbol){
    std::unique_lock<std::mutex> lock(this->board_mutex);
    while (!this->allowedToPrint.load()){
        this->cond_var.wait(lock);
    }
    std::string end_line = scan(simbol);
    buffer = buffer + this->firstLine + this->separator +  this->rows[0] + this->separator
    + this->rows[1] + this->separator + this->rows[2] + this->separator + end_line;
    this->allowedToPrint = false;
    this->isMyTurn = false;
    this->cond_var.notify_all();
    if(end_line != ""){
        this->allowedToPrint = true;
        return 1;
    }
    return 0;
}

Board::~Board(){}
