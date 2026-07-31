#pragma once
#include <vector>
#include <raylib.h>
#include <stack>
#include <set> 
#include <string> 
#include <random>
#include "map.h"
#include "raymath.h"
#include <iostream> 
//int felledTree = 0;
//typedef pair<int, int> Pair;
enum BuildingType {
	coalMine,
	armsmithsForge,
	ironSmelter,
	trainingCamp
};

class Building {
	public:
	Pair position;
	int worker;
	BuildingType type;
	float productionTimer;
	float timeToProduce;
	void Produce();
	Building();
	bool usable;
	void ProduceCost();
	void PlaceResource();
};

void DrawBuildings();
Pair FindSpot();
void MakeBuildZone();
void MakeNewBuilding(BuildingType build, int prodTime, Pair dest);
void FillList();
void ProduceItems();
void DeleteFromZone(Pair pos);