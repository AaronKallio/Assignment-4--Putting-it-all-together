#include "building.h"
using namespace std;

std::vector<Building> buildings;
Map& gameMap = GetMap();
extern int coal;
extern int wood;
extern int ore;
extern int ironBar;
extern int sword;
Building::Building(){
	position;
	worker = -1;
	type;
	productionTimer = 0;
	timeToProduce;
	usable = false;
}

void DrawBuildings(){
	if (buildings.size()==0) {
		return;
	}
	for (int i = 0; i < buildings.size(); i++) {
		if (buildings[i].type == coalMine) {
			DrawRectangle(buildings[i].position.first, buildings[i].position.second, 6, 6, BLACK);
		}
		else if (buildings[i].type == armsmithsForge) {
			DrawRectangle(buildings[i].position.first, buildings[i].position.second, 6, 6, GRAY);
		}
		else if (buildings[i].type == ironSmelter) {
			DrawRectangle(buildings[i].position.first, buildings[i].position.second, 6, 6, RED);
		}
		else if (buildings[i].type == trainingCamp) {
			DrawRectangle(buildings[i].position.first, buildings[i].position.second, 6, 6, YELLOW);
		}
	}
};

void Building::Produce() {
	if (productionTimer == 0) {
		productionTimer = 0;

	}

	if (productionTimer==timeToProduce) {
		productionTimer = 0;

	}
}

void Building::ProduceCost() {
	if (type == coalMine && wood >= 2) {
		auto& objects = gameMap.grid[97][4].objects;
		int removed = 0;
		for (int i = objects.size() - 1; i >= 0 && removed < 2; --i) {
			if (objects[i].locked == -1) {
				objects.erase(objects.begin() + i);
				removed++;
			}
		}
		if (removed == 2) {
			wood -= 2;
		}
		usable = true;
		return;
	}

	else if (type == ironSmelter && coal >= 3&&ore>=2) {
		auto& objectsOre = gameMap.grid[97][4].objects;
		auto& objectsCoal = gameMap.grid[97][3].objects;
		int removed = 0;
		for (int i = objectsOre.size() - 1; i >= 0 && removed < 2; --i) {
			if (objectsOre[i].locked == -1) {
				objectsOre.erase(objectsOre.begin() + i);
				removed++;
			}
		}
		for (int i = objectsCoal.size() - 1; i >= 0 && removed < 5; --i) {
			if (objectsCoal[i].locked == -1) {
				objectsCoal.erase(objectsCoal.begin() + i);
				removed++;
			}
		}
		if (removed == 5) {
			ore -= 2;
			coal -= 3;
		}
		usable = true;
		return;
	}

	else if (type == armsmithsForge && coal >= 2 && ironBar >= 1) {
		auto& objectsIron = gameMap.grid[96][3].objects;
		auto& objectsCoal = gameMap.grid[97][3].objects;
		int removed = 0;
		for (int i = objectsIron.size() - 1; i >= 0 && removed < 1; --i) {
			if (objectsIron[i].locked == -1) {
				objectsIron.erase(objectsIron.begin() + i);
				removed++;
			}
		}
		for (int i = objectsCoal.size() - 1; i >= 0 && removed < 3; --i) {
			if (objectsCoal[i].locked == -1) {
				objectsCoal.erase(objectsCoal.begin() + i);
				removed++;
			}
		}
		if (removed == 3) {
			ironBar --;
			coal -= 3;
		}
		usable = true;
		return;
	}
	usable = false;
}