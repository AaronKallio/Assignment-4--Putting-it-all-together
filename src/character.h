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

enum State {
	cutTree,
	Idle
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
    craftman

} ;

class Character {

public:
	Vector2 pos;
	Vector2 velocity;
	State state;
	float rotation;
	bool active;
	Vector2 wanderTarget;
	int collisionTimer;
	Vector2 collisionVector;
	int previousPath;
    Type type;
	float speed;
	ObjectType currentItem;
	std::vector<Pair> path;
    size_t currentNode = 0;
	int currentPathIndex;
	Task* behaviourTree = nullptr;
	DecisionTreeNode* decisionState;
	int characterNum;


	void RequestPath(Map& map, Pair target);
    void Update(float dt);
	Character();
	void FSM();
	//Pair TreeSearch(Map& m);

private:
	
};
void Place();
void DrawCharacters();
void RunCharacters();
void SetTask();
void RunTask();
void IncreaseItem(ObjectType item);

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

	Vector2 dirTest = Vector2Subtract({dest.second*10+5,dest.first*10+5}, c->pos);
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
		map->RemoveTree(static_cast<int>(c->pos.y / 10),static_cast<int>(c->pos.x / 10),c->characterNum);
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

	Vector2 dirTest = Vector2Subtract({found.second*10+5,found.first*10+5}, c->pos);
	float distTest = Vector2Length(dirTest);
	//std::string elapsedStr1 = to_string(distTest);
	//const char* cstr1 = elapsedStr1.c_str();
	//DrawText(cstr1,500,400,50,PURPLE);
	//c->currentPathIndex < 0 || c->currentPathIndex >= (int)c->path.size()|| 
    if (distTest <=2.5f) {
        pathRequested = false;
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

        map->AddObject(row, col, ObjectType::ChoppedTree);
		IncreaseItem(ObjectType::ChoppedTree);

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
		c->state = Idle;
		return success;
}
};


class ScoutTask : public Task {
private:
    Character* c;
    Map* map;
	bool s;
	Pair dest;

    bool pathRequested = false;

public:
    ScoutTask(Character* character, Map* m,bool scout)
        : c(character), map(m), s(scout) {}

    behaviour Run() override
{
    Pair start{
        static_cast<int>(c->pos.y / 10),
        static_cast<int>(c->pos.x / 10)
    };

    if (!pathRequested) {
        c->path.clear();
        c->currentPathIndex = 0;
		dest = map->RandomUndiscovered();
        bool found = map->AStarSearch(start, dest, c->path,s);

        if (!found || c->path.empty()) {
			pathRequested = false;
            return failure;
        }

        pathRequested = true;
    }

	Vector2 dirTest = Vector2Subtract({dest.second*10+5,dest.first*10+5}, c->pos);
	float distTest = Vector2Length(dirTest);
    if (distTest <=2.5f) {
        pathRequested = false;
		//cout<<"what t"<<'\n';
        return success;
    }

	map->Discover((c->pos.y / 10),(c->pos.x / 10));
	c->Update(GetFrameTime());
    return running;
}
};
