#include "Globals.h"
#include "Application.h"
#include "ModuleRender.h"
#include "ModuleSceneIntro.h"
#include "ModuleInput.h"
#include "ModuleTextures.h"
#include "ModuleAudio.h"
#include "ModulePlayer.h"
#include "ModulePhysics.h"
#include "ModuleFonts.h"

ModuleSceneIntro::ModuleSceneIntro(Application* app, bool start_enabled) : Module(app, start_enabled)
{
	ray_on = false;
	sensed = false;
}

ModuleSceneIntro::~ModuleSceneIntro()
{}

// Load assets
bool ModuleSceneIntro::Start()
{
	LOG("Loading Intro assets");
	bool ret = true;

	//Spritesheet loading
	spritesheet = App->textures->Load("pinball/spritesheet.png");

	//fx loading
	App->audio->PlayMusic("pinball/music/music.ogg");
	App->audio->LoadFx("pinball/music/bumper.wav");
	App->audio->LoadFx("pinball/music/bar.wav");
	App->audio->LoadFx("pinball/music/porcupine.wav");
	App->audio->LoadFx("pinball/music/demon.wav");
	App->audio->LoadFx("pinball/music/ghost.wav");

	//fonts loading
	App->fonts->Load("pinball/fonts.png", "0123456789", 1, 6, 9, 10);
	App->fonts->Load("pinball/fonts2.png", "0123456789", 1, 8, 7, 10);

	//set camera position
	App->renderer->camera.x = 0;
	App->renderer->camera.y = (-561 + SCREEN_HEIGHT)*SCREEN_SIZE;


	sensor = App->physics->CreateRectangleSensor(SCREEN_WIDTH / 2, SCREEN_HEIGHT, SCREEN_WIDTH, 50);

	//ANIMATIONS
	float speed = 0.06f;
	porcupine.PushBack({ 165,304,16,16 },speed);
	porcupine.PushBack({ 184,304,16,16 },speed);
	porcupine.loop = true;

	speed = 0.04f;
	casper.PushBack({ 167,323,15,15 },speed);
	casper.PushBack({ 167,323,16,15 },speed);
	casper.PushBack({ 204,323,16,15 },speed);
	casper.PushBack({ 274,323,14,15 },speed);
	casper.PushBack({ 291,323,14,15 },speed);
	casper.PushBack({ 204,323,16,15 }, speed);
	casper.loop = true;

	speed = 0.1f;
	demon.PushBack({ 165,344,20,15 },speed);
	demon.PushBack({ 187,343,20,16 },speed);
	demon.PushBack({ 208,341,20,19 },speed);
	demon.loop = true;

	casper_dead.PushBack({ 325,323,14,15 },speed);
	casper_dead.loop = true;

	//limits of the map
	int spritesheet_1[40] = {
		41, 433,
		3, 424,
		1, 288,
		2, 275,
		102, 295,
		63, 297,
		60, 320,
		56, 302,
		47, 296,
		18, 295,
		12, 301,
		8, 316,
		9, 339,
		23, 352,
		23, 361,
		19, 365,
		12, 375,
		13, 390,
		12, 412,
		39, 430
	};
	int spritesheet_2[44] = {
		59, 293,
		102, 295,
		101, 311,
		105, 320,
		107, 305,
		111, 298,
		120, 295,
		146, 295,
		153, 301,
		156, 310,
		156, 334,
		150, 344,
		140, 353,
		140, 360,
		148, 370,
		152, 379,
		153, 410,
		144, 420,
		125, 438,
		161, 425,
		160, 284,
	};
	int spritesheet_3[12] = {
		28, 383,
		29, 402,
		49, 412,
		51, 409,
		30, 399,
		28, 384
	};
	int spritesheet_4[10] = {
		114, 410,
		135, 398,
		136, 382,
		136, 401,
		115, 412
	};
	int spritesheet_5[8] = {
		44, 383,
		49, 391,
		46, 390,
		44, 384
	};
	int spritesheet_6[8] = {
		120, 382,
		115, 390,
		116, 391,
		121, 383
	};

	//puting it in the list
	walls.add(App->physics->CreateChain(-2,-2, spritesheet_1, 40, true));
	walls.add(App->physics->CreateChain(-2, -2, spritesheet_2, 44, true));
	walls.add(App->physics->CreateChain(-2, -2, spritesheet_3, 12, true));
	walls.add(App->physics->CreateChain(-2, -2, spritesheet_4, 10, true));
	walls.add(App->physics->CreateChain(-2, -2, spritesheet_5, 8, true));
	walls.add(App->physics->CreateChain(-2, -2, spritesheet_6, 8, true));

	//creating the ball
	ball=App->physics->CreateCircle(85, 530, 7);
	ball->listener = this;
	ball->body->SetType(b2_dynamicBody);

	//creating the physbody of the porcupine
	p_porcupine1 = App->physics->CreateCircle(32, 320, 7);
	p_porcupine1->listener = this;

	//creating the physbody of the porcupine
	p_porcupine2 = App->physics->CreateCircle(127, 320, 7);
	p_porcupine2->listener = this;

	//set demon not visible
	demon_not_visible = true;
	score_demon_not_visible = 0;
	p_demon = NULL;

	//creating the physbody of the casper
	p_casper1 = App->physics->CreateCircle(23, 332, 8);
	p_casper1->listener = this;
	p_casper1->body->SetType (b2_kinematicBody);

	//creating the physbody of the casper
	p_casper2 = App->physics->CreateCircle(118, 332, 8);
	p_casper2->listener = this;
	p_casper2->body->SetType(b2_kinematicBody);


	//creating the physbody of the kicker
	kicker = App->physics->CreateRectangle(80, 530, 28, 10, true);
	kicker->body->SetType(b2_kinematicBody);
	kicker->listener = this;

	//letters we use to combo
	for (int i = 0; i < 7; i++)scarfy[i] = { 164 + (50 * i),363,47,11 };
	scarfy_number = 0;

	App->physics->CreateCircle(SCREEN_WIDTH / 2 - 1, 295, 1);

	return ret;
}

