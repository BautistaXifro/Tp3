#include "protected_map.h"
#include <mutex>
#include <string>
#include <map>
#include <iostream>
#include <atomic>
#include <utility>

int ProtectedMap::find(const std::string& key, ProtectedQueue*& value){
    std::unique_lock<std::mutex> lock(this->protected_map_mutex);
    std::map<std::string, ProtectedQueue>::iterator iterator = 
                this->map.find(key);
    if (iterator != this->map.end()){
        value = &iterator->second;
        return 0;
    }
    return 1;
}

ProtectedQueue* ProtectedMap::insert(std::string& key){
    std::unique_lock<std::mutex> lock(this->protected_map_mutex);

    this->map.emplace(key, std::move(ProtectedQueue()));

    return &this->map.at(key);
}

void ProtectedMap::get_keys(std::string& buffer, const std::string& separator){
    std::unique_lock<std::mutex> lock(this->protected_map_mutex);
    std::map<std::string, ProtectedQueue>::iterator iterator = 
                this->map.begin();
    while (iterator != this->map.end()){
        buffer = buffer + separator + iterator->first;
        ++iterator;
    }
}
