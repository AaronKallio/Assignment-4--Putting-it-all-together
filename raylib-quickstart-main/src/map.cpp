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
int wood = 0;
int ore = 0;
int coal = 0;
int ironBar = 0;
int sword = 0;
constexpr int screenWidth = 1000;
constexpr int screenHeight = 1000;

namespace {
    const int dRow[8] = {-1, 1, 0, 0, -1, -1, 1, 1};
    const int dCol[8] = {0, 0, -1, 1, -1, 1, -1, 1};
    //int dRow[] = { -1, 1, 0, 0 };
    //int dCol[] = { 0, 0, -1, 1 };

    const float moveCost[8] = {
        10, 10, 10, 10,
        14, 14, 14, 14
    };
    //const float moveCost[4] = {
    //    10, 10, 10, 10
    //};
}




void FillMap(){
    //ofstream MyFile("filename.txt");
    //MyFile.close();
	theMap.MapFill("MapSimulation.txt");
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
            //current.objects.push_back({'O', {GetRandomValue(0,9), GetRandomValue(0,9)}});
            current.objects.push_back({
                ObjectType::Ore,
                Vector2{(float)GetRandomValue(0, 9),(float)GetRandomValue(0, 9)},
                -1});
            //cout<<oreCount;
            iron.push_back({y,x});
            oreCount++;
            
        }
    }
}


void Map::DrawMap() {
	for (int y = 0; y < grid.size(); ++y) {
    for (int x = 0; x < grid[y].size(); ++x) {
        Node& current = grid[y][x];
		char tempType = current.type;
        if (current.discovered == false) {
            continue;
		}
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
            DrawRectangle(current.position.x,current.position.y,10,10,GREEN);
		}
        for(int j = 0;j < current.objects.size();j++){
            if(current.objects[j].type == ObjectType::Tree&&current.discovered == true){
                DrawCircle(
                    current.position.x + current.objects[j].localPos.x,
                    current.position.y + current.objects[j].localPos.y,
                    1,
                    DARKGREEN);
            }
            else if(current.objects[j].type == ObjectType::Ore &&current.discovered == true){

                DrawCircle(
                    current.position.x + current.objects[j].localPos.x,
                    current.position.y + current.objects[j].localPos.y,
                    1,
                    RED);

            }
            else if(current.objects[j].type == ObjectType::ChoppedTree &&current.discovered == true){

                DrawCircle(
                    current.position.x + current.objects[j].localPos.x,
                    current.position.y + current.objects[j].localPos.y,
                    1,
                    DARKBROWN);

            }
            else if (current.objects[j].type == ObjectType::Coal && current.discovered == true) {


                DrawCircle(
                    current.position.x + current.objects[j].localPos.x,
                    current.position.y + current.objects[j].localPos.y,
                    1,
                    BLACK);
            }
            else if (current.objects[j].type == ObjectType::IronBar && current.discovered == true) {


                DrawCircle(
                    current.position.x + current.objects[j].localPos.x,
                    current.position.y + current.objects[j].localPos.y,
                    1,
                    DARKGRAY);
            }
            else if (current.objects[j].type == ObjectType::Sword && current.discovered == true) {


                DrawCircle(
                    current.position.x + current.objects[j].localPos.x,
                    current.position.y + current.objects[j].localPos.y,
                    1,
                    SKYBLUE);
            }

        }
		}
	}
        
    }


//RenderTexture2D mapTexture = LoadRenderTexture(screenWidth, screenHeight);
RenderTexture2D mapTexture;
void LoadTextureRender() {
    mapTexture = LoadRenderTexture(screenWidth, screenHeight);
}

void BakeMapTexture() {
    BeginTextureMode(mapTexture);
    ClearBackground(BLANK);
    theMap.DrawMap();
    EndTextureMode();
}

void DrawMapOptimized() {
    DrawTextureRec(mapTexture.texture,  { 0, 0, (float)mapTexture.texture.width, (float)-mapTexture.texture.height },  { 0, 0 }, WHITE);
}


