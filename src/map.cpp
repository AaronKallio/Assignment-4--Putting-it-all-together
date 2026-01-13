#include "map.h"
#include <stack>
#include <set>
#include <chrono>
#include <queue>
//#include <cfloat>
#include <fstream>
#include <iostream>
#include <algorithm>
using namespace std;
int pathMode = 0;
int mode = 1;
double timed;
std::vector<Map> maps;
Map theMap;
/*
bool Map::IsValid(int row, int col)
{
    return (row >= 0) && (row < width) && (col >= 0)
        && (col < height);
}


bool Map::IsUnBlocked(int row, int col)
{
    if (grid[row][col].type != 'X')
        return (true);
    else
        return (false);
}

bool Map::IsDestination(int row, int col, Pair dest)
{
    if (row == dest.first && col == dest.second)
        return (true);
    else
        return (false);
}


double Map::CalculateHValue(int row, int col, Pair dest)  
{
    int dx = abs(row - dest.first);
    int dy = abs(col - dest.second);
    return  10* max(dx, dy);  //Chebyshev distance


    //return 2 * (abs(row - dest.first) + abs(col - dest.second)); 
    // Return using the distance formula
    //return ((double)sqrt(
     //   (row - dest.first) * (row - dest.first)
     //   + (col - dest.second) * (col - dest.second)));
}

void Map::TracePath(Pair dest) //used in a star/dijkstra to draw the shortest path
{
    int row = dest.first;
    int col = dest.second;

    while (!(grid[row][col].parentI == row
        && grid[row][col].parentJ == col)) {
        path.push(make_pair(row, col));
        int temp_row = grid[row][col].parentI;
        int temp_col = grid[row][col].parentJ;
        row = temp_row;
        col = temp_col;
    }

    path.push(make_pair(row, col));
    auto tempPath = path;
    while (!tempPath.empty()) {
        pair<int, int> p = tempPath.top();
        tempPath.pop();
        grid[p.first][p.second].nodeStatus = visited;
    }
    return;
}

void Map::AStarSearch(Pair src, Pair dest)
{
    while (!path.empty()) //always empty the path just in case
        path.pop();
    
    // If the source/start is out of range
    if (IsValid(src.first, src.second) == false) {
        return;
    }

    // If the destination/end is out of range
    if (IsValid(dest.first, dest.second) == false) {
        return;
    }

    // Either the source or the destination is blocked
    if (IsUnBlocked(src.first, src.second) == false || IsUnBlocked(dest.first, dest.second)== false) {
        return;
    }

    // If the destination cell is the same as source cell
    if (IsDestination(src.first, src.second, dest) == true) {
        return;
    }
    
    //empty closed list to check if node needs to be checked or not
    std::vector<std::vector<bool>> closedList;
    closedList.resize(height, std::vector<bool>(width, false));
    int i, j;

    //undiscovered nodes set to highest possible cost so they are not traversed to prematurely
    for (i = 0; i < height; i++) {
        for (j = 0; j < width; j++) {
            grid[i][j].f = FLT_MAX;
            grid[i][j].g = FLT_MAX;
            grid[i][j].h = FLT_MAX;
            grid[i][j].parentI = -1;
            grid[i][j].parentJ = -1;
        }
    }

    // Initialising the parameters of the starting node
    i = src.first, j = src.second;
    grid[i][j].f = 0.0;
    grid[i][j].g = 0.0;
    grid[i][j].h = 0.0;
    grid[i][j].parentI = i;
    grid[i][j].parentJ = j;
    set<pPair> openList;

    // Put the starting cell on the open list and set its
    // 'f' as 0
    openList.insert(make_pair(0.0, make_pair(i, j)));
    bool foundDest = false;
    while (!openList.empty()) {
        pPair p = *openList.begin();

        // Remove this node from the open list
        openList.erase(openList.begin());

        // Add this node to the closed list
        i = p.second.first;
        j = p.second.second;
        closedList[i][j] = true;
        double gNew, hNew, fNew;

        //north
        if (IsValid(i - 1, j) == true) {

            // If the end found
            if (IsDestination(i - 1, j, dest) == true) {
                grid[i - 1][j].parentI = i;
                grid[i - 1][j].parentJ = j;
                TracePath(dest);
                foundDest = true;
                return;
            }
           
            //if not in closed list and can be accessed
            else if (closedList[i - 1][j] == false && IsUnBlocked(i - 1, j) == true) {
                gNew = grid[i][j].g + 1.0;
                hNew = CalculateHValue(i - 1, j, dest);
                fNew = gNew + hNew;

                //if not in open list or found a better path
                if (grid[i - 1][j].f == FLT_MAX || grid[i - 1][j].f > fNew) {
                    openList.insert(make_pair(fNew, make_pair(i - 1, j)));
                    grid[i - 1][j].nodeStatus = visited;
                    grid[i - 1][j].f = fNew;
                    grid[i - 1][j].g = gNew;
                    grid[i - 1][j].h = hNew;
                    grid[i - 1][j].parentI = i;  //used to reconstruct path
                    grid[i - 1][j].parentJ = j;
                }
            }
        }

        //south
        if (IsValid(i + 1, j) == true) {
           
            // If the end found
            if (IsDestination(i + 1, j, dest) == true) {
                grid[i + 1][j].parentI = i;
                grid[i + 1][j].parentJ = j;
                TracePath(dest);
                foundDest = true;
                return;
            }
          
            //if not in closed list and can be accessed
            else if (closedList[i + 1][j] == false&& IsUnBlocked(i + 1, j) == true) {
                gNew = grid[i][j].g + 1.0;
                hNew = CalculateHValue(i + 1, j, dest);
                fNew = gNew + hNew;
                //if not in open list or found a better path
                if (grid[i + 1][j].f == FLT_MAX || grid[i + 1][j].f > fNew) {
                    openList.insert(make_pair(fNew, make_pair(i + 1, j)));
                    grid[i + 1][j].nodeStatus = visited;
                    grid[i + 1][j].f = fNew;
                    grid[i + 1][j].g = gNew;
                    grid[i + 1][j].h = hNew;
                    grid[i + 1][j].parentI = i;//used to reconstruct path
                    grid[i + 1][j].parentJ = j;
                }
            }
        }

        //east
        if (IsValid(i, j + 1) == true) {
            // If the end found
            if (IsDestination(i, j + 1, dest) == true) {
                // Set the Parent of the destination cell
                grid[i][j + 1].parentI = i;
                grid[i][j + 1].parentJ = j;
                TracePath(dest);
                foundDest = true;
                return;
            }

            //if not in closed list and can be accessed
            else if (closedList[i][j + 1] == false && IsUnBlocked(i, j + 1)== true) {
                gNew = grid[i][j].g + 1.0;
                hNew = CalculateHValue(i, j + 1, dest);
                fNew = gNew + hNew;
                //if not in open list or found a better path
                if (grid[i][j + 1].f == FLT_MAX || grid[i][j + 1].f > fNew) {
                    openList.insert(make_pair(fNew, make_pair(i, j + 1)));
                    grid[i][j + 1].nodeStatus = visited;
                    grid[i][j + 1].f = fNew;
                    grid[i][j + 1].g = gNew;
                    grid[i][j + 1].h = hNew;
                    grid[i][j + 1].parentI = i;//used to reconstruct path
                    grid[i][j + 1].parentJ = j;
                }
            }
        }

        //west
        if (IsValid(i, j - 1) == true) {
            // If the end found
            if (IsDestination(i, j - 1, dest) == true) {
                grid[i][j - 1].parentI = i;
                grid[i][j - 1].parentJ = j;
                TracePath(dest);
                foundDest = true;
                return;
            }
            //if not in closed list and can be accessed
            else if (closedList[i][j - 1] == false&& IsUnBlocked(i, j - 1) == true) {
                gNew = grid[i][j].g + 1.0;
                hNew = CalculateHValue(i, j - 1, dest);
                fNew = gNew + hNew;
                //if not in open list or found a better path
                if (grid[i][j - 1].f == FLT_MAX || grid[i][j - 1].f > fNew) {
                    openList.insert(make_pair(fNew, make_pair(i, j - 1)));
                    grid[i][j - 1].nodeStatus = visited;
                    grid[i][j - 1].f = fNew;
                    grid[i][j - 1].g = gNew;
                    grid[i][j - 1].h = hNew;
                    grid[i][j - 1].parentI = i;//used to reconstruct path
                    grid[i][j - 1].parentJ = j;
                }
            }
        }

        //south west
        if (IsValid(i - 1, j -1 ) == true&& grid[i - 1][j].type!='X'&& grid[i][j-1].type != 'X') {
            // If the end found
            if (IsDestination(i - 1, j - 1, dest) == true) {
                grid[i - 1][j - 1].parentI = i;
                grid[i - 1][j - 1].parentJ = j;
                TracePath(dest);
                foundDest = true;
                return;
            }
            //if not in closed list and can be accessed
            else if (closedList[i - 1][j - 1] == false&& IsUnBlocked(i - 1, j - 1) == true) {
                gNew = grid[i][j].g + 1;
                hNew = CalculateHValue(i - 1, j - 1, dest);
                fNew = gNew + hNew;
                //if not in open list or found a better path
                if (grid[i - 1][j - 1].f == FLT_MAX|| grid[i - 1][j - 1].f > fNew) {
                    openList.insert(make_pair(fNew, make_pair(i - 1, j - 1)));
                    grid[i - 1][j - 1].nodeStatus = visited;
                    grid[i - 1][j - 1].f = fNew;
                    grid[i - 1][j - 1].g = gNew;
                    grid[i - 1][j - 1].h = hNew;
                    grid[i - 1][j - 1].parentI = i;//used to reconstruct path
                    grid[i - 1][j - 1].parentJ = j;
                }
            }
        }
        //north east
        if (IsValid(i - 1, j +1 ) == true&& grid[i - 1][j].type!='X'&& grid[i][j+1].type != 'X') {
            // If the end found
            if (IsDestination(i - 1, j + 1, dest) == true) {
                // Set the Parent of the destination cell
                grid[i - 1][j + 1].parentI = i;
                grid[i - 1][j + 1].parentJ = j;
                TracePath(dest);
                foundDest = true;
                return;
            }
            //if not in closed list and can be accessed
            else if (closedList[i - 1][j + 1] == false&& IsUnBlocked(i - 1, j + 1) == true) {
                gNew = grid[i][j].g + 1;
                hNew = CalculateHValue(i - 1, j+1, dest);
                fNew = gNew + hNew;
                //if not in open list or found a better path
                if (grid[i - 1][j + 1].f == FLT_MAX|| grid[i - 1][j + 1].f > fNew) {
                    openList.insert(make_pair(fNew, make_pair(i - 1, j + 1)));
                    grid[i - 1][j + 1].nodeStatus = visited;
                    grid[i - 1][j + 1].f = fNew;
                    grid[i - 1][j + 1].g = gNew;
                    grid[i - 1][j + 1].h = hNew;
                    grid[i - 1][j + 1].parentI = i;//used to reconstruct path
                    grid[i - 1][j + 1].parentJ = j;
                }
            }
        }
        //south east
        if (IsValid(i + 1, j + 1) == true && grid[i + 1][j].type != 'X' && grid[i][j + 1].type != 'X') {
            // If the end found
            if (IsDestination(i + 1, j + 1, dest) == true) {
                grid[i + 1][j + 1].parentI = i;
                grid[i + 1][j + 1].parentJ = j;
                TracePath(dest);
                foundDest = true;
                return;
            }
            //if not in closed list and can be accessed
            else if (closedList[i + 1][j + 1] == false&& IsUnBlocked(i + 1, j + 1) == true) {
                gNew = grid[i][j].g + 1;
                hNew = CalculateHValue(i + 1, j+1, dest);
                fNew = gNew + hNew;
                //if not in open list or found a better path
                if (grid[i + 1][j + 1].f == FLT_MAX|| grid[i + 1][j + 1].f > fNew) {
                    openList.insert(make_pair(fNew, make_pair(i + 1, j + 1)));
                    grid[i + 1][j + 1].nodeStatus = visited;
                    // Update the details of this cell
                    grid[i + 1][j + 1].f = fNew;
                    grid[i + 1][j + 1].g = gNew;
                    grid[i + 1][j + 1].h = hNew;
                    grid[i + 1][j + 1].parentI = i;//used to reconstruct path
                    grid[i + 1][j + 1].parentJ = j;
                }
            }
        }


        //north west
        if (IsValid(i -1, j -1) == true && grid[i - 1][j].type != 'X' && grid[i][j - 1].type != 'X') {
            // If the end found
            if (IsDestination(i - 1, j - 1, dest) == true) {
                grid[i - 1][j - 1].parentI = i;
                grid[i - 1][j - 1].parentJ = j;
                TracePath(dest);
                foundDest = true;
                return;
            }
            //if not in closed list and can be accessed
            else if (closedList[i - 1][j - 1] == false&& IsUnBlocked(i - 1, j - 1) == true) {
                gNew = grid[i][j].g + 1;
                hNew = CalculateHValue(i - 1, j - 1, dest);
                fNew = gNew + hNew;
                //if not in open list or found a better path
                if (grid[i - 1][j - 1].f == FLT_MAX|| grid[i - 1][j - 1].f > fNew) {
                    openList.insert(make_pair(fNew, make_pair(i - 1, j - 1)));
                    grid[i - 1][j - 1].nodeStatus = visited;
                    grid[i - 1][j - 1].f = fNew;
                    grid[i - 1][j - 1].g = gNew;
                    grid[i - 1][j - 1].h = hNew;
                    grid[i - 1][j - 1].parentI = i;//used to reconstruct path
                    grid[i - 1][j - 1].parentJ = j;
                }
            }
        }
    }

    
    if (foundDest == false)
        //DrawDebugText({20,10}, "not found");

    return;
}

void RunAStar(int mapNum)
{
    Pair src = make_pair(1, 1);
    Pair dest = make_pair(maps[mapNum].height - 2, maps[mapNum].width - 2);
   maps[mapNum].AStarSearch(src, dest);

}

void RunBFS(int mapNum)
{
    Pair src = make_pair(1, 1);
    Pair dest = make_pair(maps[mapNum].height - 2, maps[mapNum].width - 2);
    maps[mapNum].BFS(src, dest);

}

void ResetMap(int mapNum) {  //resets all nodes when changing maps
    for (int y = 0; y < maps[mapNum].height; ++y) {
        for (int x = 0; x < maps[mapNum].width; ++x) {
            maps[mapNum].grid[y][x].nodeStatus = unvisited;
            while (!maps[mapNum].path.empty())
                maps[mapNum].path.pop();
        }
    }
}
*/

