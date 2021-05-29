#include "protected_map.h"
#include <mutex>
#include <string>
#include <map>
#include <iostream>

void ProtectedMap::find(const std::string& key, Board*& value){
    std::map<std::string, Board>::iterator iterator = 
                this->map.find(key);
    if (iterator != this->map.end()){
        value = &iterator->second;
        return;
    }
    std::cerr << "Error al encontrar la clave en el mapa\n";
}

void ProtectedMap::insert(const std::string& key){
    this->map.insert(std::pair<std::string,
            Board>(key, Board()));
}

void ProtectedMap::get_keys(std::string& buffer, const std::string& separator){
    std::map<std::string, Board>::iterator iterator = 
                this->map.begin();
    while (iterator != this->map.end()){
        buffer = buffer + separator + iterator->first;
        iterator++;
    }
}