void UpdateMapTexture() {
    if(theMap.updateMap) {
        BakeMapTexture();
        theMap.updateMap = false;
    }
}


void Node::PlaceTrees(int count) {    
    // Place the first tree
    objects.push_back({
                ObjectType::Tree,
                Vector2{(float)GetRandomValue(0, 9),(float)GetRandomValue(0, 9)},
                -1});

    for (int i = 1; i < count; i++) {
        bool foundSpot = false;

        while (!foundSpot) {
            int randomX = GetRandomValue(0, 9);
            int randomY = GetRandomValue(0, 9);
            bool safe = true;

            // Check if a tree is already at this position
            for (auto& obj : objects) {
                if (obj.localPos.x == randomX && obj.localPos.y == randomY) {
                    safe = false;
                    break;
                }
            }

            if (safe) {
                objects.push_back({
                ObjectType::Tree,
                Vector2{(float)randomX,(float)randomY},
                -1});
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

    if(grid[row][col].passable == false){
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


bool Map::AStarSearch(Pair start, Pair goal, std::vector<Pair>& outPath, bool scout)
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
            if(!scout&&!grid[nr][nc].discovered){
                continue;
            }

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

const std::vector<Vector2>& Map::GetLastPath() const{
    return lastPath;
}


Map& GetMap(){
    return theMap;
}


Node& Map::GetNodeAtWorld(Vector2 worldPos){
    int col = int(worldPos.x / 10);
    int row = int(worldPos.y / 10);
    return grid[row][col];
}

void Map::RemoveItem(int y, int x,int characterNum,ObjectType object){
    //DrawRectangle(10,10,1000,1000,BLACK);
    for(int i = 0; i < grid[y][x].objects.size();i++){ 
        
        if(grid[y][x].objects[i].type==object&&grid[y][x].objects[i].locked==characterNum){ 
            grid[y][x].objects.erase(grid[y][x].objects.begin() + i); 
            //cout << characterNum << '\n';
            return; 
        } 
       } 
    updateMap = true;
    }

Pair Map::BFS(Pair start, ObjectType theObject, std::vector<Pair>& outPath, int characterNum){
    outPath.clear();

    if (!IsValid(start.first, start.second))
        return { -1, -1 };

    // Create BFS node grid (same idea as A*)
    std::vector<std::vector<AStarNode>> nodes(
        height,
        std::vector<AStarNode>(width)
    );

    std::queue<Pair> open;

    // Initialize start node
    nodes[start.first][start.second].g = 0.0f;
    nodes[start.first][start.second].parentRow = start.first;
    nodes[start.first][start.second].parentCol = start.second;

    open.push(start);

    int dRow[] = { -1, 1, 0, 0, 1, 1, -1, -1 };
    int dCol[] = { 0, 0, -1, 1, 1, -1, 1, -1 };

    Pair found = { -1, -1 };

    while (!open.empty()) {
        Pair current = open.front();
        open.pop();

        int r = current.first;
        int c = current.second;


        if (grid[r][c].discovered&&NodeHasObject(r, c, theObject, characterNum)) {
            found = current;
            break;
        }

        for (int i = 0; i < 8; i++) {
            int nr = r + dRow[i];
            int nc = c + dCol[i];

            if (!IsValid(nr, nc))
                continue;

            if (!IsWalkable(nr, nc))
                continue;

            if (!grid[nr][nc].discovered)
                continue;

            if (i >= 4) {
                if (!IsWalkable(r, nc) || !IsWalkable(nr, c))
                    continue;
            }

            auto& neighbor = nodes[nr][nc];

            if (neighbor.g != FLT_MAX)
                continue;

            neighbor.g = nodes[r][c].g + 1.0f;
            neighbor.parentRow = r;
            neighbor.parentCol = c;

            open.push({ nr, nc });
        }
    }

    if (found.first == -1)
        return { -1, -1 };

    int cr = found.first;
    int cc = found.second;

    while (!(nodes[cr][cc].parentRow == cr &&
             nodes[cr][cc].parentCol == cc)) {

        outPath.push_back({ cr, cc });

        int pr = nodes[cr][cc].parentRow;
        int pc = nodes[cr][cc].parentCol;
        cr = pr;
        cc = pc;
    }

    outPath.push_back(start);
    std::reverse(outPath.begin(), outPath.end());

    return found;
}
/*
Pair Map::BFS(Pair start,
              ObjectType targetObject,
              std::vector<Pair>& outPath,
              int characterNum)
{
    outPath.clear();

    // Safety checks
    if (!IsValid(start.first, start.second))
        return { -1, -1 };

    if (!IsWalkable(start.first, start.second))
        return { -1, -1 };

    // Node grid (visited + parent)
    std::vector<std::vector<AStarNode>> nodes(
        height,
        std::vector<AStarNode>(width)
    );

    std::queue<Pair> open;

    // Initialize start
    nodes[start.first][start.second].g = 0.0f;
    nodes[start.first][start.second].parentRow = start.first;
    nodes[start.first][start.second].parentCol = start.second;
    open.push(start);

    // 4-direction movement ONLY
    const int dRow[4] = { -1, 1, 0, 0 };
    const int dCol[4] = { 0, 0, -1, 1 };

    Pair found = { -1, -1 };

    while (!open.empty()) {
        Pair current = open.front();
        open.pop();

        int r = current.first;
        int c = current.second;

        // SUCCESS CONDITION (no discovery check!)
        if (NodeHasObject(r, c, targetObject, characterNum)) {
            found = current;
            break;
        }

        for (int i = 0; i < 4; i++) {
            int nr = r + dRow[i];
            int nc = c + dCol[i];

            if (!IsValid(nr, nc))
                continue;

            if (!IsWalkable(nr, nc))
                continue;

            auto& neighbor = nodes[nr][nc];

            // Already visited
            if (neighbor.g != FLT_MAX)
                continue;

            neighbor.g = nodes[r][c].g + 1.0f;
            neighbor.parentRow = r;
            neighbor.parentCol = c;

            open.push({ nr, nc });
        }
    }

    // No object found
    if (found.first == -1)
        return { -1, -1 };

    // ---- PATH RECONSTRUCTION ----
    int cr = found.first;
    int cc = found.second;

    while (!(nodes[cr][cc].parentRow == cr &&
             nodes[cr][cc].parentCol == cc)) {

        outPath.push_back({ cr, cc });

        int pr = nodes[cr][cc].parentRow;
        int pc = nodes[cr][cc].parentCol;
        cr = pr;
        cc = pc;
    }

    outPath.push_back(start);
    std::reverse(outPath.begin(), outPath.end());

    return found;
}
*/

bool Map::NodeHasObject(int row, int col, ObjectType type,int characterNum){//, int characterNum){
    //for (const WorldObject& obj : grid[row][col].objects) {
    for(int i = 0; i < grid[row][col].objects.size();i++){
        if (grid[row][col].objects[i].type == type&&(grid[row][col].objects[i].locked == -1)){//||grid[row][col].objects[i].locked == characterNum)){
            grid[row][col].objects[i].locked = characterNum;
            return true;
        }
    }
    return false;
}



void Map::AddObject(int row, int col, ObjectType type){
    //WorldObject insertObj;
    WorldObject insertObj = {
    type,
    Vector2{ (float)GetRandomValue(0,9), (float)GetRandomValue(0,9) },
    -1};
    grid[row][col].objects.emplace_back(insertObj);

    updateMap = true;

}

/*
Pair Map::RandomUndiscovered()
{
    if (grid.empty())
        return { -1, -1 };

    std::vector<Pair> candidates;

    for (int r = 0; r < (int)grid.size(); r++) {
        for (int c = 0; c < (int)grid[r].size(); c++) {
            if (!grid[r][c].discovered && grid[r][c].passable) {
                candidates.push_back({ r, c });
            }
        }
    }

    if (candidates.empty())
        return { -1, -1 };

    return candidates[GetRandomValue(0, (int)candidates.size() - 1)];
}


void Map::Discover(int row, int col)
{
    for (int dy = -1; dy <= 1; dy++) {
        for (int dx = -1; dx <= 1; dx++) {

            int nr = row + dy;
            int nc = col + dx;

            if (IsValid(nr, nc)) {
                grid[nr][nc].discovered = true;
            }
        }
    }
}
*/
Pair Map::FindFrontier()
{
    if (grid.empty())
        return { -1, -1 };

    std::vector<Pair> frontiers;
    for (int r = 1; r < grid.size()-1; r++) {
        for (int c = 1; c < grid[r].size()-1; c++) {

            Node& node = grid[r][c];

            if (!node.passable || !node.discovered)
                continue;

            for (int i = 0; i < 4; i++) {
                int nr = r + dRow[i];
                int nc = c + dCol[i];

                if (IsValid(nr, nc) && !grid[nr][nc].discovered) {
                    frontiers.push_back({ r, c });
                    break;
                }
            }
        }
    }

    if (frontiers.empty())
        return { -1, -1 };

    return frontiers[GetRandomValue(0, frontiers.size() - 1)];
}
    


Pair Map::RandomUndiscovered()
{
    std::vector<Pair> candidates;

    for (int r = 1; r < grid.size() - 1; ++r) {
        for (int c = 1; c < grid[r].size() - 1; ++c) {
            if (!grid[r][c].discovered && grid[r][c].passable) {
                candidates.push_back({ r, c });
            }
        }
    }

    if (candidates.empty())
        return { -1, -1 }; // nothing left

    return candidates[GetRandomValue(0, candidates.size() - 1)];
}


void Map::Discover(int row, int col){
    /*
    grid[row+1][col].discovered = true; 
    grid[row+1][col+1].discovered = true; 
    grid[row][col+1].discovered = true; 
    grid[row+1][col-1].discovered = true; 
    grid[row-1][col+1].discovered = true; 
    grid[row-1][col-1].discovered = true; 
    grid[row-1][col].discovered = true; 
    grid[row][col-1].discovered = true; 
    */

    for (int i = -1; i < 2; i++) {
        for (int j = -1; j < 2; j++) {
            if(grid[row + i][col + j].discovered == false){
                grid[row + i][col + j].discovered = true;
                updateMap = true;
            }
            

        }
    }

}


Pair Map::FindIron(Pair location, int characterNum) {
    Pair closest = {-1,-1};
    float distance = FLT_MAX;
    float storeI = -1;
    for (int i = 0; i < iron.size();i++) {
        double dx = location.first - iron[i].first;
        double dy = iron[i].second - location.second;
        float tempDistance = dx * dx + dy * dy;
        if (tempDistance<distance&&grid[iron[i].first][iron[i].second].discovered) {
            storeI = i;
            distance = tempDistance;
            closest = iron[i];
        }
    }
    if (closest.first == -1) {
        //cout << "toomy";
        return closest;
    }
    if (NodeHasObject(closest.first, closest.second, ObjectType::Ore, characterNum)) {
        iron.erase(iron.begin() + storeI); 
        //cout << "larry";
        return closest;
        
    }

    return { -1,-1 };
}


bool Map::UseItems(Pair location, int amount) {
    //std::vector<int> deletes;
    //for (int i = 0; i < grid[location.first][location.second].objects.size()-1;i++) {
        if (grid[location.first][location.second].objects.size()>=amount) {
            //for (int i = 0; i < amount;i++) {
            //    grid[location.first][location.second].objects.pop_back();
            //}
            return true;
        }
        else {
            return false;
        }
}

void Map::DeleteItems(Pair location, int amount) {  
        for (int i = 0; i < amount;i++) {
            grid[location.first][location.second].objects.pop_back();

        }
        updateMap = true;
}

void Map::RemoveWood(int amount) {
    wood -= amount;
}

void Map::RemoveBar(int amount) {
    ironBar -= amount;
}

void Map::RemoveSword(int amount) {
    sword -= amount;
}