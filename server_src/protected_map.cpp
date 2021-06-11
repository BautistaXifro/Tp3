#include "protected_map.h"
#include <mutex>
#include <string>
#include <map>
#include <iostream>
#include <utility>
#include <condition_variable>

void ProtectedMap::find(const std::string& key, ProtectedQueue*& value){
    std::unique_lock<std::mutex> lock(this->protected_map_mutex);
    std::map<std::string, ProtectedQueue>::iterator iterator;
    while ((iterator = this->map.find(key)) == this->map.end()){
        this->cond_var.wait(lock);
    }
    value = &iterator->second;
}

ProtectedQueue* ProtectedMap::insert(std::string& key){
    std::unique_lock<std::mutex> lock(this->protected_map_mutex);

    this->map.emplace(key, std::move(ProtectedQueue()));

    this->cond_var.notify_all();
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
