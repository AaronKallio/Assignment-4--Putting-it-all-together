#pragma once
#include <vector>
#include <raylib.h>
#include <stack>
#include <set> 
#include <string> 
#include <random>
#include "map.h"


enum Type{
    worker,
    scout,
    soldier,
    craftman

} ;

class Character {

public:
	Vector2 pos;
	Vector2 velocity;
	int state;
	float rotation;
	bool active;
	Vector2 wanderTarget;
	int collisionTimer;
	Vector2 collisionVector;
	int previousPath;
    Type type;
	float speed;
	
	void RequestPath(Map& map, Pair target);
    void Update(float dt);
	Character();

private:
	std::vector<Pair> path;
    size_t currentNode = 0;
	int currentPathIndex;
};
void Place();
void DrawCharacters();
void RunCharacters();