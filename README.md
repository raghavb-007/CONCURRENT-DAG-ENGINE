# Concurrent DAG-Based Risk Aggregation Engine

A highly concurrent, multi-threaded computation engine written in **C++17** designed to execute complex, dependent mathematical models in parallel. 

This project bridges the gap between traditional algorithmic graph theory and systems engineering by utilizing **Kahn’s Algorithm for Topological Sorting** synchronized with an **OS-level Thread Pool** to maximize CPU core utilization during high-frequency data ticks.

## ⚙️ Core Architecture

The engine is built on two foundational pillars:
1. **Object-Oriented System Design (OOPS):** Uses an abstract Directed Acyclic Graph (DAG) architecture with runtime polymorphism. Dependent financial nodes (e.g., Stock Prices, Portfolio Aggregators) inherit from a base `CalculationNode` and execute specialized business logic dynamically.
2. **Operating System Primitives (OS):** Bypasses the overhead of dynamic thread creation by maintaining a fixed-size pool of user-space worker threads.

## 🚀 Key Features
* **Thread-Safe Task Queue:** Implements a multi-producer, multi-consumer synchronization model using `std::mutex` and `std::condition_variable` to safely distribute independent nodes across available CPU cores.
* **Zero Busy-Waiting:** Worker threads yield execution context to the OS scheduler when idle, completely eliminating CPU spin-lock waste.
* **Incremental Computation:** Traverses the DAG dynamically, pushing calculation nodes to the concurrent queue strictly when their in-degree dependency counters reach zero.

## 🛠️ Project Structure
The engine employs a modular, header-only architecture for clean dependency management:
* `Nodes.h` - Defines the abstract graph components and polymorphic pricing nodes.
* `TaskQueue.h` - Encapsulates the critical sections, lock guards, and OS condition variables.
* `Engine.h` - Manages the hardware-aware thread pool and executes the topological sort.
* `main.cpp` - The pipeline entry point simulating a dependent portfolio risk calculation.

## 💻 How to Compile and Run

This project requires a C++17 compatible compiler and standard POSIX thread support.

```bash
# Compile the engine with multithreading support
g++ -std=c++17 main.cpp -lpthread -o engine

# Execute the binary
./engine