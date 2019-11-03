#include "Globals.h"
#include "Application.h"
#include "ModuleInput.h"
#include "ModuleSceneIntro.h"
#include "ModulePlayer.h"
#include "SDL/include/SDL_scancode.h"

ModulePlayer::ModulePlayer(Application* app, bool start_enabled) : Module(app, start_enabled)
{
	level = 4;
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
	if (level == 4 && (App->input->GetKey(SDL_SCANCODE_UP) == KEY_DOWN || App->input->GetKey(SDL_SCANCODE_UP) == KEY_REPEAT)) {
		if (y >= 430)App->scene_intro->kicker->body->SetLinearVelocity({ 0.0f,-10.0f });
		else App->scene_intro->kicker->body->SetLinearVelocity({ 0.0f,0.0f });
	}
	else {
		if (y <= 530)App->scene_intro->kicker->body->SetLinearVelocity({ 0.0f,10.0f });
		else App->scene_intro->kicker->body->SetLinearVelocity({ 0.0f,0.0f });
	}
	return UPDATE_CONTINUE;
}



