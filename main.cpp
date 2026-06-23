#include <iostream>
#include <vector>
#include <memory>
#include "Nodes.h"
#include "Engine.h"

using namespace std;

int main() {
    cout << "Initializing Portfolio Risk Dependency Graph...\n\n";

    auto stockA = make_shared<StockPriceNode>("Apple_Stock_Node", 150.0);
    auto stockB = make_shared<StockPriceNode>("Google_Stock_Node", 2800.0);

    vector<shared_ptr<CalculationNode>> portfolio_deps = {stockA, stockB};
    auto riskNode = make_shared<PortfolioRiskNode>("Total_Portfolio_Risk_Node", portfolio_deps);

    stockA->children.push_back(riskNode);
    riskNode->in_degree++;

    stockB->children.push_back(riskNode);
    riskNode->in_degree++;

    vector<shared_ptr<CalculationNode>> graph = {stockA, stockB, riskNode};

    Engine engine(graph);
    engine.run();

    cout << "\nAll dependency computations completed successfully.\n";
    return 0;
}