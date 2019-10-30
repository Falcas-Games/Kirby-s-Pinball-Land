#include "Globals.h"
#include "Application.h"
#include "ModuleRender.h"
#include "ModuleSceneIntro.h"
#include "ModuleInput.h"
#include "ModuleTextures.h"
#include "ModuleAudio.h"
#include "ModulePhysics.h"
#include "ModuleStartScene.h"

ModuleStartScene::ModuleStartScene(Application* app, bool start_enabled) : Module(app, start_enabled)
{

}

bool ModuleStartScene::Start() {


	return true;
}

update_status ModuleStartScene::Update() {
	

	return update_status::UPDATE_CONTINUE;
}

bool ModuleStartScene::CleanUp() {


	return true;
}