//int WhichMap() {
	//if (Play::KeyPressed(Play::KEY_1)) {
	//	mode = 1;
	//}
	//else if (Play::KeyPressed(Play::KEY_2)) {
	//	mode = 2;
	//}
	//else if (Play::KeyPressed(Play::KEY_3)) {
	//	mode = 3;
	//}
	//return mode;
//}

//int WhichPathFinding() {
    //int funcPathNode = 0;
    //if (Play::KeyPressed(Play::KEY_4)) {
    //   funcPathNode = 4;
    //}
    //else if (Play::KeyPressed(Play::KEY_5)) {
     //   funcPathNode = 5;
    //}
    //else if (Play::KeyPressed(Play::KEY_6)) {
    //    funcPathNode = 6;
    //}
    //else if (Play::KeyPressed(Play::KEY_7)) {
     //   funcPathNode = 7;
    //}
    //return funcPathNode;
//}

/*
void RunPathFinding(int mapNum) {  
    int temp = WhichPathFinding();
    std::string elapsedStr = std::to_string(timed) + "ms";
    const char* cstr = elapsedStr.c_str();
    //DrawDebugText({455,850},cstr, cRed);
    if (pathMode ==temp) {
        return;
    }
    for (int i = 0; i < 3;i++) {
        ResetMap(i);
    }
    if (temp == 4) {
        std::chrono::duration<double> timer;
        auto start = std::chrono::high_resolution_clock::now();
        RunAStar(mapNum);
        auto end = std::chrono::high_resolution_clock::now();
        timer = end - start;
        timed = timer.count();
    }
    if (temp == 5) {
        std::chrono::duration<double> timer;
        auto start = std::chrono::high_resolution_clock::now();
        RunBFS(mapNum);
        auto end = std::chrono::high_resolution_clock::now();
        timer = end - start;
        timed = timer.count();
    }
    if (temp == 6) {
        std::chrono::duration<double> timer;
        auto start = std::chrono::high_resolution_clock::now();
        RunDFS(mapNum);
        auto end = std::chrono::high_resolution_clock::now();
        timer = end - start;
        timed = timer.count();
    }
    if (temp == 7) {
        std::chrono::duration<double> timer;
        auto start = std::chrono::high_resolution_clock::now();
        RunDijkstra(mapNum);
        auto end = std::chrono::high_resolution_clock::now();
        timer = end - start;
        timed = timer.count();
    }
}

*/

