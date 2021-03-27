# Afeka_AI_Course_HW1_Bidirectional_BFS
Solving a maze using Bidirectional BFS Algorithm.

## Summary
The following code implements Bidirectional BFS algorithm to find the shortest path from the Start point to the Target point in the maze.
* The maze is implemented using cells and OpenGL.
* The maze size can be changed; but the time to find the shortest path increases
* The solution is based on the BFS and OpenGL code of the AI course.
* The IDE used is Visual Studio 2019 Community

## How the code works
1. Maze Initialization
2. Set Start and Target points
3. Run BFS from the Start point and Target point at the same time.
4. When the BFS runs find an intersection gray point the BFS run is done.
5. The program restores the shortest path by traversing the parent cells
