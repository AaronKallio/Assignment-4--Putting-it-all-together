#include "character.h"
using namespace std;
extern int coal;
extern int wood;
extern int ore;
extern int ironBar;
extern int sword;

std::vector<Character> characters;
//Map map;
Map& map = GetMap();
Character::Character(){
    pos = { 50, 50 };
    //velocity = { 0, 0 };
    //rotation = 0.0f;

    //active = true;
    speed = 60.0f;

    state = active;
    type = worker;

    currentItem = ObjectType::None;

    currentPathIndex = 0;

    behaviourTree = nullptr;
    decisionState = nullptr;
}
/*
void Place(){
    constexpr size_t CHARACTER_COUNT = 50;
    characters.clear();
    characters.reserve(CHARACTER_COUNT);
    for (size_t i = 0; i < CHARACTER_COUNT; ++i) {
        Character c;
        c.characterNum = i;
		c.pos = {(float)i * 0.5f +22.5f, 975.0f};
        characters.emplace_back(c);
    }
    cout << "Characters placed: " << characters.size() << '\n';
}
*/
void DrawCharacters(){
    for (const Character& c : characters) {
       //cout << c.pos.x << '\n'; 
        switch(c.type){
        case worker:
            DrawCircle(
                static_cast<int>(c.pos.x),
                static_cast<int>(c.pos.y),
                1,
                BLUE
            );
            break;
        case scout:
            DrawCircle(
                static_cast<int>(c.pos.x),
                static_cast<int>(c.pos.y),
                1,
                ORANGE
            );
            break;
        case builder:
            DrawCircle(
                static_cast<int>(c.pos.x),
                static_cast<int>(c.pos.y),
                1,
                PURPLE
            );
            break;
        
        }
        
        //DrawCircle(c.pos.x,c.pos.y,1000,RED);
        //DrawCircle(500,500,1000,RED);
    }
}
void RunCharacters(){
	for(int i = 0; i < characters.size();i++){
        if(characters[i].state!=idle){
            characters[i].Update(GetFrameTime());
        }
		//characters[i].Update(GetFrameTime());
	}
}




void Character::RequestPath(Map& map, Pair target)
{

    Pair start = { (int)(pos.y / 10.0f), (int)(pos.x / 10.0f) }; 
    Pair goal  = target; 

    if (map.AStarSearch(start, goal, path,false)) {
        currentPathIndex = 0;
    }
}


void Character::Update(float dt){
    if (currentPathIndex >= path.size())
        return;


    Vector2 target = { path[currentPathIndex].second * 10.0f + 5, path[currentPathIndex].first * 10.0f + 5};
    Vector2 dir = Vector2Subtract(target, pos);
    float dist = Vector2Length(dir);
    if (dist < 2.0f) {
        currentPathIndex++;
        return;
    }

	int col = int(pos.x / 10);
	int row = int(pos.y / 10);
	if(row < 0 || row >= map.height || col < 0 || col >= map.width) {
    	return; // ignore update if out of bounds
	}
    Node& tile = map.GetNodeAtWorld(pos);

    float finalSpeed = 1 / tile.moveCost;
    dir = Vector2Normalize(dir);
    pos += dir * finalSpeed * dt;
}





Task* BuildCutTreeTree(Character* c, Pair destination, Map* m)
{
	Selector* root = new Selector();
	Sequence* chopSequence = new Sequence();
	//chopSequence -> AddChild(new TravelTask(c,destination,m)); 

    
    chopSequence -> AddChild(new FindTreeTask(c,m)); 
	chopSequence -> AddChild(new ChopTask(c,m)); 
    chopSequence -> AddChild(new TravelTask(c,destination,m,false)); 
    chopSequence -> AddChild(new DepositTask(c,m,ObjectType::ChoppedTree)); 
	IdleTask* idle = new IdleTask(c); 

	root->AddChild(chopSequence);
	root->AddChild(idle);

	return root;
}

Task* BuildUpgradeTree(Character* c,Type t)
{
    Selector* root = new Selector();
    //Sequence* chopSequence = new Sequence();
    //chopSequence -> AddChild(new TravelTask(c,destination,m)); 


    
    UpgradeTask* upgrade = new UpgradeTask(c,t);

    root->AddChild(upgrade);

    return root;
}



Task* BuildUpgradeScoutTree(Character* c, Type t, Map* m)
{
    Selector* root = new Selector();
    //Sequence* chopSequence = new Sequence();
    //chopSequence -> AddChild(new TravelTask(c,destination,m)); 
    Sequence* scoutSequence = new Sequence();
    scoutSequence->AddChild(new UpgradeTask(c, t));
    scoutSequence->AddChild(new ScoutTask(c, m));

    //UpgradeTask* upgrade = new UpgradeTask(c, t);
    IdleTask* idle = new IdleTask(c);

    root->AddChild(scoutSequence);
    root->AddChild(idle);
    return root;
}


Task* BuildScoutTree(Character* c, Map* m)
{
	Selector* root = new Selector();
	//Sequence* chopSequence = new Sequence();
	//chopSequence -> AddChild(new TravelTask(c,destination,m)); 

    
    ScoutTask* scout = new ScoutTask(c,m);
	IdleTask* idle = new IdleTask(c); 

	root->AddChild(scout);
	root->AddChild(idle);

	return root;
}


Task* BuildMineTree(Character* c, Pair destination, Map* m)
{
    Selector* root = new Selector();
    Sequence* mineSequence = new Sequence();
    //chopSequence -> AddChild(new TravelTask(c,destination,m)); 


    mineSequence->AddChild(new FindIronTask(c, m));
    mineSequence->AddChild(new TravelTask(c, destination, m, false));
    mineSequence->AddChild(new DepositTask(c, m, ObjectType::Ore));
    IdleTask* idle = new IdleTask(c);

    root->AddChild(mineSequence);
    root->AddChild(idle);

    return root;
}



/*
void SetTask(){
    for(int i = 0; i < 5;i++){
        characters[i].type = scout;
	    characters[i].behaviourTree = BuildScoutTree(&characters[i],&map);
    }
    for(int j = 5; j < characters.size()-10;j++){
	    characters[j].behaviourTree = BuildCutTreeTree(&characters[j],{97,4},&map);
    }
    for (int t = characters.size() - 10; t < characters.size() - 5; t++) {
        characters[t].behaviourTree = BuildMineTree(&characters[t], { 97,4 }, &map);
    }
    for (int k = characters.size() - 5; k < characters.size(); k++) {
        characters[k].behaviourTree = BuildUpgradeTree(&characters[k], scout);
    }
}

void RunTask(){
    for(int i = 0; i < characters.size();i++){
        if(characters[i].state!=Idle){
           characters[i].behaviourTree->Run();
        }
	
    }
}
*/
void IncreaseItem(ObjectType item){
    if (item == ObjectType::ChoppedTree) {
        wood++;
    }
    else if (item == ObjectType::Ore) {
        ore++;
    }
    else if (item == ObjectType::Coal) {
        coal++;
    }
    else if (item == ObjectType::IronBar) {
        ironBar++;
    }
    else if (item == ObjectType::Sword) {
        ironBar++;
    }
}
