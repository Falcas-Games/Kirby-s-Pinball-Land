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
	circle = box = rick = NULL;
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

	spritesheet = App->textures->Load("pinball/spritesheet.png");

	App->audio->PlayMusic("pinball/music/music.ogg");
	App->audio->LoadFx("pinball/music/bumper.wav");
	App->audio->LoadFx("pinball/music/bar.wav");
	App->audio->LoadFx("pinball/music/porcupine.wav");
	App->audio->LoadFx("pinball/music/demon.wav");

	App->fonts->Load("pinball/fonts.png", "0123456789", 1, 6, 9, 10);
	App->fonts->Load("pinball/fonts2.png", "0123456789", 1, 8, 7, 10);

	App->renderer->camera.x = 0;
	App->renderer->camera.y = (-561 + SCREEN_HEIGHT)*SCREEN_SIZE;

	circle = App->textures->Load("pinball/wheel.png");
	box = App->textures->Load("pinball/crate.png");
	rick = App->textures->Load("pinball/rick_head.png");

	sensor = App->physics->CreateRectangleSensor(SCREEN_WIDTH / 2, SCREEN_HEIGHT, SCREEN_WIDTH, 50);
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


	int spritesheet_1[42] = {
		41, 433,
		3, 424,
		1, 288,
		2, 275,
		60, 293,
		61, 311,
		59, 320,
		57, 304,
		48, 296,
		26, 295,
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
		59, 286
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


	walls.add(App->physics->CreateChain(-2,-2, spritesheet_1, 42, true));
	walls.add(App->physics->CreateChain(-2, -2, spritesheet_2, 44, true));
	walls.add(App->physics->CreateChain(-2, -2, spritesheet_3, 12, true));
	walls.add(App->physics->CreateChain(-2, -2, spritesheet_4, 10, true));
	walls.add(App->physics->CreateChain(-2, -2, spritesheet_5, 8, true));
	walls.add(App->physics->CreateChain(-2, -2, spritesheet_6, 8, true));


	ball=App->physics->CreateCircle(85, 530, 7);
	ball->listener = this;
	ball->body->SetType(b2_dynamicBody);

	p_porcupine1 = App->physics->CreateCircle(32, 320, 7);
	p_porcupine1->listener = this;

	p_porcupine2 = App->physics->CreateCircle(127, 320, 7);
	p_porcupine2->listener = this;

	p_demon = App->physics->CreateCircle(81, 351, 9);
	p_demon->listener = this;

	p_casper1 = App->physics->CreateCircle(46, 332, 8);
	p_casper1->listener = this;
	p_casper1->body->SetType (b2_kinematicBody);

	p_casper2 = App->physics->CreateCircle(141, 332, 8);
	p_casper2->listener = this;
	p_casper2->body->SetType(b2_kinematicBody);



	kicker = App->physics->CreateRectangle(80, 530, 28, 10, true);
	kicker->body->SetType(b2_kinematicBody);
	kicker->listener = this;


	for (int i = 0; i < 7; i++)scarfy[i] = { 164 + (50 * i),363,47,11 };
	scarfy_number = 0;
	return ret;
}

// Load assets
bool ModuleSceneIntro::CleanUp()
{
	LOG("Unloading Intro scene");
	App->fonts->UnLoad(2);
	App->fonts->UnLoad(1);
	App->textures->Unload(circle);
	App->textures->Unload(box);
	App->textures->Unload(rick);
	App->textures->Unload(spritesheet);

	//I DONT KNOW HOW TO DESTROY P2LIST OF PHYSBODIES

	return true;
}

