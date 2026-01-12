#pragma once
#include "vector"
#include <raylib.h>
#include <stack>
#include <set>
#include <string> 
using namespace std;
#include <random>



void FillMap();
void RunMap();
int WhichMap();

enum status {
	used,
	unvisited,
	visited
};
typedef pair<int, int> Pair;

// Creating a shortcut for pair<int, pair<int, int>> type
typedef pair<double, pair<int, int> > pPair;
typedef pair<char, pair<int,int>> Object;
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
	std::vector<Object> objects;
	bool passable;
	char building;
	Node(int x, int y, char t, float pI, float pJ, float f1, float g1, float h1)
		: position{x,y}, type(t), parentI(pI), parentJ(pJ), f(f1), g(g1), h(h1), passable(true){}
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
	void BFS(Pair start, Pair end);
	bool IsValid(int row, int col);
	bool IsUnBlocked(int row, int col);
	bool IsDestination(int row, int col, Pair dest);
	double CalculateHValue(int row, int col, Pair dest);
	void TracePath(Pair dest);
	void AStarSearch(Pair src, Pair dest);
	void DFS(Pair start, Pair end);
	void Dijkstra(Pair src, Pair dest);
	void PlaceTrees(int count);

private:
	void ConnectNodes();
};


void RunAStar(int mapNum);
int WhichPathFinding();
void RunPathFinding(int mapNum);
void ResetMap(int mapNum);
void RunBFS(int mapNum);
void RunDFS(int mapNum);
void RunDijkstra(int mapNum);
void getDirection(Pair start, Pair end, int dRow[8], int dCol[8]);