namespace {
    const int dRow[8] = {-1, 1, 0, 0, -1, -1, 1, 1};
    const int dCol[8] = {0, 0, -1, 1, -1, 1, -1, 1};

    const float moveCost[8] = {
        10, 10, 10, 10,
        14, 14, 14, 14
    };
}

void FillMap(){
	theMap.MapFill("src/MapSimulation.txt");
    //theMap.PlaceTrees(5);
    cout << "poop" << endl;
	//Map map2; map2.MapFill("map2.txt"); maps.push_back(map2);
	//Map map3; map3.MapFill("map3.txt"); maps.push_back(map3);
}

void RunMap() {
	theMap.DrawMap();
}



Map::Map() {
	this->grid = {};
	this->width = 0;
	this->height = 0;
}

Map::~Map() {
}

void Map::MapFill(const std::string& fileName) {
    width = 0;
    height = 0;
    grid.clear();

    std::ifstream file(fileName);
    if (!file.is_open()) {
        //cout << "bruh\n";
        return;
    }

    std::string line;
    while (std::getline(file, line)) {
        if (width == 0) {
            width = line.size();
        }

        std::vector<Node> row;
        for (int x = 0; x < line.size(); ++x) {
            row.emplace_back(x, grid.size(), line[x], 0,0,0,0,0);
        }
        grid.push_back(row);
    }

    height = grid.size();
    ConnectNodes();
}

