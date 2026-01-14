#include "character.h"
using namespace std;


std::vector<Character> characters;
//Map map;
Map& map = GetMap();
Character::Character()
    : pos{0.0f, 0.0f},
      velocity{0.0f, 0.0f},
      rotation(5.0f),
      active(false),
      wanderTarget{12.5f, 12.5f},
      collisionTimer(0),
      collisionVector{0.0f, 0.0f},
      previousPath(-1),
      type(worker)
{
}

void Place(){
	//map = GetMap();
    constexpr size_t CHARACTER_COUNT = 50;
    characters.clear();
    characters.reserve(CHARACTER_COUNT);
    for (size_t i = 0; i < CHARACTER_COUNT; ++i) {
        Character c;
        //c.pos = {(float)i * 3 +15, 980.0f};
		c.pos = {(float)i * 3 +10, 900.0f};
        characters.emplace_back(c);
		characters[i].RequestPath(map,{10,98});
    }

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
		characters[i].Update(GetFrameTime());
	}
}




void Character::RequestPath(Map& map, Pair target)
{

    Pair start = { (int)(pos.y / 10.0f), (int)(pos.x / 10.0f) }; 
    Pair goal  = target; 

    if (map.AStarSearch(start, goal, path)) {
        currentNode = 0;
    }
}

void Character::Update(float dt)
{
    if (currentNode >= path.size())
        return;


    Vector2 target = { path[currentNode].second * 10.0f + 5, path[currentNode].first * 10.0f + 5};
    Vector2 dir = Vector2Subtract(target, pos);
    float dist = Vector2Length(dir);
    if (dist < 2.0f) {
        currentNode++;
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
	chopSequence -> AddChild(new TravelTask(c,destination,m)); 
	chopSequence -> AddChild(new ChopTask(c,m)); 
	//Task* work = BuildFactoryTree(c);

	root->AddChild(chopSequence);
	//root->AddChild(work);

	return root;
}


void SetTask(){
	characters[0].behaviourTree = BuildCutTreeTree(&characters[0],{54,27},&map);
}

void RunTask(){
	characters[0].behaviourTree->Run();
}

