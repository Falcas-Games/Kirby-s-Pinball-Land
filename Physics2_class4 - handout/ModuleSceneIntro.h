#pragma once
#include "Module.h"
#include "p2List.h"
#include "p2Point.h"
#include "Globals.h"
#include "Animation.h"
#include "ModulePhysics.h"

class PhysBody;
class SDL_Texture;

class ModuleSceneIntro : public Module
{
public:
	ModuleSceneIntro(Application* app, bool start_enabled = true);
	~ModuleSceneIntro();

	bool Start();
	update_status Update();
	bool CleanUp();
	void OnCollision(PhysBody* bodyA, PhysBody* bodyB);

public:
	p2List<PhysBody*> circles;
	p2List<PhysBody*> boxes;
	p2List<PhysBody*> ricks;
	p2List<PhysBody*> walls;
	PhysBody* bumper_left;
	PhysBody* bumper_right;
	PhysBody* ball;
	PhysBody* kicker;
	PhysBody* sensor;
	PhysBody* p_porcupine1;
	PhysBody* p_porcupine2;
	PhysBody* p_casper1;
	PhysBody* p_casper2;
	PhysBody* p_demon;
	SDL_Rect scarfy[7];
	bool sensed;
	bool demon_not_visible;
	int score_demon_not_visible;
	float scarfy_number;

	bool right_bar_check = false;
	bool left_bar_check = false;
	bool left_porcupine_check = false;
	bool right_porcupine_check = false;
	bool demon_check = false;
	bool left_casper_check = false;
	bool right_casper_check = false;


	SDL_Texture* circle;
	SDL_Texture* box;
	SDL_Texture* rick;
	SDL_Texture* spritesheet;
	
	uint bonus_fx;
	p2Point<int> ray;
	bool ray_on;

	Animation porcupine;
	Animation casper;
	Animation demon;
	Animation casper_dead;


	float x_casper = 38; //12
	float y_casper = 326; //304

	float x_casper2 = 133;
	float y_casper2 = 326;


	b2Vec2 vec;

	bool doing_cicle = true;

	char score_text[10];

	// 300		35
	// 326		

private:

};
