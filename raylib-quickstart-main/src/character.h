#pragma once
#include <vector>
#include <raylib.h>
#include <stack>
#include <set> 
#include <string> 
#include <random>
#include "map.h"
#include "building.h"
#include "raymath.h"
#include <iostream> 
//int felledTree = 0;

enum State {
	//cutTree,
	//mine,
	//scout,
	active,
	idle
};
enum behaviour {
	success,
	failure,
	running
};


class BTNode {
public:
	virtual behaviour Tick() = 0;
	virtual ~BTNode() = default;
};
class DecisionTreeNode {
public:
	virtual DecisionTreeNode* MakeDecision() = 0;
	virtual ~DecisionTreeNode() = default;
};
class Task {
public:
	virtual behaviour Run() = 0;
	virtual ~Task() = default;
};


class Action : public DecisionTreeNode {
public:
	DecisionTreeNode* MakeDecision() override {
		return this;
	}
};


class Decision : public DecisionTreeNode {
protected:
	DecisionTreeNode* trueNode = nullptr;
	DecisionTreeNode* falseNode = nullptr;

public:
	virtual bool TestValue() = 0;

	void SetBranches(DecisionTreeNode* t, DecisionTreeNode* f) {
		trueNode = t;
		falseNode = f;
	}

	DecisionTreeNode* GetBranch() {
		return TestValue() ? trueNode : falseNode;
	}

	DecisionTreeNode* MakeDecision() override {
		DecisionTreeNode* branch = GetBranch();

		if (branch == this) {
			return this;
		}

		return branch->MakeDecision();
	}
};

class FloatDecision : public Decision {
private:
	float *value; 
	float minValue;
	float maxValue;

public:
	FloatDecision(float* ptr, float min, float max)
		: value(ptr), minValue(min), maxValue(max) {
	}
	bool TestValue() override {
		if (std::isnan(*value)) {
			return false;
		}
		return (*value >= minValue && *value <= maxValue);
	}
	DecisionTreeNode* GetBranch() {
		if (TestValue()) {
			return trueNode;
		}
		return falseNode;
	}
};

class IntDecision : public Decision {
private:
	int* value;
	int minValue;
	int maxValue;

public:
	IntDecision(int* ptr, int min, int max)
		: value(ptr), minValue(min), maxValue(max) {
	}
	bool TestValue() override {
		return (*value >= minValue && *value <= maxValue);
	}
	DecisionTreeNode* GetBranch() {
		if (TestValue()) {
			return trueNode;
		}
		return falseNode;
	}
};

class CharacterDecision : public Decision {
private:
	string* value;

public:
	CharacterDecision(string* ptr)
		: value(ptr) {
	}
	bool TestValue() override {
		if (*value=="") {
			return false;
		}
		return true;
	}
	DecisionTreeNode* GetBranch() {
		if (TestValue()) {
			return trueNode;
		}
		return falseNode;
	}
};


class StateAction : public Action {
public:
	State result;

	StateAction(State state) {
		result = state;
	}
};

//DecisionTreeNode* BuildStateDecisionTree(Character* character);

class Sequence : public Task {
private:
	std::vector<Task*> children;
	size_t currentIndex = 0;

public:
	void AddChild(Task* task) {
		children.push_back(task);
	}

	behaviour Run() override {
		while (currentIndex < children.size()) {
			behaviour result = children[currentIndex]->Run();

			if (result == running) {
				return running; // pause here
			}

			if (result == failure) {
				currentIndex = 0; // reset for next time
				return failure;
			}
			currentIndex++;
		}

		currentIndex = 0;
		return success;
	}
};

class Selector : public Task {
private:
	std::vector<Task*> children;
	size_t currentIndex = 0;

public:
	void AddChild(Task* task) {
		children.push_back(task);
	}

	behaviour Run() override {
		while (currentIndex < children.size()) {
			behaviour result = children[currentIndex]->Run();

			if (result == running) {
				return running;
			}

			if (result == success) {
				currentIndex = 0;
				return success;
			}
			currentIndex++;
		}
		currentIndex = 0;
		return failure;
	}
};