// Update: draw background
update_status ModuleSceneIntro::Update()
{
	if (App->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN)
	{
		ray_on = !ray_on;
		ray.x = App->input->GetMouseX();
		ray.y = App->input->GetMouseY();
	}

	if (App->input->GetKey(SDL_SCANCODE_1) == KEY_DOWN)
	{
		circles.add(App->physics->CreateCircle(App->input->GetMouseX(), App->input->GetMouseY(), 25));
		circles.getLast()->data->listener = this;
	}

	if (App->input->GetKey(SDL_SCANCODE_2) == KEY_DOWN)
	{
		boxes.add(App->physics->CreateRectangle(App->input->GetMouseX(), App->input->GetMouseY(), 100, 50));
	}

	if (App->input->GetKey(SDL_SCANCODE_3) == KEY_DOWN)
	{
		// Pivot 0, 0
		int rick_head[64] = {
			14, 36,
			42, 40,
			40, 0,
			75, 30,
			88, 4,
			94, 39,
			111, 36,
			104, 58,
			107, 62,
			117, 67,
			109, 73,
			110, 85,
			106, 91,
			109, 99,
			103, 104,
			100, 115,
			106, 121,
			103, 125,
			98, 126,
			95, 137,
			83, 147,
			67, 147,
			53, 140,
			46, 132,
			34, 136,
			38, 126,
			23, 123,
			30, 114,
			10, 102,
			29, 90,
			0, 75,
			30, 62
		};

		ricks.add(App->physics->CreateChain(App->input->GetMouseX(), App->input->GetMouseY(), rick_head, 64));
	}

	

	// Prepare for raycast ------------------------------------------------------

	iPoint mouse;
	mouse.x = App->input->GetMouseX();
	mouse.y = App->input->GetMouseY();
	int ray_hit = ray.DistanceTo(mouse);

	fVector normal(0.0f, 0.0f);

	// All draw functions ------------------------------------------------------
	p2List_item<PhysBody*>* c = circles.getFirst();

	while (c != NULL)
	{
		int x, y;
		c->data->GetPosition(x, y);
		if (c->data->Contains(App->input->GetMouseX(), App->input->GetMouseY()))
			App->renderer->Blit(circle, x, y, NULL, 1.0f, c->data->GetRotation());
		c = c->next;
	}

	c = boxes.getFirst();

	while (c != NULL)
	{
		int x, y;
		c->data->GetPosition(x, y);
		App->renderer->Blit(box, x, y, NULL, 1.0f, c->data->GetRotation());
		if (ray_on)
		{
			int hit = c->data->RayCast(ray.x, ray.y, mouse.x, mouse.y, normal.x, normal.y);
			if (hit >= 0)
				ray_hit = hit;
		}
		c = c->next;
	}

	c = ricks.getFirst();

	while (c != NULL)
	{
		int x, y;
		c->data->GetPosition(x, y);
		App->renderer->Blit(rick, x, y, NULL, 1.0f, c->data->GetRotation());
		c = c->next;
	}

	c = walls.getFirst();

	while (c != NULL)
	{
		int x, y;
		c->data->GetPosition(x, y);
		App->renderer->Blit(rick, x, y, NULL, 1.0f, c->data->GetRotation());
		c = c->next;
	}

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
	App->renderer->Blit(spritesheet, 24, 312, &porcupine.GetCurrentFrame());
	App->renderer->Blit(spritesheet, 119, 312, &porcupine.GetCurrentFrame());
	rect = { 272,395,14,14 };
	ball->GetPosition(x, y);
	App->renderer->Blit(spritesheet, x, y, &rect); //Right Bumper
	rect = { 308,407,28,15 };
	kicker->GetPosition(x, y);
	App->renderer->Blit(spritesheet, x, y, &rect);

	///if (App->input->GetKey(SDL_SCANCODE_UP) == KEY_DOWN || App->input->GetKey(SDL_SCANCODE_UP) == KEY_REPEAT)App->renderer->camera.y++;
	//else if (App->input->GetKey(SDL_SCANCODE_DOWN) == KEY_DOWN || App->input->GetKey(SDL_SCANCODE_DOWN) == KEY_REPEAT)App->renderer->camera.y--;
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
			p_casper1->body->SetLinearVelocity({ -0.55f,0.0f });
			p_casper2->body->SetLinearVelocity({ -0.55f,0.0f });
		}
	}
	if (x_casper <= 12) {
		if (y_casper >= 300) {
			y_casper -= 0.5f;
			y_casper2 -= 0.5f;
			p_casper1->body->SetLinearVelocity({ 0.0f,-0.55f });
			p_casper2->body->SetLinearVelocity({ 0.0f,-0.55f });
		}
	}
	if (y_casper <= 300) {
		if (x_casper <= 38) {
			x_casper += 0.5f;
			x_casper2 += 0.5f;
			p_casper1->body->SetLinearVelocity({ 0.55f,0.0f });
			p_casper2->body->SetLinearVelocity({ 0.55f,0.0f });
		}
	}
	if (x_casper >= 38) {
		if (y_casper <= 326) {
			doing_cicle = false;
			y_casper += 0.5f;
			y_casper2 += 0.5f;
			p_casper1->body->SetLinearVelocity({ 0.0f,0.55f });
			p_casper2->body->SetLinearVelocity({ 0.0f,0.55f });
		}
		else {
			doing_cicle = true;
		}
	}
	App->renderer->Blit(spritesheet, x_casper, y_casper, &casper.GetCurrentFrame());
	App->renderer->Blit(spritesheet, x_casper2, y_casper2, &casper.GetCurrentFrame());
	App->renderer->Blit(spritesheet, 71, 342, &demon.GetCurrentFrame());
	

	if (App->input->GetKey(SDL_SCANCODE_LEFT) == KEY_DOWN)
		App->audio->PlayFx(1);
	if (App->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_DOWN)
		App->audio->PlayFx(1);
	ball->GetPosition(x,y);
	if (y >= 424 && App->player->level != 4) {
		App->renderer->camera.y -= SCREEN_HEIGHT*SCREEN_SIZE;
		if (App->player->level == 3) {
			App->player->lives--;
			App->player->live_losed = true;
			scarfy_number = 0;
		}
		App->player->level++;
		if (App->player->lives == 0)App->player->dead = true;
	}
	else if (y <= 424 && App->player->level == 4) {
		App->renderer->camera.y += SCREEN_HEIGHT * SCREEN_SIZE;
		App->player->level--;
	}

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
	int j = 0;
	if (App->player->level == 3) {
		App->renderer->Blit(spritesheet, 56, 332, &scarfy[(int)scarfy_number]);
		if (App->player->top_score < App->player->score)App->player->top_score = App->player->score;
		sprintf_s(score_text, 10, "%7d", App->player->score);
		App->fonts->BlitText(0, 417, 2, score_text, 0);
		sprintf_s(score_text, 10, "%7d", App->player->last_score);
		for (int i = App->player->last_score; i > 10; i *= 0.1)j++;
		App->fonts->BlitText(152-(8*j), 417, 2, score_text, 0);
		sprintf_s(score_text, 10, "%7d", App->player->top_score);
		j = 0;
		for (int i = App->player->top_score; i > 10; i *= 0.1)j++;
		App->fonts->BlitText(152 - (8 * j), 287, 2, score_text, 0);
		rect = { 348,323,11,11 };
		for (int i = App->player->lives; i > 0; i--) App->renderer->Blit(spritesheet, (i - 1) * 11, 287, &rect);
	}
	else if (App->player->level == 4) {
		if (App->player->top_score < App->player->score)App->player->top_score = App->player->score;
		sprintf_s(score_text, 10, "%7d", App->player->score);
		App->fonts->BlitText(0, 554, 2, score_text, 0);
		sprintf_s(score_text, 10, "%7d", App->player->last_score);
		for (int i = App->player->last_score; i > 10; i *= 0.1)j++;
		App->fonts->BlitText(152-(8*j), 554, 2, score_text, 0);
		sprintf_s(score_text, 10, "%7d", App->player->top_score);
		j = 0;
		for (int i = App->player->top_score; i > 10; i *= 0.1)j++;
		App->fonts->BlitText(152-(8*j), 424, 2, score_text, 0);
		rect = { 348,323,11,11 };
		for (int i = App->player->lives; i > 0; i--) App->renderer->Blit(spritesheet, (i - 1) * 11, 424, &rect);
	}
	return UPDATE_CONTINUE;
}

void ModuleSceneIntro::OnCollision(PhysBody* bodyA, PhysBody* bodyB)
{
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

	if (bodyA == p_porcupine1 || bodyB == p_porcupine1) {
		left_porcupine_check = true;
		App->player->score += 240;
		App->audio->PlayFx(3);
	}
	else left_porcupine_check = false;

	if (bodyA == p_porcupine2 || bodyB == p_porcupine2) {
		right_porcupine_check = true;
		App->player->score += 240;
		App->audio->PlayFx(3);
	}
	else right_porcupine_check = false;

	if (bodyA == p_demon || bodyB == p_demon) {
		demon_check = true;
		App->player->score += 480;
		if (scarfy_number <= 6)scarfy_number+=0.5;
		else scarfy_number = 0;
		App->audio->PlayFx(4);
	}
	else demon_check = false;


	/*
	if(bodyA)
	{
		bodyA->GetPosition(x, y);
		App->renderer->DrawCircle(x, y, 50, 100, 100, 100);
	}

	if(bodyB)
	{
		bodyB->GetPosition(x, y);
		App->renderer->DrawCircle(x, y, 50, 100, 100, 100);
	}*/
}
