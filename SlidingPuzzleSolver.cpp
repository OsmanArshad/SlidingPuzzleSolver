/*
    Title:  SlidingPuzzleSolver.cpp
    Author: Osman Arshad
    Email:  osmanaarshad@gmail.com
    Description: SlidingPuzzleSolver.cpp solves the sliding puzzle for a 3x3 puzzle using a search algorithm 
    guided by one of three different heuristics; uniform cost, misplaced tile, or manhattan distance. Code
    can be modified to run on a larger sized puzzle. Output gives information regarding the number of nodes
    expanded searching for the solution, the maximum queue size at any one point, and the greatest depth at 
    any one point.
    
    Notes: Requires c++11 to compile, command for compile is: 
        g++ -o SlidingPuzzleSolver -std=c++11 SlidingPuzzleSolver.cpp
*/
#include <iostream>
#include <queue>
#include <cmath>
#include <string>
#include <set>
using namespace std;

// Global Variables
int PUZZLE_SIZE = 8;
int goalPuzzle[3][3] =
{
    {1,2,3},
    {4,5,6},
    {7,8,0}
};

// The Node class represents a single puzzle and contains information regarding the puzzle's state
class Node
{
    private:
        int nodePuzzle[3][3];
        int blankTileX;
        int blankTileY;
        int missingTileValue;
        int manhattanDistanceValue;
        int depth = 0;

    public:
        Node();
        Node(int arr[3][3])
        {
            for(int i = 0; i < 3; i++)
            {
                for(int j = 0; j < 3; j++)
                {
                    nodePuzzle[i][j] = arr[i][j];
                    if (nodePuzzle[i][j] == 0)
                    {
                        blankTileX = i;
                        blankTileY = j;
                    }
                }
            }
        }

        // returns the puzzle's current state as a string value
        string getPuzzle()
        {
            string puzzleKey = "";
            string stringPuzzle = "";
            for (int v = 0; v < 3; v++)
            {
                for (int t = 0; t < 3; t++)
                {
                    stringPuzzle = to_string(nodePuzzle[v][t]);
                    puzzleKey.append(stringPuzzle);
                }
            }
            return puzzleKey;
        }

        // checks if the puzzle's current state is the goal state
        bool isGoalState()
        {
            if (nodePuzzle[0][0] != 1) return false;
            if (nodePuzzle[0][1] != 2) return false;
            if (nodePuzzle[0][2] != 3) return false;
            if (nodePuzzle[1][0] != 4) return false;
            if (nodePuzzle[1][1] != 5) return false;
            if (nodePuzzle[1][2] != 6) return false;
            if (nodePuzzle[2][0] != 7) return false;
            if (nodePuzzle[2][1] != 8) return false;
            if (nodePuzzle[2][2] != 0) return false;

            return true;
        }

        void printPuzzle()
        {
            for (int x = 0; x < 3; x++)
            {
                for (int y = 0; y < 3; y++)
                {
                    cout << nodePuzzle[x][y] << "  ";
                }
                cout << "\n";
            }
        }

        int getBlankTileX() {   return blankTileX;  }
        int getBlankTileY() {   return blankTileY;  }

        int getDepth() const    {   return depth;   }
        void incrDepth()    {   depth++;    }

        int getMissingTileValue() const {   return missingTileValue;    }
        int getManhattanDistanceValue() const   {   return manhattanDistanceValue;  }
        
        void calculateMissingTileValue()
        {
            missingTileValue = 0;

            if (nodePuzzle[0][0] != 1) missingTileValue++;
            if (nodePuzzle[0][1] != 2) missingTileValue++;
            if (nodePuzzle[0][2] != 3) missingTileValue++;
            if (nodePuzzle[1][0] != 4) missingTileValue++;
            if (nodePuzzle[1][1] != 5) missingTileValue++;
            if (nodePuzzle[1][2] != 6) missingTileValue++;
            if (nodePuzzle[2][0] != 7) missingTileValue++;
            if (nodePuzzle[2][1] != 8) missingTileValue++;
            if (nodePuzzle[2][2] != 0) missingTileValue++;
        }

