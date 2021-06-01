#ifndef __BOARD_H__
#define __BOARD_H__
#include <string>
#include <vector>
#include <atomic>
#include <string>
#include <condition_variable>


class Board{
    private:
        std::string firstLine;
        std::string separator;
        std::vector<std::string> rows;
        int positions[3] = {4,8,12};
        std::vector<std::vector<char>> matrix;

        std::condition_variable cond_var;
        std::atomic<bool> isMyTurn;
        std::atomic<bool> allowedToPrint;
        std::mutex board_mutex;
    public:
        Board();
        Board(Board&& other);
        void put(const int& column, const int& row, const char& charToInsert);
        int print(std::string& buffer, const char& simbol);
        std::string scan(const char& simbol);
        ~Board();
};
#endif
