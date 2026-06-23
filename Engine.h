#pragma once
#include <iostream>
#include <vector>
#include <thread>
#include "Nodes.h"
#include "TaskQueue.h"

using namespace std;

class Engine {
private:
    vector<thread> pool;
    TaskQueue queue;
    vector<shared_ptr<CalculationNode>> all_nodes;

    void worker_thread(int thread_id) {
        while (true) {
            auto node = queue.pop();
            if (!node) break;

            static mutex log_mtx;
            {
                lock_guard<mutex> lock(log_mtx);
                cout << "[Thread " << thread_id << "] Processing: " << node->name << "\n";
            }

            node->calculate();

            {
                lock_guard<mutex> lock(log_mtx);
                cout << "[Thread " << thread_id << "] Finished: " << node->name << " | Result: " << node->result << "\n";
            }

            for (auto& child : node->children) {
                if (--child->in_degree == 0) {
                    queue.push(child);
                }
            }
        }
    }

public:
    Engine(vector<shared_ptr<CalculationNode>> nodes) : all_nodes(nodes) {
        unsigned int num_threads = thread::hardware_concurrency();
        if (num_threads == 0) num_threads = 4;

        for (unsigned int i = 0; i < num_threads; ++i) {
            pool.push_back(thread(&Engine::worker_thread, this, i));
        }
    }

    void run() {
        for (auto& node : all_nodes) {
            if (node->in_degree == 0) {
                queue.push(node);
            }
        }

        bool processing = true;
        while (processing) {
            this_thread::sleep_for(chrono::milliseconds(50));
            processing = false;
            for (const auto& node : all_nodes) {
                if (node->in_degree > 0) {
                    processing = true;
                    break;
                }
            }
        }

        queue.stop();
        for (auto& t : pool) {
            if (t.joinable()) t.join();
        }
    }
};