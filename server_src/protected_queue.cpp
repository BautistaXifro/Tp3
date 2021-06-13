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

void ProtectedQueue::pushX(const std::vector<int>& values){
    std::unique_lock<std::mutex> lock(this->queue_mutex);
    this->queueX.push(values);
    this->cond_var.notify_all();
}

void ProtectedQueue::pushO(const std::vector<int>& values){
    std::unique_lock<std::mutex> lock(this->queue_mutex);
    this->queueO.push(values);
    this->cond_var.notify_all();
}

std::vector<int> ProtectedQueue::popX(){
    std::unique_lock<std::mutex> lock(this->queue_mutex);
    while (this->queueX.empty()){
        this->cond_var.wait(lock);
    }
    std::vector<int> return_value = this->queueX.front();
    this->queueX.pop();
    return return_value;
}

std::vector<int> ProtectedQueue::popO(){
    std::unique_lock<std::mutex> lock(this->queue_mutex);
    while (this->queueO.empty()){
        this->cond_var.wait(lock);
    }
    std::vector<int> return_value = this->queueO.front();
    this->queueO.pop();
    return return_value;
}

ProtectedQueue::~ProtectedQueue(){}
