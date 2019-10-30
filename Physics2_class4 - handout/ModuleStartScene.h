#pragma once
#include "Module.h"
#include "p2List.h"
#include "p2Point.h"
#include "Globals.h"

class ModuleStartScene :public Module {
public:
	ModuleStartScene(Application* app, bool start_enabled = true);
	~ModuleStartScene() {};

	bool Start();
	update_status Update();
	bool CleanUp();
};