        void calculateManhattanDistanceValue()
        {
            int misplacedNumber;
            manhattanDistanceValue = 0;

            for (int i = 0; i < 3; i++)
            {
                for (int j = 0; j < 3; j++)
                {
                    if (nodePuzzle[i][j] != goalPuzzle[i][j])
                    {
                        misplacedNumber = nodePuzzle[i][j];
                        int x = i;
                        int y = j;

                        for (int k = 0; k < 3; k++)
                        {
                            for (int h = 0; h < 3; h++)
                            {
                                if (misplacedNumber == goalPuzzle[k][h])
                                {
                                    int a = k;
                                    int b = h;

                                    manhattanDistanceValue += abs(x - a) + abs(y - b);
                                }
                            }
                        }
                    }
                }
            }
        }

        void moveBlankTile(int dir)
        {
            // moving up
            if (dir == 1)
            {
                int tempUp;
                tempUp = nodePuzzle[blankTileX-1][blankTileY];
                nodePuzzle[blankTileX-1][blankTileY] = 0;
                nodePuzzle[blankTileX][blankTileY] = tempUp;
                
                blankTileX--;
            }

            // moving down
            if (dir == 2)
            {
                int tempDown;
                tempDown = nodePuzzle[blankTileX+1][blankTileY];
                nodePuzzle[blankTileX+1][blankTileY] = 0;
                nodePuzzle[blankTileX][blankTileY] = tempDown;
            
                blankTileX++;
            }

            // moving right 
            if (dir == 3)
            {
                int tempRight;
                tempRight = nodePuzzle[blankTileX][blankTileY+1];
                nodePuzzle[blankTileX][blankTileY+1] = 0;
                nodePuzzle[blankTileX][blankTileY] = tempRight;

                blankTileY++;
            }
            
            // moving left
            if (dir == 4)
            {
                int tempLeft;
                tempLeft = nodePuzzle[blankTileX][blankTileY-1];
                nodePuzzle[blankTileX][blankTileY-1] = 0;
                nodePuzzle[blankTileX][blankTileY] = tempLeft;

                blankTileY--;
            }
        }
};

// Takes in a node class object, and expands that puzzle by considering all possible moves to take
// Returns all new possible puzzle configurations in a queue
std::queue<Node> expandNode(Node n)
{
    std::queue<Node> q;

    int currBlankTileX = n.getBlankTileX();
    int currBlankTileY = n.getBlankTileY();

    // To move the blank tile up
    if (currBlankTileX != 0)
    {
        Node movUp = n;
        movUp.moveBlankTile(1);
        movUp.incrDepth();
        movUp.calculateMissingTileValue();
        movUp.calculateManhattanDistanceValue();
        q.push(movUp);
    }

    // To move the blank tile down
    if (currBlankTileX != 2)
    {
        Node movDown = n;
        movDown.moveBlankTile(2);
        movDown.incrDepth();
        movDown.calculateMissingTileValue();
        movDown.calculateManhattanDistanceValue();
        q.push(movDown);
    }

    // To moce the blank tile right
    if (currBlankTileY != 2)
    {
        Node movRight = n;
        movRight.moveBlankTile(3);
        movRight.incrDepth();
        movRight.calculateMissingTileValue();
        movRight.calculateManhattanDistanceValue();
        q.push(movRight);
    }

    // To move the blank tile left
    if (currBlankTileY != 0)
    {
        Node movLeft = n;
        movLeft.moveBlankTile(4);
        movLeft.incrDepth();
        movLeft.calculateMissingTileValue();
        movLeft.calculateManhattanDistanceValue();
        q.push(movLeft);
    }

    return q;
}

// // Orders priority queue nodes by their depth
struct UniformCostHeuristic : public binary_function <Node, Node, bool>                                                                                
{
	bool operator()(const Node& n1, const Node& n2) const
	{
		return n1.getDepth() > n2.getDepth();
	}
};

