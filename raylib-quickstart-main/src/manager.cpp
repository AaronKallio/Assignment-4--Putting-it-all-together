#include "manager.h"
//int felledTree;
extern int coal;
extern int wood;
extern int ore;
extern int ironBar;
extern int sword;
int soldiers;
int workers;
int ironMiners;
int scouts;
int builders;
int craftsmen;
int coalMines;
int armsmiths;
int smelters;
int trainingCamps;
std::vector<BuildingType> buildList;

void DrawItemsCollected(){
    std::string str1 = "Wood = " + to_string(wood);
	const char* cstr1 = str1.c_str();
    DrawText(cstr1,257,5,10,ORANGE);
    std::string str2 = "Ore = " + to_string(ore);
    const char* cstr2 = str2.c_str();
    DrawText(cstr2, 317, 5, 10, ORANGE);
}
extern Map& map;
extern std::vector<Character> characters;
extern std::vector<Building> buildings;



void Place() {
    constexpr size_t CHARACTER_COUNT = 50;
    characters.clear();
    characters.reserve(CHARACTER_COUNT);
    for (size_t i = 0; i < CHARACTER_COUNT; ++i) {
        Character c;
        c.characterNum = i;
        c.pos = { (float)i * 0.5f + 22.5f, 975.0f };
        characters.emplace_back(c);
    }
    cout << "Characters placed: " << characters.size() << '\n';
    cout << "test " << '\n';
}


/*
void SetTask() {
    for (int i = 0; i < 5; i++) {
        characters[i].type = scout;
        characters[i].behaviourTree = BuildScoutTree(&characters[i], &map);
    }
    for (int j = 5; j < characters.size() - 10; j++) {
        characters[j].behaviourTree = BuildCutTreeTree(&characters[j], { 97,4 }, &map);
    }
    for (int t = characters.size() - 10; t < characters.size() - 5; t++) {
        characters[t].behaviourTree = BuildMineTree(&characters[t], { 97,4 }, &map);
    }
    for (int k = characters.size() - 5; k < characters.size(); k++) {
        characters[k].behaviourTree = BuildUpgradeTree(&characters[k], scout);
    }
}
*/
void SetTask() {
    for (int i = 0; i < 5; i++) {
        scouts++;
        characters[i].type = scout;
        characters[i].behaviourTree = BuildUpgradeScoutTree(&characters[i],scout, &map);
    }
    for (int j = 5; j < characters.size() - 5; j++) {
        workers++;
        characters[j].behaviourTree = BuildCutTreeTree(&characters[j], { 97,4 }, &map);
    }
    for (int t = characters.size() - 5; t < characters.size(); t++) {
        ironMiners++;
        characters[t].behaviourTree = BuildMineTree(&characters[t], { 96,4 }, &map);
    }
}

void ReturnToWork() {
    for (int i = 0; i < characters.size(); i++) {
        if (characters[i].state==idle && characters[i].type == worker) {
            characters[i].state = active;
            characters[i].behaviourTree->Run();
       }
    }
}



void RunTask() {
    for (int i = 0; i < characters.size(); i++) {
        if (characters[i].state != idle) {
            //characters[i].state = active;
            characters[i].behaviourTree->Run();
        }
    }
}