// Load assets
bool ModuleSceneIntro::CleanUp()
{
	LOG("Unloading Intro scene");
	App->fonts->UnLoad(2);
	App->fonts->UnLoad(1);
	
	App->textures->Unload(spritesheet);

	//I DONT KNOW HOW TO DESTROY P2LIST OF PHYSBODIES

	return true;
}

// Update: draw background
update_status ModuleSceneIntro::Update()
{
	//control visibility of demon
	if (demon_not_visible == true)p_demon = NULL;
	if (demon_not_visible == false && p_demon == NULL) {
		p_demon = App->physics->CreateCircle(81, 351, 9);
		p_demon->listener = this;
	}
	if (App->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN)
	{
		ray_on = !ray_on;
		ray.x = App->input->GetMouseX();
		ray.y = App->input->GetMouseY();
	}

	// Prepare for raycast ------------------------------------------------------

	iPoint mouse;
	mouse.x = App->input->GetMouseX();
	mouse.y = App->input->GetMouseY();
	int ray_hit = ray.DistanceTo(mouse);

	fVector normal(0.0f, 0.0f);

	// All draw functions ------------------------------------------------------
	p2List_item<PhysBody*>* c = walls.getFirst();

	// ray -----------------
	if (ray_on == true)
	{
		fVector destination(mouse.x - ray.x, mouse.y - ray.y);
		destination.Normalize();
		destination *= ray_hit;

		App->renderer->DrawLine(ray.x, ray.y, ray.x + destination.x, ray.y + destination.y, 255, 255, 255);

		if (normal.x != 0.0f)
			App->renderer->DrawLine(ray.x + destination.x, ray.y + destination.y, ray.x + destination.x + normal.x * 25.0f, ray.y + destination.y + normal.y * 25.0f, 100, 255, 100);
	}

	//RENDER MAP
	SDL_Rect rect;
	int x, y;
	rect = { 0,0,160,564 };
	App->renderer->DrawQuad(rect, 255, 255, 255, 255); //White Screen Behind
	rect = { 2,2,160,424 };
	App->renderer->Blit(spritesheet, 0, 0, &rect); //Map Rect
	rect = { 273,410,22,12 };
	App->renderer->Blit(spritesheet, 89, 406,&rect,1.0f,bumper_right->GetRotation(),86,18); //Right Bumper
	rect = { 233,410,22,12 };
	App->renderer->Blit(spritesheet, 49, 406, &rect, 1.0f, bumper_left->GetRotation(),22,18); //Left Bumper
	App->renderer->Blit(spritesheet, 24, 312, &porcupine.GetCurrentFrame()); //Porcupine draw
	App->renderer->Blit(spritesheet, 119, 312, &porcupine.GetCurrentFrame()); //porcupine 2 draw
	rect = { 308,407,28,15 };
	kicker->GetPosition(x, y);
	App->renderer->Blit(spritesheet, x, y, &rect); //kicker draw

	//BUMPERS MOVEMENT
	if (App->input->GetKey(SDL_SCANCODE_LEFT) == KEY_DOWN || App->input->GetKey(SDL_SCANCODE_LEFT) == KEY_REPEAT)
	{
		App->physics->MoveBumper(1, true);
	}
	else App->physics->MoveBumper(1, false);

	if (App->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_DOWN || App->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_REPEAT) {
		App->physics->MoveBumper(2, true);
	}
	else App->physics->MoveBumper(2, false);

	//movement casper
	if (x_casper >= 12 && doing_cicle) {
		if (y_casper >= 300) {
			x_casper-=0.5f;
			x_casper2 -= 0.5f;
		}
	}
	if (x_casper <= 12) {
		if (y_casper >= 300) {
			y_casper -= 0.5f;
			y_casper2 -= 0.5f;
		}
	}
	if (y_casper <= 300) {
		if (x_casper <= 38) {
			x_casper += 0.5f;
			x_casper2 += 0.5f;
		}
	}
	if (x_casper >= 38) {
		if (y_casper <= 326) {
			doing_cicle = false;
			y_casper += 0.5f;
			y_casper2 += 0.5f;
		}
		else {
			doing_cicle = true;
		}
	}
	if (x_casper == 11.5 && y_casper < 326.5) {
		p_casper1->body->SetLinearVelocity({ 0.0f,-0.55f });
		p_casper2->body->SetLinearVelocity({ 0.0f,-0.55f });
	}
	else if (x_casper == 11.5 && y_casper == 326.5) {
		p_casper1->body->SetLinearVelocity({ -0.5f,-0.5f });
		p_casper2->body->SetLinearVelocity({ -0.5f,-0.5f });
	}
	else if (x_casper == 38.5 && y_casper > 299.5) {
		p_casper1->body->SetLinearVelocity({ 0.0f,0.55f });
		p_casper2->body->SetLinearVelocity({ 0.0f,0.55f });
	}
	else if (x_casper == 38.5 && y_casper == 299.5) {
		p_casper1->body->SetLinearVelocity({ 0.5f,0.5f });
		p_casper2->body->SetLinearVelocity({ 0.5f,0.5f });
	}
	else if (y_casper == 326.5 && x_casper < 38.5) {
		p_casper1->body->SetLinearVelocity({ -0.55f,0.0f });
		p_casper2->body->SetLinearVelocity({ -0.55f,0.0f });
	}
	else if (y_casper == 326.5 && x_casper == 38.5) {
		p_casper1->body->SetLinearVelocity({ -0.5f,-0.5f });
		p_casper2->body->SetLinearVelocity({ -0.5f,-0.5f });
	}
	else if (y_casper == 299.5 && x_casper > 11.5) {
		p_casper1->body->SetLinearVelocity({ 0.55f,0.0f });
		p_casper2->body->SetLinearVelocity({ 0.55f,0.0f });
	}
	else if (y_casper == 299.5 && x_casper == 11.5) {
		p_casper1->body->SetLinearVelocity({ 0.5f,0.5f });
		p_casper2->body->SetLinearVelocity({ 0.5f,0.5f });
	}

	if(demon_not_visible==false) 
		App->renderer->Blit(spritesheet, 71, 342, &demon.GetCurrentFrame());
	
	//fx bumpers
	if (App->input->GetKey(SDL_SCANCODE_LEFT) == KEY_DOWN)
		App->audio->PlayFx(1);
	if (App->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_DOWN)
		App->audio->PlayFx(1);

	ball->GetPosition(x,y);

	//control of the letters to get the lives
	if (y >= 424 && App->player->level != 4) {
		App->renderer->camera.y -= SCREEN_HEIGHT*SCREEN_SIZE;
		if (App->player->level == 3) {
			App->player->lives--;
			App->player->live_losed = true;
			scarfy_number = 0;
			if (p_demon != NULL)App->physics->DestroyBody(p_demon);
			score_demon_not_visible = App->player->score;
			demon_not_visible = true;
		}
		App->player->level++;
		if (App->player->lives == 0)App->player->dead = true;
	}
	else if (y <= 424 && App->player->level == 4) {
		App->renderer->camera.y += SCREEN_HEIGHT * SCREEN_SIZE;
		App->player->level--;
	}

	//printing the score when colide each object
	if (left_bar_check == true) {
		rect = { 369,409,9,15 };
		App->renderer->Blit(spritesheet, 39, 378, &rect);
		App->fonts->BlitText(46, 383, 1, "50", 1);
	}
	if (right_bar_check == true) {
		rect = { 378,414,9,15 };
		App->renderer->Blit(spritesheet, 112, 378, &rect);
		App->fonts->BlitText(120, 382, 1, "50", 1);
	}

	if (left_porcupine_check == true)
		App->fonts->BlitText(37, 320, 1, "240", 1);

	if (right_porcupine_check == true)
		App->fonts->BlitText(134, 320, 1, "240", 1);

	if(demon_check==true)
		App->fonts->BlitText(92, 351, 1, "480", 1);

	if (left_casper_check == true) {
		App->fonts->BlitText(x_casper + 5, y_casper + 15, 1, "360", 1);
		App->renderer->Blit(spritesheet, x_casper, y_casper, &casper_dead.GetCurrentFrame());
	}
	else App->renderer->Blit(spritesheet, x_casper, y_casper, &casper.GetCurrentFrame());

	if (right_casper_check == true) {
		App->fonts->BlitText(x_casper2 + 5, y_casper2 + 15, 1, "360", 1);
		App->renderer->Blit(spritesheet, x_casper2, y_casper2, &casper_dead.GetCurrentFrame());
	}
	else App->renderer->Blit(spritesheet, x_casper2, y_casper2, &casper.GetCurrentFrame());

	//SCORE MANAGEMENT
	int j = 0;
	if (App->player->level == 3) {
		App->renderer->Blit(spritesheet, 56, 332, &scarfy[(int)scarfy_number]);

		if (App->player->top_score < App->player->score)
			App->player->top_score = App->player->score;

		sprintf_s(score_text, 10, "%7d", App->player->score);
		App->fonts->BlitText(0, 417, 2, score_text, 0);
		sprintf_s(score_text, 10, "%7d", App->player->last_score);

		for (int i = App->player->last_score; i > 10; i *= 0.1)
			j++;

		App->fonts->BlitText(152-(8*j), 417, 2, score_text, 0);
		sprintf_s(score_text, 10, "%7d", App->player->top_score);
		j = 0;

		for (int i = App->player->top_score; i > 10; i *= 0.1)
			j++;

		App->fonts->BlitText(152 - (8 * j), 287, 2, score_text, 0);
		rect = { 348,323,11,11 };

		for (int i = App->player->lives; i > 0; i--) 
			App->renderer->Blit(spritesheet, (i - 1) * 11, 287, &rect);
	}
	else if (App->player->level == 4) {

		if (App->player->top_score < App->player->score)
			App->player->top_score = App->player->score;

		sprintf_s(score_text, 10, "%7d", App->player->score);
		App->fonts->BlitText(0, 554, 2, score_text, 0);
		sprintf_s(score_text, 10, "%7d", App->player->last_score);

		for (int i = App->player->last_score; i > 10; i *= 0.1)
			j++;

		App->fonts->BlitText(152-(8*j), 554, 2, score_text, 0);
		sprintf_s(score_text, 10, "%7d", App->player->top_score);

		j = 0;

		for (int i = App->player->top_score; i > 10; i *= 0.1)
			j++;

		App->fonts->BlitText(152-(8*j), 424, 2, score_text, 0);
		rect = { 348,323,11,11 };

		for (int i = App->player->lives; i > 0; i--) 
			App->renderer->Blit(spritesheet, (i - 1) * 11, 424, &rect);
	}
	if (demon_not_visible == true && score_demon_not_visible + 1000 <= App->player->score)
		demon_not_visible = false;

	//What happens if player is dead
	if (App->player->dead == true) {
		sprintf_s(score_text, 10, "%7d", App->player->last_score);
		if (App->player->last_score == 0)
			App->fonts->BlitText(SCREEN_WIDTH / 2 - 4, 558 - SCREEN_HEIGHT / 2 - 50, 1, score_text, 1);
		else if (App->player->last_score < 1000)
			App->fonts->BlitText(SCREEN_WIDTH / 2 - 14, 558 - SCREEN_HEIGHT / 2 - 50, 1, score_text, 1);
		else if (App->player->last_score < 10000)
			App->fonts->BlitText(SCREEN_WIDTH / 2 - 18, 558 - SCREEN_HEIGHT / 2 - 50, 1, score_text, 1);
		else
			App->fonts->BlitText(SCREEN_WIDTH / 2 - 22, 558 - SCREEN_HEIGHT / 2 - 50, 1, score_text, 1);

		sprintf_s(score_text, 10, "%7d", App->player->score);

		if (App->player->score == 0)
			App->fonts->BlitText(SCREEN_WIDTH / 2 - 4, 558 - SCREEN_HEIGHT / 2 - 20, 1, score_text, 1);
		else if (App->player->score < 1000)
			App->fonts->BlitText(SCREEN_WIDTH / 2 - 14, 558 - SCREEN_HEIGHT / 2 - 20, 1, score_text, 1);
		else if (App->player->score < 10000)
			App->fonts->BlitText(SCREEN_WIDTH / 2 - 18, 558 - SCREEN_HEIGHT / 2 - 20, 1, score_text, 1);
		else
			App->fonts->BlitText(SCREEN_WIDTH / 2 - 22, 558 - SCREEN_HEIGHT / 2 - 20, 1, score_text, 1);

		sprintf_s(score_text, 10, "%7d", App->player->top_score);

		if (App->player->top_score == 0)
			App->fonts->BlitText(SCREEN_WIDTH / 2 - 4, 558 - SCREEN_HEIGHT / 2 + 10, 1, score_text, 1);
		else if (App->player->top_score < 1000)
			App->fonts->BlitText(SCREEN_WIDTH / 2 - 14, 558 - SCREEN_HEIGHT / 2 + 10, 1, score_text, 1);
		else if (App->player->top_score < 10000)
			App->fonts->BlitText(SCREEN_WIDTH / 2 - 18, 558 - SCREEN_HEIGHT / 2 + 10, 1, score_text, 1);
		else
			App->fonts->BlitText(SCREEN_WIDTH / 2 - 22, 558 - SCREEN_HEIGHT / 2 + 10, 1, score_text, 1);
	}

	rect = { 272,395,14,14 };
	ball->GetPosition(x, y);
	App->renderer->Blit(spritesheet, x, y, &rect); //Right Bumper
	return UPDATE_CONTINUE;
}

