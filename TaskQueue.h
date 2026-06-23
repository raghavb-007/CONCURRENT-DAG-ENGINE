#pragma once
#include <queue>
#include <mutex>
#include <condition_variable>
#include <memory>
#include "Nodes.h"

using namespace std;

class TaskQueue {
private:
    queue<shared_ptr<CalculationNode>> tasks;
    mutex mtx;
    condition_variable cv;
    bool stop_flag = false;

public:
    void push(shared_ptr<CalculationNode> node) {
        {
            lock_guard<mutex> lock(mtx);
            tasks.push(node);
        }
        cv.notify_one();
    }

    shared_ptr<CalculationNode> pop() {
        unique_lock<mutex> lock(mtx);
        cv.wait(lock, [this]() { return !tasks.empty() || stop_flag; });
        
        if (stop_flag && tasks.empty()) return nullptr;
        
        auto node = tasks.front();
        tasks.pop();
        return node;
    }

    void stop() {
        {
            lock_guard<mutex> lock(mtx);
            stop_flag = true;
        }
        cv.notify_all();
    }
};