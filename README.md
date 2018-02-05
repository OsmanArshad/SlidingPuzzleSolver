# SlidingPuzzleSolver

## Description
This is a simple sliding puzzle solver that uses a search algorithm aided by heuristic functions to solve the puzzle. The user is presented with three search options: Uniform Cost Search, A* Search using Misplaced Tile, and A* Search using Manhattan Distance.

Uniform Cost Search is treated as a A* Search without a heuristic function. 

A* Search using Misplaced Tile works by counting the total number of tiles that are not in their correct goal positions currently, calculating that same value for each move that is possible to take, and then taking the move that gives the lowest misplaced tile value. 

A* Search using Manhattan distance works by summing the distances each individual tile is from it's goal position, and comparing that value with the Manhattan distance value for the next potential move.

The output generates each move the puzzle takes and will tell you the total number of tile nodes generated, the maximum number of nodes in the queue at any one time, and the depth of the goal.

## Output
