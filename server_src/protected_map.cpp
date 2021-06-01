#include "protected_map.h"
#include <mutex>
#include <string>
#include <map>
#include <iostream>
#include <atomic>
#include <utility>

void ProtectedMap::find(const std::string& key, Board*& value){
    std::unique_lock<std::mutex> lock(this->protected_map_mutex);
    std::map<std::string, Board>::iterator iterator = 
                this->map.find(key);
    if (iterator != this->map.end()){
        value = &iterator->second;
        return;
    }
    std::cerr << "Error al encontrar la clave en el mapa\n";
}

void ProtectedMap::insert(std::string& key){
    std::unique_lock<std::mutex> lock(this->protected_map_mutex);

    this->map.emplace(key, std::move(Board()));
}

void ProtectedMap::get_keys(std::string& buffer, const std::string& separator){
    std::unique_lock<std::mutex> lock(this->protected_map_mutex);
    std::map<std::string, Board>::iterator iterator = 
                this->map.begin();
    while (iterator != this->map.end()){
        buffer = buffer + separator + iterator->first;
        ++iterator;
    }
}
