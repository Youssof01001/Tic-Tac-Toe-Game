# Tic-Tac-Toe Game

![Build & Test](https://github.com/Youssof01001/Tic-Tac-Toe-Game/actions/workflows/build.yml/badge.svg)

A simple Tic Tac Toe game with a smart AI opponent built using **Qt6**.  
Includes full unit test coverage using **Google Test**, and runs automatically with **GitHub Actions** CI.

---

## 🎮 Features

- ✅ Play against AI (Easy / Medium / Hard)
- ✅ Qt6 GUI
- ✅ Smart AI with Minimax algorithm
- ✅ Unit-tested logic
- ✅ CI/CD with GitHub Actions

---

## 🚀 How to Build

### 🔧 Requirements

- CMake ≥ 3.16
- Qt 6.x (Core + Widgets)
- Ninja (or `make`)
- C++17 compatible compiler

### 🧱 Build Steps

```bash
git clone https://github.com/Youssof01001/Tic-Tac-Toe-Game.git
cd Tic-Tac-Toe-Game
cmake -S . -B build -G Ninja -DCMAKE_BUILD_TYPE=Debug
cmake --build build