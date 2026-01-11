#pragma once
#include "vector"
#include <raylib.h>
#include <stack>
#include <set>
#include <string> 
using namespace std;


void FillMap();
void RunMap(int num);
int WhichMap();

enum status {
	used,
	unvisited,
	visited
};

class Node {
public:
	Vector2 position;
	char type;
	std::vector<Node*> connectedNodes;
	status nodeStatus;
	int parentI;
	int parentJ;
	float f;
	float g;
	float h;
	Node(int x, int y, char t, status s, float pI, float pJ, float f1, float g1, float h1)
		: position{x,y}, type(t),nodeStatus(s), parentI(pI), parentJ(pJ), f(f1), g(g1), h(h1){}
};
// Creating a shortcut for int, int pair type
typedef pair<int, int> Pair;

// Creating a shortcut for pair<int, pair<int, int>> type
typedef pair<double, pair<int, int> > pPair;
class Map {
public:
	std::vector<std::vector<Node>> grid;
	int width;
	int height;
	stack<Pair> path;
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