// Orders priority queue nodes by their missing tile value
struct MissingTileHeuristic : public binary_function <Node, Node, bool>                                                                                
{
	bool operator()(const Node& n1, const Node& n2) const
	{
		return n1.getMissingTileValue() + n1.getDepth() > n2.getMissingTileValue() + n2.getDepth();
	}
};

// Orders priority queue nodes by their manhattan distance value
struct ManhattanDistanceHeuristic : public binary_function <Node, Node, bool>                                                                                
{
	bool operator()(const Node& n1, const Node& n2) const
	{
		return n1.getManhattanDistanceValue() + n1.getDepth() > n2.getManhattanDistanceValue() + n2.getDepth();
	}
};

// Runs the search algorithm the user specifies by using the corresponding priority queue
bool generalSearch(int arr[3][3], int queueFunc)
{
    set<string> repeats;
    Node initialState = Node(arr);
    initialState.calculateMissingTileValue();
    initialState.calculateManhattanDistanceValue();
    int numNodesExpanded = 0;
    int maxQueueSize = 1;
    bool isRepeat;

    // Uniform cost algorithm
    if (queueFunc == 1) 
    {
        priority_queue<Node, vector<Node>, UniformCostHeuristic> nodes; 
        nodes.push(initialState);
        repeats.insert(initialState.getPuzzle());

        while (!nodes.empty())
        {
            set<string>::iterator it=repeats.begin();
            isRepeat = false;
            if (maxQueueSize < nodes.size())
            {
                maxQueueSize = nodes.size();
            }

            // The node placed at the top by the priority queue, is the best node to expand
            Node topNode = nodes.top();
            nodes.pop();
            cout << "The best state to expand is:" << endl;
            topNode.printPuzzle();
            if (topNode.isGoalState())
            {
                cout << "To solve this problem the search algorithm expanded a total of " << numNodesExpanded << " nodes.\n";
                cout << "The maximum number of nodes in the queue at any one time was " << maxQueueSize << endl;
                cout << "The depth of the goal node was " << topNode.getDepth() << endl;
                return true;
            }
        
            std::queue<Node> expandedNodes;
            cout << "Expanding this node...\n";
            expandedNodes = expandNode(topNode);
            numNodesExpanded++;
        
            while(!expandedNodes.empty())
            {
                Node temp = expandedNodes.front();
                for (auto key : repeats)
                {
                    if (temp.getPuzzle() == key)
                    {
                        isRepeat = true;
                    }
                }
                if (isRepeat == false)
                {
                    nodes.push(temp);
                    repeats.insert(temp.getPuzzle());
                }
                expandedNodes.pop();
                isRepeat = false;
            }
        }

        return false;
    }

    // Missing tile algorithm
    if (queueFunc == 2) 
    {
        priority_queue<Node, vector<Node>, MissingTileHeuristic> nodes; 
        nodes.push(initialState);
        repeats.insert(initialState.getPuzzle());

        while (!nodes.empty())
        {
            set<string>::iterator it=repeats.begin();
            isRepeat = false;
            if (maxQueueSize < nodes.size())
            {
                maxQueueSize = nodes.size();
            }

            // The node placed at the top by the priority queue, is the best node to expand
            Node topNode = nodes.top();
            nodes.pop();
            cout << "The best state to expand is:" << endl;
            topNode.printPuzzle();
            if (topNode.isGoalState())
            {
                cout << "To solve this problem the search algorithm expanded a total of " << numNodesExpanded << " nodes.\n";
                cout << "The maximum number of nodes in the queue at any one time was " << maxQueueSize << endl;
                cout << "The depth of the goal node was " << topNode.getDepth() << endl;
                return true;
            }
        
            std::queue<Node> expandedNodes;
            cout << "Expanding this node...\n";
            expandedNodes = expandNode(topNode);
            numNodesExpanded++;
        
            while(!expandedNodes.empty())
            {
                Node temp = expandedNodes.front();
                for (auto key : repeats)
                {
                    if (temp.getPuzzle() == key)
                    {
                        isRepeat = true;
                    }
                }
                if (isRepeat == false)
                {
                    nodes.push(temp);
                    repeats.insert(temp.getPuzzle());
                }
                expandedNodes.pop();
                isRepeat = false;
            }
        }

        return false;
    }

    // Manhattan distance algorithm
    if (queueFunc == 3) 
    {
        priority_queue<Node, vector<Node>, ManhattanDistanceHeuristic> nodes; 
        nodes.push(initialState);
        repeats.insert(initialState.getPuzzle());

        while (!nodes.empty())
        {
            set<string>::iterator it=repeats.begin();
            isRepeat = false;
            if (maxQueueSize < nodes.size())
            {
                maxQueueSize = nodes.size();
            }

            // The node placed at the top by the priority queue, is the best node to expand
            Node topNode = nodes.top();
            nodes.pop();
            cout << "The best state to expand is:" << endl;
            topNode.printPuzzle();
            if (topNode.isGoalState())
            {
                cout << "To solve this problem the search algorithm expanded a total of " << numNodesExpanded << " nodes.\n";
                cout << "The maximum number of nodes in the queue at any one time was " << maxQueueSize << endl;
                cout << "The depth of the goal node was " << topNode.getDepth() << endl;
                return true;
            }
        
            std::queue<Node> expandedNodes;
            cout << "Expanding this node...\n";
            expandedNodes = expandNode(topNode);
            numNodesExpanded++;
        
            while(!expandedNodes.empty())
            {
                Node temp = expandedNodes.front();
                for (auto key : repeats)
                {
                    if (temp.getPuzzle() == key)
                    {
                        isRepeat = true;
                    }
                }
                if (isRepeat == false)
                {
                    nodes.push(temp);
                    repeats.insert(temp.getPuzzle());
                }
                expandedNodes.pop();
                isRepeat = false;
            }
        }
        return false;
    }
}