enum Type{
    worker,
    scout,
    soldier,
	builder,
	coalMiner,
	armSmith,
	smelter,
    craftman

} ;

class Character {

public:
	Vector2 pos;
	//Vector2 velocity;
	State state;
	//float rotation;
	//bool active;
	//Vector2 wanderTarget;
	//int collisionTimer;
	//Vector2 collisionVector;
	//int previousPath;
    Type type;
	float speed;
	ObjectType currentItem;
	std::vector<Pair> path;
	int currentPathIndex;
	Task* behaviourTree = nullptr;
	DecisionTreeNode* decisionState;
	int characterNum;
	//bool inTask = false;

	void RequestPath(Map& map, Pair target);
    void Update(float dt);
	Character();


	Pair soldierGather = { 97,4 };
	Pair trainingGroundPos = { 97,2 };
	//void FSM();
	//Pair TreeSearch(Map& m);

private:
	
};
void Place();
void DrawCharacters();
void RunCharacters();
void SetTask();
void RunTask();
void IncreaseItem(ObjectType item);
Task* BuildScoutTree(Character* c, Map* m);
Task* BuildCutTreeTree(Character* c, Pair destination, Map* m);
Task* BuildMineTree(Character* c, Pair destination, Map* m);
Task* BuildUpgradeTree(Character* c, Type t);
Task* BuildUpgradeScoutTree(Character* c, Type t, Map* m);

Task* BuildUpgradeBuilderTree(Character* c, Type t, Map* m, std::vector<BuildingType>* buildingList);
Task* BuildBuilderTree(Character* c, Type t, Map* m, std::vector<BuildingType>* buildingList);
Task* BuildCraftsmanTree(Character* c, Type t);
Task* BuildCraftsmanTravelTree(Character* c, Type t, Map* m, Pair dest);

Task* BuildSoldierTree(Character* c, Map* m);


class TravelTask : public Task {
private:
    Character* c;
    Pair dest;
    Map* map;
	bool s;

    bool pathRequested = false;

public:
    TravelTask(Character* character, Pair destination, Map* m,bool scout)
        : c(character), dest(destination), map(m), s(scout) {}

    behaviour Run() override
{
    Pair start{
        static_cast<int>(c->pos.y / 10),
        static_cast<int>(c->pos.x / 10)
    };

    if (!pathRequested) {
        c->path.clear();
        c->currentPathIndex = 0;

        bool found = map->AStarSearch(start, dest, c->path,s);

        if (!found || c->path.empty()) {
            pathRequested = false;
            return failure;
        }

        pathRequested = true;
    }

	Vector2 dirTest = Vector2Subtract({(float)dest.second*10+5,(float)dest.first*10+5}, c->pos);
	float distTest = Vector2Length(dirTest);
	//std::string elapsedStr1 = to_string(distTest);
	//const char* cstr1 = elapsedStr1.c_str();
	//DrawText(cstr1,500,400,50,PURPLE);
	//c->currentPathIndex < 0 || c->currentPathIndex >= (int)c->path.size()|| 
    if (distTest <=2.5f) {
        pathRequested = false;
		//cout<<"what t"<<'\n';
        return success;
    }
	/*
    Pair next = c->path[c->currentPathIndex];
	
    Vector2 target{
        next.second * 10.0f + 5.0f,
        next.first  * 10.0f + 5.0f
    };

    Vector2 dir{
        target.x - c->pos.x,
        target.y - c->pos.y
    };

    float dist = Vector2Length(dir);

    if (dist < 1.0f) {
        c->currentPathIndex++;
        return running;
    }

	*/
    //dir = Vector2Normalize(dir);
    //c->pos.x += dir.x * c->speed * GetFrameTime();
    //c->pos.y += dir.y * c->speed * GetFrameTime();
	c->Update(GetFrameTime());
	//std::string elapsedStr = to_string(c->currentPathIndex);
	//const char* cstr = elapsedStr.c_str();
	//DrawText(cstr,500,500,50,PURPLE);
    return running;
}
};




class ChopTask : public Task {
private:
    Character* c;
	Map* map;
    float elapsed = 0.0f;
    static constexpr float chopTime = 30.0f;

public:
    ChopTask(Character* character, Map* m)
        : c(character), map(m) {}

