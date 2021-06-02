#include "protected_queue.h"
#include <utility>
#include <vector>
#include <string>
#include <atomic>

ProtectedQueue::ProtectedQueue(){}

ProtectedQueue::ProtectedQueue(ProtectedQueue&& other){
    this->queue = std::move(other.queue);
}

//PRE: el string enviado tiene la forma de una URL.
void ProtectedQueue::push(const std::vector<int>& values){
    std::unique_lock<std::mutex> lock(this->queue_mutex);
    this->queue.push(values);
    this->cond_var.notify_all();
    while (!this->queue.empty()){
       this->cond_var.wait(lock);
    }
}

std::vector<int> ProtectedQueue::pop(){
    std::unique_lock<std::mutex> lock(this->queue_mutex);
    while (this->queue.empty()){
       this->cond_var.wait(lock);
    }
    std::vector<int> return_value = this->queue.front();
    this->queue.pop();
    this->cond_var.notify_all();
    return return_value;
}

ProtectedQueue::~ProtectedQueue(){}
