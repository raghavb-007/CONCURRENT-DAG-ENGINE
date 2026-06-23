#pragma once
#include <vector>
#include <string>
#include <atomic>
#include <memory>
#include <thread>
#include <chrono>

using namespace std;

class CalculationNode {
public:
    string name;
    atomic<int> in_degree{0};
    vector<shared_ptr<CalculationNode>> children;
    double result = 0.0;

    CalculationNode(string node_name) : name(node_name) {}
    virtual ~CalculationNode() = default;

    virtual void calculate() = 0;
};

class StockPriceNode : public CalculationNode {
public:
    double initial_price;
    StockPriceNode(string name, double price) : CalculationNode(name), initial_price(price) {}

    void calculate() override {
        this_thread::sleep_for(chrono::milliseconds(200));
        result = initial_price * 1.05;
    }
};

class PortfolioRiskNode : public CalculationNode {
private:
    vector<shared_ptr<CalculationNode>> dependencies;
public:
    PortfolioRiskNode(string name, vector<shared_ptr<CalculationNode>> deps) 
        : CalculationNode(name), dependencies(deps) {}

    void calculate() override {
        this_thread::sleep_for(chrono::milliseconds(300));
        double total = 0;
        for (const auto& dep : dependencies) {
            total += dep->result;
        }
        result = total * 0.15;
    }
};