    behaviour Run() override{
        // Accumulate time
        elapsed += GetFrameTime();

        // Still chopping
        if (elapsed < chopTime) {
			//DrawText("workin",500,500,50,PURPLE);
            return running;
        }

        // Finished chopping
        elapsed = 0.0f; // reset for next use
		//DrawText("rdgggggggggggggggggggg",500,500,50,BLACK);
		c->currentItem = ObjectType::ChoppedTree;
		map->RemoveItem(static_cast<int>(c->pos.y / 10),static_cast<int>(c->pos.x / 10),c->characterNum,ObjectType::Tree);
		//c->pos = {0,0};
		//cout<<"idk man";
        return success;
    }
};


class FindTreeTask : public Task {
private:
    Character* c;
    //Pair dest;
    Map* map;
	Pair found;
	Pair notFound = {-1,-1};
    bool pathRequested = false;

public:
    FindTreeTask(Character* character, Map* m)
        : c(character),  map(m) {}

    behaviour Run() override{
    Pair start{
        static_cast<int>(c->pos.y / 10),
        static_cast<int>(c->pos.x / 10)
    };

    if (!pathRequested) {
        c->path.clear();
        c->currentPathIndex = 0;

        found = map->BFS(start, ObjectType::Tree, c->path,c->characterNum);
		

        if (found == notFound || c->path.empty()) {
            pathRequested = false;
            return failure;
        }
		
        pathRequested = true;
    }

	Vector2 dirTest = Vector2Subtract({ (float)found.second*10+5,(float)found.first*10+5}, c->pos);
	float distTest = Vector2Length(dirTest);
    if (distTest <=2.5f) {
        pathRequested = false;
        return success;
    }
	c->Update(GetFrameTime());
    return running;
	}
};



class DepositTask : public Task {
private:
    Character* c;
    //Pair dest;
    Map* map;
	ObjectType item;

public:
    DepositTask(Character* character, Map* m, ObjectType it)
        : c(character),  map(m), item(it) {}

    behaviour Run() override {
    if (c->currentItem == item) {

        c->currentItem = ObjectType::None;

        int row = static_cast<int>(c->pos.y / 10);
        int col = static_cast<int>(c->pos.x / 10);

        map->AddObject(row, col, item);
		IncreaseItem(item);

        //cout << "deposit success\n";
        return success;
    }

    return failure;
}
};

class IdleTask : public Task {
private:
    Character* c;


public:
    IdleTask(Character* character)
        : c(character) {}

    behaviour Run() override {
		c->state = idle;
		return success;
}
};



class ScoutTask : public Task {
	private: Character* c; 
	Map* map; 
	Pair dest; 
	bool pathRequested = false; 

	public: 
	ScoutTask(Character* character, Map* m)
	: c(character), map(m) {}
	behaviour Run() override { 
		Pair start{ static_cast<int>(c->pos.y / 10), static_cast<int>(c->pos.x / 10) }; 
		if (!pathRequested) { 
			c->path.clear(); c->currentPathIndex = 0;
			if (GetRandomValue(1, 10)!=1) {
				dest = map->FindFrontier();
			}
			else {
				dest = map->RandomUndiscovered();
			}
			//dest = map->RandomUndiscovered();
			//dest = map->FindFrontier();
			bool found = map->AStarSearch(start, dest, c->path,true); 
			//if (!found || c->path.empty()) { 
			//	pathRequested = false; 
			//	return failure; 
			//} 
			pathRequested = true; 
		} 
		Vector2 dirTest = Vector2Subtract({ (float)dest.second*10+5,(float)dest.first*10+5}, c->pos);
		float distTest = Vector2Length(dirTest); 
		if (distTest <=2.5f) { 
			pathRequested = false; 
			//dest = map->RandomUndiscovered(); 
			return running; 
		} 
		map->Discover((c->pos.y / 10),(c->pos.x / 10)); 
		c->Update(GetFrameTime()); 
		return running; 
	} 
}; 


