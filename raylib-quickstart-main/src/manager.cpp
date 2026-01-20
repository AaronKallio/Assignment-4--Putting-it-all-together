#include "manager.h"
//int felledTree;
int coal;
extern int wood;
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
}