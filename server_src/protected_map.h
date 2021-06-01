#ifndef __PROTECTED_MAP_H__
#define __PROTECTED_MAP_H__
#include <mutex>
#include <string>
#include <map>
#include "board.h"
class ProtectedMap {
    private:
        std::map<std::string, Board> map;
        std::mutex protected_map_mutex;
    public:
        void find(const std::string& key, Board*& value);
        void insert(std::string& key);
        void get_keys(std::string& buffer, const std::string& separator);
};
#endif