class UpgradeTask : public Task {
private:
	Character* c;
	//Map* map;
	float elapsed = 0.0f;
	 float u = 60.0f;
	Type t;

public:
	UpgradeTask(Character* character, Type desiredType, int upgradeTime)
		: c(character), t(desiredType), u(upgradeTime){
	}

	behaviour Run() override {
		elapsed += GetFrameTime();
		if (elapsed < u) {
			return running;
		}
		elapsed = 0.0f; // reset for next use
		c->type = t;
		return success;
	}
};



class FindIronTask : public Task {
private:
	Character* c;
	//Pair dest;
	Map* map;
	Pair found;
	Pair notFound = { -1,-1 };
	bool pathRequested = false;

public:
	FindIronTask(Character* character, Map* m)
		: c(character), map(m) {
	}

	behaviour Run() override {
		Pair start{
			static_cast<int>(c->pos.y / 10),
			static_cast<int>(c->pos.x / 10)
		};

		if (!pathRequested) {
			c->path.clear();
			c->currentPathIndex = 0;

			found = map->FindIron(start, c->characterNum);
			//cout << "buddy";
			if (found == notFound) {
				//cout << "buddy";
				pathRequested = false;
				return failure;
			}
			map->AStarSearch(start, found, c->path, false);
			if (found == notFound || c->path.empty()) {
				//cout << "buddy";
				pathRequested = false;
				return failure;
			}
			//bool runLock = map->NodeHasObject(found.second, found.first, ObjectType::Ore,c->characterNum);
			pathRequested = true;
		}

		Vector2 dirTest = Vector2Subtract({ (float)found.second * 10 + 5,(float)found.first * 10 + 5 }, c->pos);
		float distTest = Vector2Length(dirTest);
		if (distTest <= 2.5f) {
			pathRequested = false;
			map->RemoveItem(static_cast<int>(c->pos.y / 10), static_cast<int>(c->pos.x / 10), c->characterNum, ObjectType::Ore);
			c->currentItem = ObjectType::Ore;
			return success;
		}
		c->Update(GetFrameTime());
		return running;
	}
};



/*
class BuildSpotTask : public Task {
private:
	Character* c;
	//Pair dest;
	Map* map;
	Pair found;
	Pair notFound = { -1,-1 };
	bool pathRequested = false;

public:
	BuildSpotTask(Character* character, Map* m)
		: c(character), map(m) {
	}

	behaviour Run() override {
		Pair start{
			static_cast<int>(c->pos.y / 10),
			static_cast<int>(c->pos.x / 10)
		};

		if (!pathRequested) {
			c->path.clear();
			c->currentPathIndex = 0;

			found = FindSpot();
			//cout << "buddy";
			if (found == notFound) {
				//cout << "buddy";
				pathRequested = false;
				return failure;
			}
			map->AStarSearch(start, found, c->path, false);
			if (found == notFound || c->path.empty()) {
				//cout << "buddy";
				pathRequested = false;
				return failure;
			}
			//bool runLock = map->NodeHasObject(found.second, found.first, ObjectType::Ore,c->characterNum);
			pathRequested = true;
		}

		Vector2 dirTest = Vector2Subtract({ (float)found.second * 10 + 5,(float)found.first * 10 + 5 }, c->pos);
		float distTest = Vector2Length(dirTest);
		if (distTest <= 2.5f) {
			pathRequested = false;
			//map->RemoveItem(static_cast<int>(c->pos.y / 10), static_cast<int>(c->pos.x / 10), c->characterNum, ObjectType::Ore);
			//c->currentItem = ObjectType::Ore;
			return success;
		}
		c->Update(GetFrameTime());
		return running;
	}
};



class BuildTask : public Task {
private:
	Character* c;
	Map* map;
	float elapsed = 0.0f;
	static constexpr float chopTime = 30.0f;
	BuildingType b;

public:
	BuildTask(Character* character, Map* m, BuildingType building)
		: c(character), map(m), b(building) {
	}

	behaviour Run() override {
		// Accumulate time
		elapsed += GetFrameTime();

		if (elapsed < chopTime) {
			//DrawText("workin",500,500,50,PURPLE);
			return running;
		}



		elapsed = 0.0f; // reset for next use
		return success;
	}
};


*/





