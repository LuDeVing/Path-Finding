# Pathfinding Visualization & Maze Generation 🚀🧩

This project demonstrates a visual implementation of pathfinding algorithms using the [OLC PixelGameEngine](https://www.onelonecoder.com/). It implements both Breadth First Search (BFS) and A* search algorithms along with a randomized maze generation feature. Written in C++, it shows step-by-step progress on grid-based pathfinding.

---

## Overview 📖

The application allows you to:

- **Place Start and End Points** 🎯  
  Define where the search begins and the goal is located.
  
- **Add Barriers** 🚧  
  Create obstacles the algorithm must navigate around.

- **Visualize Search Algorithms** 🔍  
  Watch both BFS and A* animate their progress as they find the shortest path.

- **Generate Random Mazes** 🎲  
  Automatically create random barriers that still allow a valid path between the start and end.

This interactive tool is perfect for learning about pathfinding, debugging algorithms, or simply enjoying visual demonstrations.

---

## Features ✨

- **Grid-Based Visualization**  
  The grid shows different block types with unique colors:
  
  - **Start Point**: Green 🟩  
  - **End Point**: Red 🟥  
  - **Barrier**: Gray ⬜️ (using shades of gray)  
  - **Empty Block**: White ⬜️  
  - **Visited Nodes**: Light Blue 🟦  
  - **Final Path**: Light Green 🟩

- **Pathfinding Algorithms**  
  - **Breadth First Search (BFS)**: Explores neighbors and finds the shortest route in an unweighted grid.
  - **A\*** Search: Uses heuristics (estimates) to efficiently find the best path.

- **Real-Time Animation**  
  The search process is animated, allowing you to see how the algorithms make their progress.

- **Mode Switching**  
  Toggle between **normal mode** (animated step-by-step) and **live mode** (instant updates).

- **Maze Generation**  
  Randomize the maze layout while ensuring a possible path exists between start and end.

---

## Setup and Compilation ⚙️

### Prerequisites

- **C++ Compiler**: The source code is compatible with C++11 (or later).
- **OLC PixelGameEngine**: Ensure `olcPixelGameEngine.h` is available in your project directory.  
  ([OLC PixelGameEngine GitHub Repository](https://github.com/OneLoneCoder/olcPixelGameEngine))

### Compilation Instructions

1. **Clone or Download** the repository that contains the source code.
2. Make sure `olcPixelGameEngine.h` is in the same folder as your source file.
3. Use microsoft visual studio to run the solution.

### Configuration Options ⚙️

The code has predefined compilation flags for different grid densities:

- **FEW_BLOCKS**: Fewer, larger blocks.
- **MED_BLOCKS**: Medium number of blocks (default).
- **MANY_BLOCKS**: Many, smaller blocks.

Select a mode by defining the corresponding macro either in your code or at compile time.

---

#### Controls ⌨️

While the application is running, use these keys for interaction:

- **1**: Set the selected cell as the **Start** point (Green) 🎯
- **2**: Set the selected cell as the **End** point (Red) 🎯
- **3**: Set the selected cell as a **Barrier** (Gray) 🚧
- **4**: **Erase** a cell (reset to White) ➖
- **S**: **Start** the search (switch from editor mode to search mode) ▶️
- **R**: **Reset** the grid and clear the current search/path 🔄
- **L**: Toggle between **Normal Mode** and **Live Mode** 🔄  
  _Normal Mode_: Search and path are animated step-by-step.  
  _Live Mode_: Recalculates and displays the full A\* search instantly.
- **F**: Toggle between BFS and A\* (Live Mode uses A\* by default) 🔀
- **G**: **Generate** a random maze with barriers (ensuring a valid path) 🎲

---

#### Visual Demonstrations 📷

- ![image](https://github.com/user-attachments/assets/05d62b60-49e1-42ed-8165-200538bf807e)

- ![image](https://github.com/user-attachments/assets/dbf1a11a-69ab-4486-af81-6258a000ef93)


---

#### Project Structure 🗂️

- **`main.cpp`**: Contains the main application code, including pathfinding and visualization logic.
- **`olcPixelGameEngine.h`**: Header file for the OLC PixelGameEngine (required to compile the application).
- **`README.md`**: This documentation file.
  
---

#### Acknowledgements 🙏

- **OLC PixelGameEngine**: Thanks to the developer for creating this versatile graphics engine.
- **Pathfinding Resources**: The project is inspired by various educational resources on BFS and A\* algorithms.

---

Enjoy exploring pathfinding and maze generation with this interactive tool! Happy coding! 💻😊