void Map::ConnectNodes() {
    for (int y = 0; y < grid.size(); ++y) {
        for (int x = 0; x < grid[y].size(); ++x) {

            Node& current = grid[y][x];
            current.position = { x * 10.0f, y * 10.0f };

            if (current.type == 'B') continue;
            if (current.type == 'T') current.PlaceTrees(5);
            if (current.type == 'V'||current.type == 'B') current.passable = false;

            // up
            if (y > 0 && x < grid[y - 1].size() && grid[y - 1][x].type != 'B') {
                current.connectedNodes.push_back(&grid[y - 1][x]);
            }

            if (y + 1 < grid.size() && x < grid[y + 1].size() && grid[y + 1][x].type != 'B') {
                current.connectedNodes.push_back(&grid[y + 1][x]);
            }

            // left
            if (x > 0 && grid[y][x - 1].type != 'B') {
                current.connectedNodes.push_back(&grid[y][x - 1]);
            }

            // right
            if (x + 1 < grid[y].size() && grid[y][x + 1].type != 'B') {
                current.connectedNodes.push_back(&grid[y][x + 1]);
            }
        }
    }
}

void Map::DrawMap() {
    //cout << "bob" << endl;
	for (int y = 0; y < grid.size(); ++y) {
    for (int x = 0; x < grid[y].size(); ++x) {
        Node& current = grid[y][x];
            //out << current.type << endl;
			Vector2 bottomLeft = current.position;
			Vector2 topRight = { current.position.x + 10,current.position.y + 10 };
			char tempType = current.type;
            if (current.discovered == false) {
				//DrawRect(bottomLeft, topRight, cWhite, true);
                DrawRectangle(current.position.x,current.position.y,10,10,BLACK);
			}
			else if (tempType == 'T') {
				//DrawRect(bottomLeft, topRight, cWhite, true);
                DrawRectangle(current.position.x,current.position.y,10,10,GREEN);
			}
			else if (tempType == 'V') {
                DrawRectangle(current.position.x,current.position.y,10,10,BLUE);
			}
			else if (tempType == 'G') {
				//DrawRect(bottomLeft, topRight, cYellow, true);
                DrawRectangle(current.position.x,current.position.y,10,10,BROWN);
			}
			else if (tempType == 'B') {
				//DrawRect(bottomLeft, topRight, cGreen, true);
                DrawRectangle(current.position.x,current.position.y,10,10,GRAY);

			}
            else if (tempType == 'M') {
				//DrawRect(bottomLeft, topRight, cGreen, true);
                DrawRectangle(current.position.x,current.position.y,10,10,GREEN);
			}
            for(int j = 0;j < current.objects.size();j++){
                if(current.objects[j].first == 'T' &&current.discovered == true){
                    DrawPixel(current.position.x + current.objects[j].second.first,current.position.y + current.objects[j].second.second,DARKGREEN);
                }
            }

		}
	}
    //if (path.empty()) return;

    //auto tempPath = path;

    //Pair endPoint = tempPath.top();
    //tempPath.pop();

    //while (!tempPath.empty()) {
        //Pair startPoint = tempPath.top();
        //tempPath.pop();

        // Convert grid coordinates to screen coordinates
        //Vector2 p1 = { startPoint.second * 10, startPoint.first * 10 };
        //Vector2 p2 = { endPoint.second * 10, endPoint.first * 10 };
        //DrawLine(p1, p2, cGreen);

        //endPoint = startPoint; // move to next segment
    }


void Node::PlaceTrees(int count) {
    // Place the first tree
    objects.push_back({'T', {GetRandomValue(0, 9), GetRandomValue(0, 9)}});

    for (int i = 1; i < count; i++) {
        bool foundSpot = false;

        while (!foundSpot) {
            int randomX = GetRandomValue(0, 9);
            int randomY = GetRandomValue(0, 9);
            bool safe = true;

            // Check if a tree is already at this position
            for (auto& obj : objects) {
                if (obj.second.first == randomX && obj.second.second == randomY) {
                    safe = false;
                    break;
                }
            }

            if (safe) {
                objects.push_back({'T', {randomX, randomY}});
                foundSpot = true;
            }
        }
    }
}



bool Map::IsValid(int row, int col) const
{
    return row >= 0 && row < height &&
           col >= 0 && col < width;
}

bool Map::IsWalkable(int row, int col) const
{
    if(grid[row][col].type == 'V' || grid[row][col].type == 'B'){
        return false;
    }
    return true;
    //return grid[row][col].type != 'V' || grid[row][col].type != 'B';
    //return grid[row][col].type != 'X';
}

float Map::Heuristic(int row, int col, Pair dest) const
{
    int dx = abs(row - dest.first);
    int dy = abs(col - dest.second);
    return 10.0f * std::max(dx, dy); // Chebyshev (8-dir)
}