class BuilderTask : public Task {
private: Character* c;
	   Map* map;
	   std::vector<BuildingType>* list;
	   Pair dest;
	   BuildingType building;
	   bool go = false;
	   float elapsed = 0.0f;
	   float buildTime;
	   bool buildingNow = false;
	   int prodTime;
public:
	BuilderTask(Character* character, Map* m, std::vector<BuildingType>* buildingList)
		: c(character), map(m), list(buildingList) {
	}
	behaviour Run() override {
		if (list->empty()) {
			//cout << "fail 1 ";
			return failure;
		}
		if (buildingNow) {

			elapsed += GetFrameTime();
			//cout << "  " << buildTime << "  ";
			//cout << buildingNow;
			if (elapsed < buildTime) {
				//cout << "running 1 ";
				//cout << elapsed << "  ";
				return running;
			}

			if (elapsed >= buildTime) {
				MakeNewBuilding(building, prodTime, dest);
				//cout << "success 1 ";
				return success;
			}
		}
		
		if (!go) {
			bool resources = map->UseItems(map->woodLocation,10);
			if (!resources) {
				//cout << map->woodLocation.first;
				//cout << map->woodLocation.second;
				//cout << "fail 2 ";
				return failure;
			}
			dest = FindSpot();
			Pair fail = { -1,-1 };
			if (dest == fail) {
				//cout << "fail 3 ";
				return failure;
			}
			//building = list->back();
			building = list->front();
			switch (building) {
			case coalMine:
				DeleteFromZone(dest);
				map->DeleteItems(map->woodLocation, 10);
				map->RemoveWood(10);
				prodTime = 30;
				buildTime = 60;
				break;
			case armsmithsForge:

				if (!map->UseItems(map->barLocation, 3)) {
					std::cout << "fails";
					return failure;
				}
				
				DeleteFromZone(dest);
				map->DeleteItems(map->woodLocation, 10);
				map->DeleteItems(map->barLocation, 3);
				map->RemoveWood(10);
				map->RemoveBar(3);
				prodTime = 60;
				buildTime = 180;
				break;
			case ironSmelter:
				//cout << " brother buddy ";
				DeleteFromZone(dest);
				map->DeleteItems(map->woodLocation, 10);
				map->RemoveWood(10);
				prodTime = 30;
				buildTime = 120;
				break;
			case trainingCamp:
				DeleteFromZone(dest);
				map->DeleteItems(map->woodLocation, 10);
				map->RemoveWood(10);
				prodTime = 60;
				buildTime = 120;
				c->trainingGroundPos = dest;
				break;
			}

			
			//found = map->BFS(start, ObjectType::Tree, c->path, c->characterNum);
			cout << list->size();
			c->path.clear();
			c->currentPathIndex = 0;
			//list->pop_back();
			list->erase(list->begin());
			go = true;


			Pair start{
			static_cast<int>(c->pos.y / 10),
			static_cast<int>(c->pos.x / 10)
			};

			map->AStarSearch(start, dest, c->path, false);
		}
		

		Vector2 dirTest = Vector2Subtract({ (float)dest.second * 10 + 5,(float)dest.first * 10 + 5 }, c->pos);
		float distTest = Vector2Length(dirTest);

		DrawCircle(
			static_cast<int>(c->pos.x),
			static_cast<int>(c->pos.y),
			300,
			PINK
		);
		//cout << c->pos.x<<" ";
		//cout << c->pos.y << " ";
		if (distTest <= 2.5f) {
			buildingNow = true;
			//cout << "running 2";
			return running;
		}
		c->Update(GetFrameTime());
		//cout << "running 3";
		DrawCircle(
			static_cast<int>(dest.first * 10 + 5),
			static_cast<int>(dest.second * 10 + 5),
			200,
			ORANGE
		);
		return running;
	
	}
};

class SoldierTask : public Task {
private:
	Character* c;
	//Pair dest;
	Map* map;
	//ObjectType item;

public:
	SoldierTask(Character* character, Map* m)
		: c(character), map(m) {
	}

	behaviour Run() override {

		map->DeleteItems(map->swordLocation, 1);
		map->RemoveSword(1);
		return success;

	}
};
