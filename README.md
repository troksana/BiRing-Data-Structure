## Bidirectional Circular Doubly-Linked List (Bi-Ring) in C++

A custom, low-level implementation of a bidirectional ring (circular doubly-linked list) data structure in native C++. This project demonstrates foundational computer science concepts, explicit pointer manipulation, dynamic memory management, and automated unit testing.
==============================================================
## Features

* **Circular Architecture:** The final node seamlessly links back to the head node, allowing for infinite bidirectional traversal without hitting a `nullptr`.
* **Bidirectional Navigation:** Each node maintains explicit pointers to both its successor (`next`) and predecessor (`prev`) nodes, enabling full two-way traversal.
* **Dynamic Memory Management:** Custom allocation and deallocation logic designed to prevent memory leaks and dangling pointers during structural mutations (inserts, deletes, splits, or joins).
* **Automated Unit Testing:** Includes a dedicated testing suite (`bi_ring_test.hpp`) to validate edge cases, boundaries, and overall structural integrity.
==============================================================
## Tech Stack & Concepts Demonstrated

* **Language:** C++ (Template-ready / Type-safe structure design)
* **Concepts:** Pointer manipulation, Node-based memory allocation, Iterator design patterns, and Resource Acquisition Is Initialization (RAII).
* **IDE/Environment:** Developed using VS Code with automated compilation setups.
==============================================================
##  Project Component Breakdown

* `bi_ring.hpp` - Core interface and implementation containing the data structure logic, node definitions, and traversal algorithms.
* `bi_ring_test.hpp` - Test suite containing isolated unit tests designed to stress-test the container against regressions.
* `main.cpp` - Application entry point showcasing practical usage examples and executing the test pipeline.
==============================================================
## Design Philosophy

This project was built to mimic standard library container principles used in high-performance engines:
* High operational efficiency for element insertion and removal ($O(1)$ complexity once the node position is acquired).
* Strict adherence to memory safety principles by ensuring robust cleanups upon destruction.
* Clear code documentation separating core algorithm mechanics from test execution.
* ==============================================================
