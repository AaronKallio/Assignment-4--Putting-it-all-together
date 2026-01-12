#pragma once
#include <vector>
#include <raylib.h>
#include <stack>
#include <set> 
#include <string> 
#include <random>


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
	Character();
	//~Character();
};
void Place();
void DrawCharacters();