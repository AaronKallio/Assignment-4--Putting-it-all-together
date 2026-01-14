#include "character.h"
#include <iostream>
#include "raymath.h" 
using namespace std;


std::vector<Character> characters;
Map map;

Character::Character()
    : pos{0.0f, 0.0f},
      velocity{0.0f, 0.0f},
      state(1),
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
	map = GetMap();
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


	//speed = 1; // character stat
    Node& tile = map.GetNodeAtWorld(pos);

    float finalSpeed = 1 / tile.moveCost;
    dir = Vector2Normalize(dir);
    pos += dir * finalSpeed * dt;
}








/*
std::vector<Character*> characters;

Character::Character() {

	this->pos = {0,0};
	this->rotation = 5;
	this->state = 1;
	this->active = false;
	this->velocity = {0,0};
	this->wanderTarget = {12.5f,12.5f};
	this->collisionTimer = 0;
	this->collisionVector = {0,0};
	this->previousPath = -1;
    this->type = worker;
	
}

Character::~Character() {}


void Place() {  //intial data for the enemies
	for (int i = 0; i < 50; i++) {
        characters.push_back(new Character());
		characters[i]->pos = { 50.0f * i + 15, 980 };
		//Vector2 temp = {  };
		//temp.Normalize();
		//characters[i]->velocity = temp;
        cout<<characters.size();
	}
}

void DrawCharacters() {  //intial data for the enemies
	for (int i = 0; i < characters.size(); i++) {
        DrawPixel(characters[i]->pos.x,characters[i]->pos.y,RED);
		//Vector2 temp = {  };
		//temp.Normalize();
		//characters[i]->velocity = temp;
	}
}
*/

