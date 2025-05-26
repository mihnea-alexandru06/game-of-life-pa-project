# Conway’s Game of Life – PA Project

[![Author](https://img.shields.io/badge/author-mihnea--alexandru06-blue)](https://github.com/mihnea-alexandru06)

A straight‑C implementation of several algorithm‑heavy extensions to **Conway’s Game of Life**, developed for the *Proiectarea Algoritmilor* course.
The code showcases linked lists, stacks, binary trees and graphs to solve four progressively harder tasks – from the classic simulation to Hamiltonian‑path analysis.

---

## Project layout

```
.
├── include/        # public headers
├── src/            # implementation files
├── Makefile        # build script
└── README.md
```

### Modules at a glance

| Task  | Main source(s)       | Purpose                                                                      |
| ----- | -------------------- | ---------------------------------------------------------------------------- |
| **1** | `task1.c`            | Classic Game of Life for **K** generations                                   |
| **2** | `task2.c`            | Stores deltas between generations as a *stack of lists*                      |
| **3** | `task3.c`, `tree.c`  | Builds a full binary tree that branches on **standard vs. custom** rules     |
| **4** | `task4.c`, `graph.c` | Converts each generation to a graph and finds the *longest Hamiltonian path* |

---

## Building

```bash
# Clone your fork
git clone https://github.com/mihnea-alexandru06/game-of-life-pa-project.git
cd game-of-life-pa-project

# Build the release binary
make            # produces ./game_of_life

# Optional helpers
make run        # runs for sample input
make clean      # removes objects & binary
```

> **Prerequisites:** any C11‑compatible compiler (e.g. `gcc`, `clang`) and `make` on Linux, macOS or WSL.
> The project has **zero external dependencies** beyond the standard library.

---

## Running

```bash
./game_of_life in1.txt out1.txt [in2.txt out2.txt ...]
```

* The program consumes **pairs** of *input* / *output* files (at least one pair).

* Each input follows the assignment format:

  ```text
  T N M K
  <N lines of M characters: 'X' (alive) or '+' (dead)>
  ```

* **Task outputs**

  * **Task 1** – prints the grid for every generation, blank line separated.
  * **Task 2** – prints a stack of change‑lists able to reconstruct the initial grid.
  * **Task 3** – preorder traversal of the evolution tree (standard + custom rules).
  * **Task 4** – the longest Hamiltonian path for each node of the tree.

Example datasets are available at **[https://gitlab.cs.pub.ro/paaa/project-data](https://gitlab.cs.pub.ro/paaa/project-data)**.

---

## Internals (very short)

* **Grids** are stored as flat arrays; `grid.c` wraps indexing & state flips.
* **Task 2** records only the cells that change between generations, shrinking memory on sparse boards.
* **Task 3** splits the evolution with two rule‑sets (`RULES_STD`, `RULES_CUSTOM`) and streams a preorder traversal so no huge tree is kept in RAM.
* **Task 4** detects connected components, then runs a DFS back‑tracking search with pruning to obtain the lexicographically‑minimal longest Hamiltonian path.
