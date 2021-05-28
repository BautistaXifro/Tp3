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
}

void Board::put(int& row, int& column, char charToInsert){
    this->matrix[row][column] = charToInsert;
    this->rows[row].replace(this->positions[column], 1, 1, charToInsert);
}

int Board::scan(){
    int xWinner, oWinner, draw;
    xWinner = oWinner = draw = 0;
    for (int i = 0; i < 3; i++){
        for(int j = 0; j < 3; j++){
            if (this->matrix[i][j] == 'X'){
                xWinner++;
            } else if(this->matrix[i][j] == 'O'){
                oWinner++;
            }else{
                draw++;
            }
        }
        if (xWinner == 3) return 1;
        if (oWinner == 3) return 2;
        xWinner = 0;
        oWinner = 0;
    }

    for(int i = 0; i < 3; i++){
        for(int j = 0; j < 3; j++){
            if(this->matrix[j][i] == 'X'){
                xWinner++;
            
            }else if(this->matrix[j][i] == 'O'){
                oWinner++;
            }else{
                draw++;
            }
        }
        if (xWinner == 3) return 1;
        if (oWinner == 3) return 2;
        xWinner = 0;
        oWinner = 0;
    }

    for(int i = 0; i < 3; i++){
        if (this->matrix[i][i] == 'X'){
            xWinner++;
        }else if (this->matrix[i][i] == 'O'){
            oWinner++;
        }else{
            draw++;
        }
    }
    if (xWinner == 3) return 1;
    if (oWinner == 3) return 2;
    xWinner = 0;
    oWinner = 0;
    int j = 2;
    for(int i = 0; i < 3; i++){
        if (this->matrix[i][j] == 'X'){
            xWinner++;
        }else if (this->matrix[i][j] == 'O'){
            oWinner++;
        }else{
            draw++;
        }
        j--;
    }
    if (xWinner == 3) return 1;
    if (oWinner == 3) return 2;
    if (draw == 0) return 3; //empate

    return 0;
}

void Board::print(){
    std::cout << this->firstLine + this->separator +  this->rows[0] + this->separator
    + this->rows[1] + this->separator + this->rows[2] + this->separator << std::endl;
}

Board::~Board(){}
