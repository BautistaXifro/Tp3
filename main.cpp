#include <iostream>
#include <vector>
#include <string>
#include "board.h"

int main(){

    Board board;
    int aux = 1;
    int winner;
    while((winner = board.scan()) == 0){
        int row, column;
        char charToInsert;
        if(aux % 2 != 0){
            charToInsert = 'X';
        }else{
            charToInsert = 'O';
        }
        aux++;
        board.print();

        std::cout << "Inserte Fila: \n";
        std::cin >> row;

        std::cout << "Inserte columna: \n";
        std::cin >> column;

        board.put(row,column,charToInsert);
    }
    board.print();
    if(winner == 1){
        std::cout << "Primer player ha ganado\n";
    }else if(winner == 2){
        std::cout << "Segundo player ha ganado\n";
    }else{
        std::cout << "Empate\n";
    }

    return 0;
}