/*
Point2D AvoidWall(Point2D enemyPos, Vector2D velocity, float time){
	for (int i = 0; i < walls.size(); i++) //checks all walls for collision
	{
			Vector2D raycast = velocity;
			raycast.Normalize();
			Vector2D normal = Collide(enemyPos, enemyPos + raycast, walls[i][0], walls[i][1]);
			if (normal != Vector2D{0,0}) {
				Point2D hit = HitPoint(walls[i][0], walls[i][1], enemyPos, enemyPos + raycast);
				Vector2D temp = hit - enemyPos;
				int distance = temp.Length();
				if (distance <  50) { //if within 50 pixels, then avoid wall
					float approachAngle = acos(dot(normalize(velocity), normal));
					float crossZ = velocity.x * normal.y - velocity.y * normal.x;
					float sign = (crossZ < 0) ? -1.0f : 1.0f; //used to flip direction of avoid
					float angleOffset = 60 * 0.2f * sign; 
					Vector2D adjustedNormal = RotateVector(normal, angleOffset);
					Point2D newPos;
					newPos.x = hit.x + adjustedNormal.x * 50;
					newPos.y = hit.y + adjustedNormal.y * 50;
					return newPos;
				}
			}
	}
	return {0,0}; //returns no vector if no collision
}
Vector2D RotateVector(Vector2D v, float angleRadians) {
	float cosA = cos(angleRadians);
	float sinA = sin(angleRadians);
	return { v.x * cosA - v.y * sinA, v.x * sinA + v.y * cosA };
}
Point2D HitPoint(Point2D wallStart, Point2D wallEnd, Point2D rayStart, Point2D rayEnd) { //calculates where the raycast hits the wall
	Vector2D r = rayEnd - rayStart;
	Vector2D d = wallEnd - wallStart;
	Vector2D diff = wallStart - rayStart;
	float denom = Cross(r, d);
	if (fabs(denom) < 1e-8f) {
		return rayStart; 
	}
	float t = Cross(diff, d) / denom;
	Point2D test = rayStart + r * t;
	return rayStart + r * t; 
}

Vector2D Collide(Point2D rayStart, Point2D rayEnd, Point2D wallStart, Point2D wallEnd) //calculates the normal of the raycast and the wall
{
	Vector2D r = rayEnd - rayStart;      
	Vector2D d = wallEnd - wallStart;    
	Vector2D diff = wallStart - rayStart;
	float denom = Cross(r, d);
	if (fabs(denom) < 1e-8f) {
		return { 0,0 };
	}

	float t = Cross(diff, d) / denom;
	float u = Cross(diff, r) / denom;

	if (t < 0) {
		return { 0,0 };
	}

	if (u < 0 || u > 1) {
		return { 0,0 };
	}

	Vector2D normal = { d.y, -d.x };
	normal.Normalize();
	if (dot(r, normal) > 0) {
		normal = -normal;
	}
	return normal;
}

float Cross(Vector2D vec1, Vector2D vec2) { //cross product
	return vec1.x * vec2.y - vec1.y * vec2.x;
}
Vector2D Seek(Point2D marker, Point2D enemyPos, Vector2D velocity, float time, bool seek) {
	float maxSpeed = 100;
	Vector2D desired;
	if (seek) { //selects between seek or flee
		desired = (marker - enemyPos);
	}
	else {
		desired = (enemyPos - marker);
	}

	desired.Normalize();
	desired *= maxSpeed;
	Play::DrawLine(enemyPos, enemyPos + desired, Play::cYellow);
	Vector2D steering = (desired - velocity);
	velocity += steering * time;

	if (velocity.Length() > maxSpeed) {
		velocity.Normalize();
		velocity *= maxSpeed;  //limits speed
	}
	Play::DrawLine(enemyPos, enemyPos + velocity, Play::cBlue);
	return velocity;
}

Vector2D Arrive(Point2D marker, Point2D pos, Vector2D velocity, float time) {
	float targetRadius = 10.0f;
	float slowRadius = 100.0f;
	float maxSpeed = 100.0f;
	float maxAccel = 100.0f;
	Vector2D direction = marker - pos;
	float distance = direction.Length();

	//stop when close enough to the marker and maintain same vector for drawing the sprite the correct way
	if (distance < targetRadius) {
		return {};
	}
	float targetSpeed = 0.0f;
	if (distance > slowRadius) {
		targetSpeed = maxSpeed; //max speed when out of range
	}
	else {
		targetSpeed = maxSpeed * (distance / slowRadius); //slow down more as approaching the marker
	}

	Vector2D desired = direction;
	desired.Normalize();
	desired *= targetSpeed;
	Play::DrawLine(pos, pos + desired, Play::cYellow);
	Vector2D steering = (desired - velocity); 

	//does not exceed max accel
	if (steering.Length() > maxAccel) {
		steering.Normalize();
		steering *= maxAccel;
	}

	velocity += steering * time;
	if (velocity.Length() > maxSpeed) {
		velocity.Normalize();
		velocity *= maxSpeed;
	}

	Play::DrawLine(pos, pos + velocity, Play::cBlue);
	return velocity;
}

void ArriveUpdate(Point2D marker, int i, float time) { //unique update function as i have implemented the stop with the the same angle which breaks with UpdateEnemies
	if (enemies[i]->active) {
		int enemyID = Play::GetSpriteId("pointer");
		Vector2D vec = Arrive(marker, enemies[i]->pos, enemies[i]->velocity, time);
		if (vec == Vector2D{}) {
			Play::DrawSpriteRotated(enemyID, enemies[i]->pos, 0, atan2(-enemies[i]->velocity.x, enemies[i]->velocity.y), 1, 1, Play::cWhite);
			enemies[i]->collisionTimer = 0;
			return;
		}
		else {
			Point2D collision = AvoidWall(enemies[i]->pos, enemies[i]->velocity, time);
			
			if (collision.Length() > 0.001f) {
				enemies[i]->velocity = Seek(collision, enemies[i]->pos, enemies[i]->velocity, time, true);
				enemies[i]->collisionTimer = 20; //does wall avoid for 20 frames and then check again
				enemies[i]->collisionVector = collision;
			}
			else if (enemies[i]->collisionTimer > 0) {
				enemies[i]->velocity = Seek(enemies[i]->collisionVector, enemies[i]->pos, enemies[i]->velocity, time, true);
				enemies[i]->collisionTimer--;
			}
			else if (enemies[i]->collisionTimer <= 0) {
				enemies[i]->velocity = vec;
			}
			enemies[i]->pos += enemies[i]->velocity * time;
			enemies[i]->pos = AvoidBorder(enemies[i]->pos);
		}
		Play::DrawSpriteRotated(enemyID, enemies[i]->pos, 0, atan2(-enemies[i]->velocity.x, enemies[i]->velocity.y), 1, 1, Play::cWhite);
		}
	}

*/