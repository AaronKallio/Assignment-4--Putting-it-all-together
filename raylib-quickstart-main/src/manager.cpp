#include "manager.h"
//int felledTree;
int coal;
extern int wood;
extern int ore;
int ironBar;
int sword;
int soldiers;
int workers;
int scouts;
int builders;
int craftsmen;
int ironOre;
int coalMine;
int armsmith;
int smelter;
int trainingCamp;

void DrawItemsCollected(){
    std::string str1 = "Wood = " + to_string(wood);
	const char* cstr1 = str1.c_str();
    DrawText(cstr1,257,5,10,ORANGE);
    std::string str2 = "Ore = " + to_string(ore);
    const char* cstr2 = str2.c_str();
    DrawText(cstr2, 317, 5, 10, ORANGE);
}