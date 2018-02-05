# SlidingPuzzleSolver

## Description
This is a simple sliding puzzle solver that uses a search algorithm aided by heuristic functions to solve the puzzle. The user can enter his own puzzle or use a default puzzle. Additionally, the user is presented with three search options: Uniform Cost Search, A* Search using Misplaced Tile, and A* Search using Manhattan Distance.

Uniform Cost Search is treated as a A* Search without a heuristic function. 

A* Search using Misplaced Tile works by counting the total number of tiles that are not in their correct goal positions currently, calculating that same value for each move that is possible to take, and then taking the move that gives the lowest misplaced tile value. 

A* Search using Manhattan distance works by summing the distances each individual tile is from it's goal position, and comparing that value with the Manhattan distance value for the next potential move.

The output generates each move the puzzle takes and will tell you the total number of tile nodes generated, the maximum number of nodes in the queue at any one time, and the depth of the goal.

Certain puzzles can take a rather long time to solve, and impossible to solve puzzles will run until memory runs out.

## Output
### Default puzzle solved using A* Search with Misplaced Tile
![slidingpuzzledefaultsolve](https://user-images.githubusercontent.com/35906533/35830265-7500e2fc-0a7a-11e8-9da5-373c16487644.PNG)

### User inputted puzzle solved using A* Search with Manhattan Distance
![slidingpuzzleinputsolve](https://user-images.githubusercontent.com/35906533/35830269-7a571a46-0a7a-11e8-9fef-7ff56b3bc34d.PNG)