int main()
{
    // Opening Message: Asks user to choose between default puzzle or their own input
    cout << "Welcome to Osman Arshad's 8-puzzle solver.\n";
    cout << "Type 1 to use a default puzzle, or 2 to enter your own puzzle.\n";
    int defaultPuzzle;
    cin >> defaultPuzzle;

    // Puzzle represented in a 2d array
    int puzzle[3][3];

    // Default Puzzle generated here 
    if (defaultPuzzle == 1)
    {
        int puzzle[3][3] =
        {
            {1,2,3},
            {4,0,6},
            {7,5,8}
        };
    }

    // User Input Puzzle generated here
    if (defaultPuzzle == 2)
    {
        cout << "Enter your puzzle, use a zero to represent the blank\n";
        
        int RowNum = 0;
        int ColNum = 0;
        int TabVal = 0;
        int r = 0;

        while(r <= PUZZLE_SIZE)
        {
            if (r == 0) 
            {
                cout << "Enter the first row, use a space or tabs between numbers\n";
            }
            if (r == 3) 
            {
                cout << "Enter the second row, use a space or tabs between numbers\n";
                RowNum = 1;
                ColNum = 0;
            }
            if (r == 6) 
            {
                cout << "Enter the third row, use a space or tabs between numbers\n";
                RowNum = 2;
                ColNum = 0;
            }

            cin >> TabVal;
            puzzle[RowNum][ColNum] = TabVal;
            ColNum++;
            r++;
        }
    }

    // Ask user to choose a search algorithm
    cout << "\nEnter your choice of algorithm\n";
    cout << "1. Unform Cost Search\n";
    cout << "2. A* with the Misplaced Tile Heuristic\n";
    cout << "3. A* with the Manhattan distance heuristic\n";
    int algoChoice = 0;
    cin >> algoChoice;

    if (generalSearch(puzzle, algoChoice))
    {
       cout << "Sovled!\n";
    }
}