bool Map::AStarSearch(Pair start, Pair goal, std::vector<Pair>& outPath)
{
    
    outPath.clear();

    if (!IsValid(start.first, start.second) ||
        !IsValid(goal.first, goal.second))
        return false;
    if (!IsWalkable(start.first, start.second) ||
        !IsWalkable(goal.first, goal.second))
        return false;

    std::vector<std::vector<bool>> closed(height,
        std::vector<bool>(width, false));

    std::vector<std::vector<AStarNode>> nodes(height,
        std::vector<AStarNode>(width));

    auto& startNode = nodes[start.first][start.second];
    startNode.g = 0.0f;
    startNode.h = Heuristic(start.first, start.second, goal);
    startNode.f = startNode.h;
    startNode.parentRow = start.first;
    startNode.parentCol = start.second;

    std::set<std::pair<float, Pair>> open;
    open.insert({startNode.f, start});

    while (!open.empty()) {
        auto current = *open.begin();
        open.erase(open.begin());

        int r = current.second.first;
        int c = current.second.second;
        closed[r][c] = true;

        if (r == goal.first && c == goal.second) {
            // reconstruct path
            int cr = r, cc = c;
            while (!(nodes[cr][cc].parentRow == cr &&
                     nodes[cr][cc].parentCol == cc)) {
                outPath.push_back({cr, cc});
                int pr = nodes[cr][cc].parentRow;
                int pc = nodes[cr][cc].parentCol;
                cr = pr;
                cc = pc;
            }
            outPath.push_back(start);
            std::reverse(outPath.begin(), outPath.end());
            return true;
        }

        for (int i = 0; i < 8; i++) {
            int nr = r + dRow[i];
            int nc = c + dCol[i];

            if (!IsValid(nr, nc) || closed[nr][nc])
                continue;

            // prevent corner cutting
            if (i >= 4) {
                if (!IsWalkable(r, nc) || !IsWalkable(nr, c))
                    continue;
            }

            if (!IsWalkable(nr, nc))
                continue;

            float gNew = nodes[r][c].g + moveCost[i];
            auto& neighbor = nodes[nr][nc];

            if (gNew < neighbor.g) {
                neighbor.g = gNew;
                neighbor.h = Heuristic(nr, nc, goal);
                neighbor.f = neighbor.g + neighbor.h;
                neighbor.parentRow = r;
                neighbor.parentCol = c;

                open.insert({neighbor.f, {nr, nc}});
            }
        }
    }
    return false; // no path
    
   
}

const std::vector<Vector2>& Map::GetLastPath() const
{
    return lastPath;
}


Map GetMap(){
    return theMap;
}







