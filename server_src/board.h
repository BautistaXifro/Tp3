#ifndef __BOARD_H__
#define __BOARD_H__
#include <string>
#include <vector>
#include <string>


class Board{
    private:
        std::string firstLine;
        std::string separator;
        std::vector<std::string> rows;
        int positions[3] = {4,8,12};
        std::vector<std::vector<char>> matrix;
    public:
        Board();
        Board(Board&& other);
        int put(const int& column, const int& row, const char& charToInsert);
        int print(std::string& buffer, const char& simbol);
        std::string scan(const char& simbol);
        ~Board();
};
#endif
