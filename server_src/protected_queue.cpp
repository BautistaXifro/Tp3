#include "protected_queue.h"
#include <utility>
#include <vector>
#include <string>
#include <atomic>
ProtectedQueue::ProtectedQueue(){}

ProtectedQueue::ProtectedQueue(ProtectedQueue&& other){
    this->queueX = std::move(other.queueX);
    this->queueO = std::move(other.queueO);
}

void ProtectedQueue::push(const std::vector<int>& values, const char simbol){
    std::unique_lock<std::mutex> lock(this->queue_mutex);
    if (simbol == 'X'){
        this->queueO.push(values);
        this->cond_var.notify_all();
    }else{
        this->queueX.push(values);
        this->cond_var.notify_all();
    }
}

std::vector<int> ProtectedQueue::pop(const char simbol){
    std::unique_lock<std::mutex> lock(this->queue_mutex);
    std::vector<int> return_value;
    if (simbol == 'X'){
        while (this->queueX.empty()){
            this->cond_var.wait(lock);
        }
        return_value = this->queueX.front();
        this->queueX.pop();
    }else{
        while (this->queueO.empty()){
            this->cond_var.wait(lock);
        }
        return_value = this->queueO.front();
        this->queueO.pop();
    }
    return return_value;
}

ProtectedQueue::~ProtectedQueue(){}
