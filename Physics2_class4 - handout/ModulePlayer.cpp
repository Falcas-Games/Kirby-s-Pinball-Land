#include "Globals.h"
#include "Application.h"
#include "ModuleInput.h"
#include "ModuleSceneIntro.h"
#include "ModulePlayer.h"
#include "SDL/include/SDL_scancode.h"

ModulePlayer::ModulePlayer(Application* app, bool start_enabled) : Module(app, start_enabled)
{
	level = 4;
	lives = 3;
	score = top_score = last_score = 0;
}

ModulePlayer::~ModulePlayer()
{}

// Load assets
bool ModulePlayer::Start()
{
	LOG("Loading player");
	return true;
}

// Unload assets
bool ModulePlayer::CleanUp()
{
	LOG("Unloading player");

	return true;
}

// Update: draw background
update_status ModulePlayer::Update()
{
	int x, y;
	App->scene_intro->kicker->GetPosition(x, y);
	if (live_losed == false) {
		if (level == 4 && (App->input->GetKey(SDL_SCANCODE_DOWN) == KEY_DOWN || App->input->GetKey(SDL_SCANCODE_DOWN) == KEY_REPEAT)) {
			if (y >= 430)App->scene_intro->kicker->body->SetLinearVelocity({ 0.0f,-10.0f });
			else App->scene_intro->kicker->body->SetLinearVelocity({ 0.0f,0.0f });
		}
		else {
			if (y <= 530)App->scene_intro->kicker->body->SetLinearVelocity({ 0.0f,10.0f });
			else App->scene_intro->kicker->body->SetLinearVelocity({ 0.0f,0.0f });
		}
	}
	else if (App->input->GetKey(SDL_SCANCODE_RETURN) == KEY_DOWN) {
		if (dead == true) {
			last_score = score;
			if (top_score < score)top_score = score;
			score = 0;
		}
		live_losed = dead = false;
	}
	return UPDATE_CONTINUE;
}



