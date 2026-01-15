#pragma once
#include <vector>
#include <raylib.h>
#include <stack>
#include <set>
#include <string> 
using namespace std;
#include <random>
#include <utility>
#include <cfloat>


void FillMap();
void RunMap();
int WhichMap();

//enum class ObjectType : char {
//    Tree = 'T',
//    Ore  = 'O',
//    Enemy = 'E'
//};

enum status {
	used,
	unvisited,
	visited
};
typedef pair<int, int> Pair;

enum class ObjectType {
	None,
    Tree,
	ChoppedTree,
    Ore
};
struct WorldObject {
    ObjectType type;
    Vector2 localPos; 
    int locked = -1;
};

// Creating a shortcut for pair<int, pair<int, int>> type
typedef pair<double, pair<int, int> > pPair;
//typedef pair<char, pair<int,int>> Object;
//std::vector<WorldObject> objects;
class Node {
public:
	Vector2 position;
	char type;
	std::vector<Node*> connectedNodes;
	int parentI;
	int parentJ;
	float f;
	float g;
	float h;
	std::vector<WorldObject> objects;
	bool passable;
	float moveCost;
	char building;
	bool discovered;
	Node(int x, int y, char t, float pI, float pJ, float f1, float g1, float h1)
		: position{x,y}, type(t), parentI(pI), parentJ(pJ), f(f1), g(g1), h(h1), passable(true), discovered(false){}
	void PlaceTrees(int count);
};
// Creating a shortcut for int, int pair type

class Map {
public:
	std::vector<std::vector<Node>> grid;
	int width;
	int height;
	void MapFill(const std::string& fileName);
	void DrawMap();
	Map();
	~Map();
	//void BFS(Pair start, Pair end);
	//void DFS(Pair start, Pair end);
	//void Dijkstra(Pair src, Pair dest);
	void PlaceTrees(int count);

	void RemoveTree(int y, int x,int characterNum);
	//bool LockObjectAt(int row, int col, ObjectType type, int characterNum);
	bool AStarSearch(Pair start, Pair goal, std::vector<Pair>& outPath);
	const std::vector<Vector2>& GetLastPath() const;
	Node& GetNodeAtWorld(Vector2 worldPos);

	Pair BFS(Pair start, ObjectType theObject, std::vector<Pair>& outPath, int characterNum);
	bool NodeHasObject(int row, int col, ObjectType type,int characterNum);
	void AddObject(int row, int col, ObjectType type);
private:
	void ConnectNodes();
	struct AStarNode {
    float g = FLT_MAX;
    float h = 0.0f;
    float f = FLT_MAX;
    int parentRow = -1;
    int parentCol = -1;
};
    bool IsValid(int row, int col) const;
    bool IsWalkable(int row, int col) const;
    float Heuristic(int row, int col, Pair dest) const;
	std::vector<Vector2> lastPath;
};


void RunAStar(int mapNum);
int WhichPathFinding();
void RunPathFinding(int mapNum);
void ResetMap(int mapNum);
void RunBFS(int mapNum);
void RunDFS(int mapNum);
void RunDijkstra(int mapNum);
void getDirection(Pair start, Pair end, int dRow[8], int dCol[8]);
Map& GetMap();
void InitObjectTextures();