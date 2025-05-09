
# Sliding Puzzle Solver (k²–1 Puzzle) 🧩

This project implements an efficient solver for the classic **sliding tile puzzle** (e.g., 8-puzzle, 15-puzzle) using **Breadth-First Search (BFS)** and a **custom hash table** for detecting repeated states.

## Features
- Solves any solvable k×k sliding puzzle (e.g., 3x3, 4x4, 5x5)
- Implements BFS with a dynamically allocated queue
- Uses a custom hashing system to track visited states and prevent cycles
- Checks for puzzle solvability before attempting a solution
- Outputs the shortest sequence of tile moves to solve the puzzle

## How It Works
1. **Input Parsing:** Reads a puzzle from an input file.
2. **Solvability Check:** Uses inversion counting to verify if the puzzle is solvable.
3. **BFS Algorithm:** Explores the puzzle state space level-by-level.
4. **Hash Table:** Remembers visited states using hashing and linked lists to avoid revisiting.
5. **Solution Output:** Prints the sequence of tile values that must be moved into the empty space to solve the puzzle.

## Usage

### 📥 Input Format
The input and output formats are given in the txt files

