#ifndef __PROTECTED_QUEUE_H__
#define __PROTECTED_QUEUE_H__
#include <queue>
#include <vector>
#include <mutex>
#include <condition_variable>
#include <string>

class ProtectedQueue{
    private:
        std::queue<std::vector<int>> queueX;
        std::queue<std::vector<int>> queueO;
        std::mutex queue_mutex;
        std::condition_variable cond_var;
    public:
        ProtectedQueue();
        ProtectedQueue(ProtectedQueue&& other);
        ProtectedQueue(const ProtectedQueue& other) = delete;
        std::vector<int> popO();
        std::vector<int> popX();
        void pushO(const std::vector<int>& values);
        void pushX(const std::vector<int>& values);
        ~ProtectedQueue();
};
#endif
