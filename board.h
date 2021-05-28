#ifndef __BOARD_H__
#define __BOARD_H__
#include <string>
#include <vector>

class Board{
    private:
        std::string firstLine;
        std::string separator;
        std::vector<std::string> rows;
        int positions[3] = {4,8,12};
        std::vector<std::vector<char>> matrix;
    public:
        Board();
        void put(int& row, int& column, char charToInsert);
        void print();
        int scan();
        ~Board();
};
#endif
