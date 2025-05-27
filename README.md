# Conway’s Game of Life – PA Project

[![Author](https://img.shields.io/badge/author-mihnea--alexandru06-blue)](https://github.com/mihnea-alexandru06)

A **straight‑C** showcase of data‑structure‑heavy extensions to **Conway’s Game of Life**, written for the *Proiectarea Algoritmilor* course. It walks through four increasingly complex problems—from the classic cellular‑automaton simulation to discovering Hamiltonian paths—while exercising linked lists, stacks, binary trees and graphs.

---

## 📜 Table of Contents

1. [Features](#features)
2. [Project layout](#project-layout)
3. [Building](#building)
4. [Running](#running)
5. [How it works (quick peek)](#how-it-works-quick-peek)

---

## Features

* **Task 1 — Simulation**   Run the standard rules for *K* generations.
* **Task 2 — Delta stack**   Store only the cell flips between generations to reverse‑reconstruct the board.
* **Task 3 — Evolution tree**   Branch each generation with **standard** vs **custom** rules and print a preorder traversal.
* **Task 4 — Graph analysis**   Transform each generation into a graph of live‑cell neighbours and find the *longest* Hamiltonian path (lexicographically minimal).

---

## Project layout

```text
.
├── include/        # public headers
├── src/            # implementation files
├── Makefile        # build script
└── README.md
```

### Modules at a glance

| Task  | Main source(s)       | Purpose                                                             |
| ----- | -------------------- | ------------------------------------------------------------------- |
| **1** | `task1.c`            | Classic Game of Life for **K** generations                          |
| **2** | `task2.c`            | Saves per‑generation deltas in a *stack of lists*                   |
| **3** | `task3.c`, `tree.c`  | Builds and traverses an evolution tree (standard vs custom rules)   |
| **4** | `task4.c`, `graph.c` | Converts grids to graphs and derives the *longest Hamiltonian path* |

---

## Building

```bash
# Clone your fork
$ git clone https://github.com/mihnea-alexandru06/game-of-life-pa-project.git
$ cd game-of-life-pa-project

# Release build (default target)
$ make            # → ./game_of_life

# Optional helpers
$ make run        # runs for sample input
% make clean      # removes objects & binary
```

> **Requires:** a C11‑compatible compiler (e.g. `gcc`, `clang`) and `make` on Linux, macOS or WSL. No external libraries needed.

---

## Running

```bash
./game_of_life in1.txt out1.txt [in2.txt out2.txt ...]
```

*Supply one or more input/output file pairs.*

### Input format

```text
T N M K
<N rows of M chars: 'X' for alive, '+' for dead>
```

* `T` ∈ {1,2,3,4}  selects the task.
* `N × M`          board size.
* `K`              generations (depth).

### Output cheatsheet

| Task  | Output                                                     |
| ----- | ---------------------------------------------------------- |
| **1** | The grid after every generation (blank line separated)     |
| **2** | Stack of change‑lists able to reconstruct the initial grid |
| **3** | Preorder traversal of the evolution tree                   |
| **4** | Longest Hamiltonian path for each tree node                |

> **Sample datasets:** [https://gitlab.cs.pub.ro/paaa/project-data](https://gitlab.cs.pub.ro/paaa/project-data)

---

## How it works (quick peek)

* **Flat grid** — cells live in a 1‑D char array; helpers in `grid.c` do index math & state flips.
* **Sparse delta stack** — Task 2 records only cells that change, slashing memory on sparse boards.
* **Two‑rule branching** — Task 3 explores both rule sets simultaneously by branching a binary tree.
* **Hamiltonian search** — Task 4 finds connected components, then back‑tracks DFS with pruning to deliver the lexicographically minimal longest path.
