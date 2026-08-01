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
//int coalMines;
//int armsmiths;
//int smelters;
//int trainingCamps;
int swordsForUse = 0;
int buildingCount = 0;
int workerForBuilding = 12;
bool soldiersRecruited = false;
std::vector<int> futureSoldiers;


void DrawItemsCollected(){
    std::string str1 = "Wood = " + to_string(wood);
	const char* cstr1 = str1.c_str();
    DrawText(cstr1,257,5,10,ORANGE);
    std::string str2 = "Ore = " + to_string(ore);
    const char* cstr2 = str2.c_str();
    DrawText(cstr2, 317, 5, 10, ORANGE);
    std::string str3 = "Coal = " + to_string(coal);
    const char* cstr3 = str3.c_str();
    DrawText(cstr3, 377, 5, 10, ORANGE);
    std::string str4 = "Iron Bar = " + to_string(ironBar);
    const char* cstr4 = str4.c_str();
    DrawText(cstr4, 437, 5, 10, ORANGE);
    std::string str5 = "Sword = " + to_string(sword);
    const char* cstr5 = str5.c_str();
    DrawText(cstr5, 507, 5, 10, ORANGE);



    std::string soldierCount = "Soldiers = " + to_string(soldiers);
    const char* soldierCount1 = soldierCount.c_str();
    DrawText(soldierCount1, 750, 5, 10, ORANGE);

    std::string instructions = "press 1 for 1x, 2 for 10x, 3 for 100x";
    const char* instructions1 = instructions.c_str();
    DrawText(instructions1, 750, 990, 10, ORANGE);
    

}
extern Map& map;
extern std::vector<Character> characters;
extern std::vector<Building> buildings;
extern std::vector<BuildingType> buildList;


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
   // cout << "test " << '\n';
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
    
    for (int j = 0; j < 12; j++) {
        workers++;
        characters[j].behaviourTree = BuildCutTreeTree(&characters[j], map.woodLocation, &map);
    }


    for (int j = 12; j < 17; j++) {
        craftsmen++;
        characters[j].behaviourTree = BuildCraftsmanTree(&characters[j], craftman);
    }

    //for (int t = characters.size() - 15; t < characters.size() - 11; t++) {
    //    ironMiners++;
    //    characters[t].behaviourTree = BuildMineTree(&characters[t], map.ironLocation, &map);
    //}
    builders++;
    characters[17].behaviourTree = BuildUpgradeBuilderTree(&characters[17], builder, &map, &buildList);

    for (int t = 18; t < 30; t++) {
        ironMiners++;
        characters[t].behaviourTree = BuildMineTree(&characters[t], map.ironLocation, &map);
    }
    for (int i = 30; i < 50; i++) {
        scouts++;
        characters[i].type = scout;
        characters[i].behaviourTree = BuildUpgradeScoutTree(&characters[i], scout, &map);
    }
}

void ReturnToWork() {
    for (int i = 0; i < characters.size(); i++) {
        if (characters[i].state==idle && (characters[i].type == worker || characters[i].type == builder)) {

            if (characters[i].type == builder) {
                //cout << "bruhv";
                characters[i].behaviourTree = BuildBuilderTree(&characters[17], builder, &map, &buildList);
            }

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
            CraftmenAssign();
        }
    }
}

void CraftmenAssign() {
    if (buildingCount< buildings.size()) {
        //cout << "buns";
        buildings[buildingCount].worker = workerForBuilding;
        characters[workerForBuilding].behaviourTree = BuildCraftsmanTravelTree(&characters[workerForBuilding], builder, &map,
            { buildings[buildingCount].position.first, buildings[buildingCount].position.second});
        characters[workerForBuilding].state = active;
        characters[workerForBuilding].behaviourTree->Run();
        buildingCount++;
        workerForBuilding++;
    }
}


void AssignSoldiers() {
    for (int i = 0; i < 10; i++) {
        futureSoldiers.push_back(i);
    }
    for (int j = 18; j < 28; j++) {
        futureSoldiers.push_back(j);
    }

}

void SoldierSlots() {
    if (sword>=20&&!soldiersRecruited) {
        //soldiers++;
        //swordsForUse 
        for (int i = 0; i < futureSoldiers.size(); i++) {
            soldiers++;
            characters[futureSoldiers[i]].behaviourTree = BuildSoldierTree(&characters[futureSoldiers[i]], &map);
            characters[futureSoldiers[i]].state = active;
            characters[futureSoldiers[i]].behaviourTree->Run();
            soldiersRecruited = true;
        }
    }
}