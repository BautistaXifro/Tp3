#ifndef __PROTECTED_MAP_H__
#define __PROTECTED_MAP_H__
#include <mutex>
#include <string>
#include <map>
#include "protected_queue.h"
class ProtectedMap {
    private:
        std::map<std::string, ProtectedQueue> map;
        std::mutex protected_map_mutex;
    public:
        int find(const std::string& key, ProtectedQueue*& value);
        ProtectedQueue* insert(std::string& key);
        void get_keys(std::string& buffer, const std::string& separator);
};
#endif
