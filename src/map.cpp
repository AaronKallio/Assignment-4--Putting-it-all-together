#include "map.h"
#include <stack>
#include <set>
#include <chrono>
#include <queue>
//#include <cfloat>
#include <fstream>
#include <iostream>
#include <algorithm>
#include <unordered_map>
using namespace std;
int pathMode = 0;
int mode = 1;
double timed;
std::vector<Map> maps;
Map theMap;


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
    
    for (int y = 95; y < 98; ++y) {
        for (int x = 2; x < 5; ++x) {
            Node& current = grid[y][x];
            current.discovered = true;
        }
    }

    for (int y = 0; y < grid.size(); ++y) {
        for (int x = 0; x < grid[y].size(); ++x) {

            Node& current = grid[y][x];
            current.position = { x * 10.0f, y * 10.0f };

            switch (current.type) {
            case 'T': current.moveCost = 1.0f, current.PlaceTrees(5); break; 
            case 'M': current.moveCost = 1.0f; break;
            case 'G': current.moveCost = 2.0f; break;
            case 'B': current.passable = false; break;
            case 'V': current.passable = false; break;
            }
        }
    }
    int oreCount = 0;
    while(oreCount<=60){
        int y = GetRandomValue(1,98);
        int x = GetRandomValue(1,98);
        Node& current = grid[y][x];
        if(current.discovered == false&&current.passable==true){
            current.objects.push_back({'O', {GetRandomValue(0,9), GetRandomValue(0,9)}});
            cout<<oreCount;
            oreCount++;
            
        }
    }
}


void Map::DrawMap() {
    //cout << "bob" << endl;
	for (int y = 0; y < grid.size(); ++y) {
    for (int x = 0; x < grid[y].size(); ++x) {
        Node& current = grid[y][x];
            //out << current.type << endl;
		
		char tempType = current.type;
		if (tempType == 'T') {
            DrawRectangle(current.position.x,current.position.y,10,10,GREEN);
		}
		else if (tempType == 'V') {
            DrawRectangle(current.position.x,current.position.y,10,10,BLUE);
		}
		else if (tempType == 'G') {
            DrawRectangle(current.position.x,current.position.y,10,10,BROWN);
		}
		else if (tempType == 'B') {
            DrawRectangle(current.position.x,current.position.y,10,10,GRAY);

		}
        else if (tempType == 'M') {
				//DrawRect(bottomLeft, topRight, cGreen, true);
            DrawRectangle(current.position.x,current.position.y,10,10,GREEN);
		}
        //DrawRectangle(270,540,10,10,BLACK);
        for(int j = 0;j < current.objects.size();j++){
            if(current.objects[j].first == 'T'){// &&current.discovered == true){
                //DrawCircle(
                //    current.position.x + current.objects[j].second.first,//static_cast<int>(c.pos.x),
                //    current.position.y + current.objects[j].second.second,//static_cast<int>(c.pos.y),
                //    1,
                //    DARKGREEN);
                DrawPixel(current.position.x + current.objects[j].second.first,current.position.y + current.objects[j].second.second,DARKGREEN);
            }
            else if(current.objects[j].first == 'O'){// &&current.discovered == true){
                DrawPixel(current.position.x + current.objects[j].second.first,current.position.y + current.objects[j].second.second,RED);
                //DrawCircle(
                //    current.position.x + current.objects[j].second.first,//static_cast<int>(c.pos.x),
                //    current.position.y + current.objects[j].second.second,//static_cast<int>(c.pos.y),
                //    1,
                //    RED);
            }
        }
		}
	}
        
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

            float tileCost = grid[nr][nc].moveCost;
            float gNew = nodes[r][c].g + moveCost[i] * tileCost;
            //float gNew = nodes[r][c].g + moveCost[i];
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


Map& GetMap(){
    return theMap;
}


Node& Map::GetNodeAtWorld(Vector2 worldPos)
{
    int col = int(worldPos.x / 10);
    int row = int(worldPos.y / 10);
    return grid[row][col];
}

void Map::RemoveTree(int y, int x){
    DrawRectangle(10,10,1000,1000,BLACK);
    for(int i = 0; i < 5;i++){ 
        
        //if(grid[y][x].objects[i].first=='T'){ 
        //    grid[y][x].objects.erase(grid[y][x].objects.begin() + i); 
        //    cout<<"idddddddddddddddddddk man"<<'\n';
            //std::string elapsedStr1 = to_string(grid[y][x].position.y);
	        //const char* cstr1 = elapsedStr1.c_str();
	        //DrawText(cstr1,500,400,50,PURPLE);
            cout<<grid[y][x].position.y<<'\n';
            cout<<grid[y][x].position.x<<'\n';

        //    DrawRectangle(grid[y][x].position.x,grid[y][x].position.y,10,10,GRAY);
        //    return; 
        //} 
        cout<<grid[y][x].objects[i].first<<'\n';
        grid[y][x].objects.erase(grid[y][x].objects.begin() + i); 
        
        } 
    }