/*
void Node::PlaceTrees(int count) {
    //std::mt19937 rng(12345);
    objects.push_back(("T",(GetRandomValue(0,10),GetRandomValue(0,10))));
    for(int i = 0;i < count-1;i++){
        bool foundSpot = false;
        while(!foundSpot){
            bool safe = true;
            int randomX = GetRandomValue(0,10);
            int randomY = GetRandomValue(0,10);
            for(int j = 0;j < objects.size();j++){
                if(objects[j].second.first==randomX||objects[j].second.first==randomY){
                    safe = false;
                    break;
                }
            }
            if(safe){
                foundSpot = true;
                objects.push_back(("T",(randomX,randomY)));
            }
        }
    }
}
*/
/*
void Map::BFS(Pair start, Pair end) {
    while (!path.empty())
        path.pop();
    queue<Pair> checkNodes;
    checkNodes.push(start);

    grid[start.first][start.second].nodeStatus = visited;

    // 2D array with pairs   {-1,-1} means no parent
    vector<vector<Pair>> parent(height, vector<Pair>(width, { -1, -1 }));

    parent[start.first][start.second] = start;

    // directions: Up, Down, Left, Right for checking if the node being checked is in bounds
    int dRow[] = { -1, 1, 0, 0, 1,1,-1,-1};
    int dCol[] = { 0, 0, -1, 1, 1,-1,1,-1};

    while (!checkNodes.empty()) { //checks the next element in order - ones that were added first will be checked first which will cause the BFS wave like pattern
        Pair current = checkNodes.front();
        checkNodes.pop();

        int row = current.first;
        int col = current.second;

        // path is reconstructed with parent where the end is back tracked as each element in the parent array is storing its previous position
        if (current == end) {
            Pair p = end;
            while (!(p.first == start.first && p.second == start.second)) {
                path.push(p);
                p = parent[p.first][p.second];
            }
            path.push(start);
            return;
        }

        // Explore neighbors
        for (int i = 0; i < 8; i++) {
            int newRow = row + dRow[i];
            int newCol = col + dCol[i];

            // Check bounds
            if (newRow < 0 || newRow >= height || newCol < 0 || newCol >= width)
                continue;

            Node& neighbor = grid[newRow][newCol];

            if (neighbor.nodeStatus != visited && neighbor.type != 'X') {
                if (abs(dCol[i]) + abs(dRow[i]) == 2) { //stops skipping corners
                    Node& check1 = grid[newRow][col];
                    Node& check2 = grid[row][newCol];
                    if (check1.type== 'X'|| check2.type == 'X') {
                        continue;
                    }
                }
                neighbor.nodeStatus = visited;   // Mark visited immediately
                checkNodes.push({ newRow, newCol });
                parent[newRow][newCol] = { row, col }; // tracks where the path came from
            }
        }
    }

    //DrawDebugText({ 100,100 }, "No path found", cRed);
}

void Map::DFS(Pair start, Pair end) {
    while (!path.empty())
        path.pop();
    stack<Pair> checkNodes; //stack is used try to follow top of stack until the search cant go anywhere, then pop and look at next item
    checkNodes.push(start);  //starts of with start and every visited element will be added so program can back track
    grid[start.first][start.second].nodeStatus = visited;
    // 2D array with pairs   {-1,-1} means no parent
    vector<vector<Pair>> parent(height, vector<Pair>(width, { -1, -1 }));

    parent[start.first][start.second] = start;
    Pair direction = make_pair(0, 0); //store direction so that the algoritm continues into a direction instead of bouncing everywhere
    int dRow[] = { 1,1,1,0 , 0, -1, -1, -1 };
    int dCol[] = { 1,0,-1,1 , -1, 1, 0, -1 };
    getDirection(start, end, dRow,dCol); //the algorithm will go in a certain direction until it cannot to lower comparisions and make it more accurate
    while (!checkNodes.empty()) {
        Pair current = checkNodes.top();
        int row = current.first;
        int col = current.second;
        grid[row][col].nodeStatus = visited;
        if (current == end) {  //create path if end found
            Pair p = end;
            while (!(p.first == start.first && p.second == start.second)) {
                path.push(p);
                p = parent[p.first][p.second];
            }
            path.push(start);
            return;
        }
        Node& currentNode = grid[row][col];
        int rowDir = row + direction.first;
        int colDir = col + direction.second;

       
        if (grid[rowDir][colDir].nodeStatus != visited && grid[rowDir][colDir].type != 'X') { //direction priority
            bool valid = true;
            if (abs(direction.first) + abs(direction.second) == 2) { //stops skipping corners
                Node& check1 = grid[rowDir][col];
                Node& check2 = grid[row][colDir];
                if (check1.type == 'X' || check2.type == 'X') {
                    valid = false;
                }
            }
            if (valid) {
                checkNodes.push(make_pair(rowDir, colDir));
                parent[rowDir][colDir] = { row, col };   
                continue;
            }
        }
        
       bool stop = false;
       for (int i = 0; i < 8; i++) {
           if (stop) {
                break;
            }
           int newRow = row + dRow[i];
           int newCol = col + dCol[i];

           // Check bounds
           if (newRow < 0 || newRow >= height || newCol < 0 || newCol >= width)
               continue;

           Node& neighbor = grid[newRow][newCol];


           if (neighbor.nodeStatus != visited && neighbor.type != 'X') {
               if (abs(dCol[i]) + abs(dRow[i]) == 2) {
                   Node& check1 = grid[newRow][col];
                   Node& check2 = grid[row][newCol];
                   if (check1.type == 'X' || check2.type == 'X') {
                       continue;
                   }
               }
               checkNodes.push(make_pair(newRow, newCol));
               parent[newRow][newCol] = { row, col };
               direction = make_pair(dRow[i], dCol[i]);
               stop = true;
               continue;
           }
       }
        checkNodes.pop(); //if cant go anywhere then pop to backtrack

    }
}


void RunDFS(int mapNum) {
    Pair src = make_pair(1, 1);
    Pair dest = make_pair(maps[mapNum].height - 2, maps[mapNum].width - 2);
    maps[mapNum].DFS(src, dest);
}

void RunDijkstra(int mapNum) {
    Pair src = make_pair(1, 1);
    Pair dest = make_pair(maps[mapNum].height - 2, maps[mapNum].width - 2);
    maps[mapNum].Dijkstra(src, dest);
}


void Map::Dijkstra(Pair src, Pair dest) { //very similar to a* but there is no heursitic h calculation
    while (!path.empty()) //always empty the path just in case
        path.pop();

    // If the source/start is out of range
    if (IsValid(src.first, src.second) == false) {
        return;
    }

    // If the destination/end is out of range
    if (IsValid(dest.first, dest.second) == false) {
        return;
    }

    // Either the source or the destination is blocked
    if (IsUnBlocked(src.first, src.second) == false || IsUnBlocked(dest.first, dest.second) == false) {
        return;
    }

    // If the destination cell is the same as source cell
    if (IsDestination(src.first, src.second, dest) == true) {
        return;
    }

    //empty closed list to check if node needs to be checked or not
    std::vector<std::vector<bool>> closedList;
    closedList.resize(height, std::vector<bool>(width, false));
    int i, j;

    //undiscovered nodes set to highest possible cost so they are not traversed to prematurely
    for (i = 0; i < height; i++) {
        for (j = 0; j < width; j++) {
            grid[i][j].f = FLT_MAX;
            grid[i][j].g = FLT_MAX;
            grid[i][j].parentI = -1;
            grid[i][j].parentJ = -1;
        }
    }

    // Initialising the parameters of the starting node
    i = src.first, j = src.second;
    grid[i][j].f = 0.0;
    grid[i][j].g = 0.0;
    grid[i][j].parentI = i;
    grid[i][j].parentJ = j;
    set<pPair> openList;

    // Put the starting cell on the open list and set its
    // 'f' as 0
    openList.insert(make_pair(0.0, make_pair(i, j)));
    bool foundDest = false;
    while (!openList.empty()) {
        pPair p = *openList.begin();

        // Remove this node from the open list
        openList.erase(openList.begin());

        // Add this node to the closed list
        i = p.second.first;
        j = p.second.second;
        closedList[i][j] = true;
        double gNew, fNew;

        //north
        if (IsValid(i - 1, j) == true) {

            // If the end found
            if (IsDestination(i - 1, j, dest) == true) {
                grid[i - 1][j].parentI = i;
                grid[i - 1][j].parentJ = j;
                TracePath(dest);
                foundDest = true;
                return;
            }

            //if not in closed list and can be accessed
            else if (closedList[i - 1][j] == false && IsUnBlocked(i - 1, j) == true) {
                gNew = grid[i][j].g + 1.0;
                fNew = gNew;

                //if not in open list or found a better path
                if (grid[i - 1][j].f == FLT_MAX || grid[i - 1][j].f > fNew) {
                    openList.insert(make_pair(fNew, make_pair(i - 1, j)));
                    grid[i - 1][j].nodeStatus = visited;
                    grid[i - 1][j].f = fNew;
                    grid[i - 1][j].g = gNew;
                    grid[i - 1][j].parentI = i;  //used to reconstruct path
                    grid[i - 1][j].parentJ = j;
                }
            }
        }

        //south
        if (IsValid(i + 1, j) == true) {

            // If the end found
            if (IsDestination(i + 1, j, dest) == true) {
                grid[i + 1][j].parentI = i;
                grid[i + 1][j].parentJ = j;
                TracePath(dest);
                foundDest = true;
                return;
            }

            //if not in closed list and can be accessed
            else if (closedList[i + 1][j] == false && IsUnBlocked(i + 1, j) == true) {
                gNew = grid[i][j].g + 1.0;
                fNew = gNew;
                //if not in open list or found a better path
                if (grid[i + 1][j].f == FLT_MAX || grid[i + 1][j].f > fNew) {
                    openList.insert(make_pair(fNew, make_pair(i + 1, j)));
                    grid[i + 1][j].nodeStatus = visited;
                    grid[i + 1][j].f = fNew;
                    grid[i + 1][j].g = gNew;
                    grid[i + 1][j].parentI = i;//used to reconstruct path
                    grid[i + 1][j].parentJ = j;
                }
            }
        }

        //east
        if (IsValid(i, j + 1) == true) {
            // If the end found
            if (IsDestination(i, j + 1, dest) == true) {
                // Set the Parent of the destination cell
                grid[i][j + 1].parentI = i;
                grid[i][j + 1].parentJ = j;
                TracePath(dest);
                foundDest = true;
                return;
            }

            //if not in closed list and can be accessed
            else if (closedList[i][j + 1] == false && IsUnBlocked(i, j + 1) == true) {
                gNew = grid[i][j].g + 1.0;
                fNew = gNew;
                //if not in open list or found a better path
                if (grid[i][j + 1].f == FLT_MAX || grid[i][j + 1].f > fNew) {
                    openList.insert(make_pair(fNew, make_pair(i, j + 1)));
                    grid[i][j + 1].nodeStatus = visited;
                    grid[i][j + 1].f = fNew;
                    grid[i][j + 1].g = gNew;
                    grid[i][j + 1].parentI = i;//used to reconstruct path
                    grid[i][j + 1].parentJ = j;
                }
            }
        }

        //west
        if (IsValid(i, j - 1) == true) {
            // If the end found
            if (IsDestination(i, j - 1, dest) == true) {
                grid[i][j - 1].parentI = i;
                grid[i][j - 1].parentJ = j;
                TracePath(dest);
                foundDest = true;
                return;
            }
            //if not in closed list and can be accessed
            else if (closedList[i][j - 1] == false && IsUnBlocked(i, j - 1) == true) {
                gNew = grid[i][j].g + 1.0;
                fNew = gNew;
                //if not in open list or found a better path
                if (grid[i][j - 1].f == FLT_MAX || grid[i][j - 1].f > fNew) {
                    openList.insert(make_pair(fNew, make_pair(i, j - 1)));
                    grid[i][j - 1].nodeStatus = visited;
                    grid[i][j - 1].f = fNew;
                    grid[i][j - 1].g = gNew;
                    grid[i][j - 1].parentI = i;//used to reconstruct path
                    grid[i][j - 1].parentJ = j;
                }
            }
        }

        //south west
        if (IsValid(i - 1, j - 1) == true && grid[i - 1][j].type != 'X' && grid[i][j - 1].type != 'X') {
            // If the end found
            if (IsDestination(i - 1, j - 1, dest) == true) {
                grid[i - 1][j - 1].parentI = i;
                grid[i - 1][j - 1].parentJ = j;
                TracePath(dest);
                foundDest = true;
                return;
            }
            //if not in closed list and can be accessed
            else if (closedList[i - 1][j - 1] == false && IsUnBlocked(i - 1, j - 1) == true) {
                gNew = grid[i][j].g + 1;
                fNew = gNew;
                //if not in open list or found a better path
                if (grid[i - 1][j - 1].f == FLT_MAX || grid[i - 1][j - 1].f > fNew) {
                    openList.insert(make_pair(fNew, make_pair(i - 1, j - 1)));
                    grid[i - 1][j - 1].nodeStatus = visited;
                    grid[i - 1][j - 1].f = fNew;
                    grid[i - 1][j - 1].g = gNew;
                    grid[i - 1][j - 1].parentI = i;//used to reconstruct path
                    grid[i - 1][j - 1].parentJ = j;
                }
            }
        }
        //north east
        if (IsValid(i - 1, j + 1) == true && grid[i - 1][j].type != 'X' && grid[i][j + 1].type != 'X') {
            // If the end found
            if (IsDestination(i - 1, j + 1, dest) == true) {
                // Set the Parent of the destination cell
                grid[i - 1][j + 1].parentI = i;
                grid[i - 1][j + 1].parentJ = j;
                TracePath(dest);
                foundDest = true;
                return;
            }
            //if not in closed list and can be accessed
            else if (closedList[i - 1][j + 1] == false && IsUnBlocked(i - 1, j + 1) == true) {
                gNew = grid[i][j].g + 1;
                fNew = gNew;
                //if not in open list or found a better path
                if (grid[i - 1][j + 1].f == FLT_MAX || grid[i - 1][j + 1].f > fNew) {
                    openList.insert(make_pair(fNew, make_pair(i - 1, j + 1)));
                    grid[i - 1][j + 1].nodeStatus = visited;
                    grid[i - 1][j + 1].f = fNew;
                    grid[i - 1][j + 1].g = gNew;
                    grid[i - 1][j + 1].parentI = i;//used to reconstruct path
                    grid[i - 1][j + 1].parentJ = j;
                }
            }
        }
        //south east
        if (IsValid(i + 1, j + 1) == true && grid[i + 1][j].type != 'X' && grid[i][j + 1].type != 'X') {
            // If the end found
            if (IsDestination(i + 1, j + 1, dest) == true) {
                grid[i + 1][j + 1].parentI = i;
                grid[i + 1][j + 1].parentJ = j;
                TracePath(dest);
                foundDest = true;
                return;
            }
            //if not in closed list and can be accessed
            else if (closedList[i + 1][j + 1] == false && IsUnBlocked(i + 1, j + 1) == true) {
                gNew = grid[i][j].g + 1;
                fNew = gNew;
                //if not in open list or found a better path
                if (grid[i + 1][j + 1].f == FLT_MAX || grid[i + 1][j + 1].f > fNew) {
                    openList.insert(make_pair(fNew, make_pair(i + 1, j + 1)));
                    grid[i + 1][j + 1].nodeStatus = visited;
                    // Update the details of this cell
                    grid[i + 1][j + 1].f = fNew;
                    grid[i + 1][j + 1].g = gNew;
                    grid[i + 1][j + 1].parentI = i;//used to reconstruct path
                    grid[i + 1][j + 1].parentJ = j;
                }
            }
        }


        //north west
        if (IsValid(i - 1, j - 1) == true && grid[i - 1][j].type != 'X' && grid[i][j - 1].type != 'X') {
            // If the end found
            if (IsDestination(i - 1, j - 1, dest) == true) {
                grid[i - 1][j - 1].parentI = i;
                grid[i - 1][j - 1].parentJ = j;
                TracePath(dest);
                foundDest = true;
                return;
            }
            //if not in closed list and can be accessed
            else if (closedList[i - 1][j - 1] == false && IsUnBlocked(i - 1, j - 1) == true) {
                gNew = grid[i][j].g + 1;
                fNew = gNew;
                //if not in open list or found a better path
                if (grid[i - 1][j - 1].f == FLT_MAX || grid[i - 1][j - 1].f > fNew) {
                    openList.insert(make_pair(fNew, make_pair(i - 1, j - 1)));
                    grid[i - 1][j - 1].nodeStatus = visited;
                    grid[i - 1][j - 1].f = fNew;
                    grid[i - 1][j - 1].g = gNew;
                    grid[i - 1][j - 1].parentI = i;//used to reconstruct path
                    grid[i - 1][j - 1].parentJ = j;
                }
            }
        }
    }


    if (foundDest == false)
        //DrawDebugText({ 20,10 }, "not found");

    return;
}

void getDirection(Pair start, Pair end, int dRow[8], int dCol[8]) {
    int dr = end.first - start.first;
    int dc = end.second - start.second;

    if (dr != 0) dr = (dr > 0 ? 1 : -1); //calculates direction
    

    // EAST
    if (dr == 0 && dc < 0) { //if (dr == 0 && dc > 0) {
        int r[] = { 1,1,1,0,0,-1,-1,-1 };
        int c[] = { 0,1,-1,1,-1,0,1,-1 };
        memcpy(dRow, r, sizeof(r));
        memcpy(dCol, c, sizeof(c));
        return;
    }

    // WEST
    if (dr == 0 && dc > 0) { //if (dr == 0 && dc < 0) {
        int r[] = { -1,-1,-1,0,0,1,1,1 };
        int c[] = { 0,1,-1,1,-1,0,1,-1 };
        memcpy(dRow, r, sizeof(r));
        memcpy(dCol, c, sizeof(c));
        return;
    }

    // NORTH
    if (dr > 0 && dc == 0) {// if (dr < 0 && dc == 0) {
        int r[] = { 0,1,-1,1,-1,0,-1,1 };
        int c[] = { 1,1,1,0,0,-1,-1,-1 };
        memcpy(dRow, r, sizeof(r));
        memcpy(dCol, c, sizeof(c));
        return;
    }

    // SOUTH
    if (dr < 0 && dc == 0) { //if (dr > 0 && dc == 0) {
        int r[] = { 0,1,-1,1,-1,0,-1,1 };
        int c[] = { -1,-1,-1,0,0,1,1,1 };
        memcpy(dRow, r, sizeof(r));
        memcpy(dCol, c, sizeof(c));
        return;
    }

    // SE
    if (dr < 0 && dc < 0) {  //if (dr < 0 && dc > 0) {
        //int r[] = { 1,1,0,0,-1,1,-1,-1 };
        //int c[] = { 1,0,1,1,1,-1,0,-1 };


        //int r[] = { -1,-1,-1,0 , 0, 1, 1, 1 };
        //int c[] = { -1,0,1,-1 , 1, -1, 0, 1 };
        int r[] = { -1,-1,0,-1 , 1, 0, 1, 1 };
        int c[] = { -1,0,-1,1 , -1, 1, 0, 1 };

        memcpy(dRow, r, sizeof(r));
        memcpy(dCol, c, sizeof(c));
        //DrawDebugText({ 100,5 }, "4", cMagenta);
        return;
    }

    // SW
    if (dr < 0 && dc > 0) {  //if (dr < 0 && dc < 0) {
        //int r[] = { -1,0,-1,-1,1,0,1,1 };
        //int c[] = { 1,1,0,-1,1,-1,0,-1 };


        int r[] = { 1,1,1,0 , 0, -1, -1, -1 };
        int c[] = { -1,0,1,-1 , 1, -1, 0, 1 };


        memcpy(dRow, r, sizeof(r));
        memcpy(dCol, c, sizeof(c));
        //DrawDebugText({ 100,5 }, "3", cMagenta);
        return;
    }

    // NE
    if (dr > 0 && dc < 0) { //if (dr > 0 && dc > 0) {
        //int r[] = { 1, 0, 1, -1,-1,0,1,-1 };
        //int c[] = { -1,-1,0,-1,0,1,1,1 };
        int r[] = { -1,-1,-1,0 , 0, 1, 1, 1 };
        int c[] = { 1,0,-1,1, -1, 1, 0, -1 };
        memcpy(dRow, r, sizeof(r));
        memcpy(dCol, c, sizeof(c));
        //DrawDebugText({100,5},"1",cMagenta);
        return;
    }

    // NW
    if (dr > 0 && dc > 0) {  //if (dr > 0 && dc < 0) {
        //int r[] = { -1, 0, -1, 1,1,0,-1,1 };
        //int c[] = { -1,-1,0,-1,0,1,1,1 };
        int r[] = { 1,1,1,0 , 0, -1, -1, -1 };
        int c[] = { 1,0,-1,1 , -1, 1, 0, -1 };
        memcpy(dRow, r, sizeof(r));
        memcpy(dCol, c, sizeof(c));
        //DrawDebugText({ 100,5 }, "2", cMagenta);
        return;
    }
}
    */