void ModuleSceneIntro::OnCollision(PhysBody* bodyA, PhysBody* bodyB)
{
	//all collisions management
	int x, y;
	p2List_item<PhysBody*>* item = walls.getFirst();
	for (int i = 0; i < 4; i++) {
		item = item->next;
	}
	if (bodyA == item->data || bodyB == item->data) { // if the object is the left bar
		App->audio->PlayFx(2);
		App->player->score += 50;
		left_bar_check = true;
	}
	else left_bar_check = false;
	item = item->next;

	if (bodyA == item->data || bodyB == item->data) { //if the object is the right bar
		App->audio->PlayFx(2);
		App->player->score += 50;
		right_bar_check = true;
	}
	else right_bar_check = false;

	if (bodyA == p_porcupine1 || bodyB == p_porcupine1) { //if the object is the left porcupine
		left_porcupine_check = true;
		App->player->score += 240;
		App->audio->PlayFx(3);
	}
	else left_porcupine_check = false;

	if (bodyA == p_porcupine2 || bodyB == p_porcupine2) { //if the object is the right porcupine
		right_porcupine_check = true;
		App->player->score += 240;
		App->audio->PlayFx(3);
	}
	else right_porcupine_check = false;

	if ((bodyA == p_demon || bodyB == p_demon)&&p_demon!=NULL) { //all logic when ball collides to demon
		demon_check = true;
		App->player->score += 480;
		if (scarfy_number <= 6)scarfy_number+=0.5;
		else {
			scarfy_number = 0;
			App->player->lives++;
			if(p_demon!=NULL)App->physics->DestroyBody(p_demon);
			demon_not_visible = true;
			score_demon_not_visible = App->player->score;
		}
		App->audio->PlayFx(4);
	}
	else demon_check = false;

	if (bodyA == p_casper1 || bodyB == p_casper1) { //if the object is the left casper
		left_casper_check = true;
		App->player->score += 360;
		App->audio->PlayFx(4);
	}
	else left_casper_check = false;

	if (bodyA == p_casper2 || bodyB == p_casper2) { //if the object is the right casper
		right_casper_check = true;
		App->player->score += 360;
		App->audio->PlayFx(4);
	}
	else right_casper_check = false;

}
