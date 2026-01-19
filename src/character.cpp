#include "character.h"
using namespace std;

int wood = 0;
std::vector<Character> characters;
//Map map;
Map& map = GetMap();
Character::Character(){
    pos = { 50, 50 };
    velocity = { 0, 0 };
    rotation = 0.0f;

    active = true;
    speed = 60.0f;

    state = cutTree;
    type = worker;

    currentItem = ObjectType::None;

    currentPathIndex = 0;

    behaviourTree = nullptr;
    decisionState = nullptr;
}

void Place(){
	//map = GetMap();
    constexpr size_t CHARACTER_COUNT = 50;
    characters.clear();
    characters.reserve(CHARACTER_COUNT);
    for (size_t i = 0; i < CHARACTER_COUNT; ++i) {
        Character c;
        //c.pos = {(float)i * 3 +15, 980.0f};
        c.characterNum = i;
		c.pos = {(float)i * 0.5f +22.5f, 975.0f};
        characters.emplace_back(c);
		//characters[i].RequestPath(map,{10,98});
    }
    //for (size_t j = 0; j < CHARACTER_COUNT/2; ++j) {
    //    Character c;
        //c.pos = {(float)i * 3 +15, 980.0f};
	//	c.pos = {(float)j * 1 +22, 970.0f};
    //    characters.emplace_back(c);
		//characters[i].RequestPath(map,{10,98});
    //}

    cout << "Characters placed: " << characters.size() << '\n';
}

void DrawCharacters(){
    for (const Character& c : characters) {
       //cout << c.pos.x << '\n'; 
        DrawCircle(
            static_cast<int>(c.pos.x),
            static_cast<int>(c.pos.y),
            1,
            BLUE
        );
        //DrawCircle(c.pos.x,c.pos.y,1000,RED);
        //DrawCircle(500,500,1000,RED);
    }
}
void RunCharacters(){
	for(int i = 0; i < characters.size();i++){
        if(characters[i].state!=Idle){
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

void SetTask(){
    for(int i = 0; i < 5;i++){
	characters[i].behaviourTree = BuildScoutTree(&characters[i],&map);
    }
    for(int j = 5; j < characters.size();j++){
	characters[j].behaviourTree = BuildCutTreeTree(&characters[j],{97,4},&map);
    }
}

void RunTask(){
    for(int i = 0; i < characters.size();i++){
        if(characters[i].state!=Idle){
           characters[i].behaviourTree->Run();
        }
	
    }
}

void IncreaseItem(ObjectType item){
    if(item==ObjectType::ChoppedTree){
        wood++;
    }
    
}
