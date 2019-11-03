#include "Globals.h"
#include "Application.h"
#include "ModuleInput.h"
#include "ModuleRender.h"
#include "ModulePhysics.h"
#include "ModuleSceneIntro.h"
#include "p2Point.h"
#include "math.h"

#ifdef _DEBUG
#pragma comment( lib, "Box2D/libx86/Debug_/Box2D.lib" )
#else
#pragma comment( lib, "Box2D/libx86/Release_/Box2D.lib" )
#endif

ModulePhysics::ModulePhysics(Application* app, bool start_enabled) : Module(app, start_enabled)
{
	world = NULL;
	mouse_joint = NULL;
	debug = false;
}

// Destructor
ModulePhysics::~ModulePhysics()
{
}

bool ModulePhysics::Start()
{
	LOG("Creating Physics 2D environment");

	world = new b2World(b2Vec2(GRAVITY_X, -GRAVITY_Y));
	world->SetContactListener(this);

	// needed to create joints like mouse joint
	b2BodyDef bd;
	ground = world->CreateBody(&bd);

	// bumper left rotation
	b2BodyDef body;
	body.type = b2_staticBody;
	body.position.Set(PIXEL_TO_METERS(54), PIXEL_TO_METERS(411));
	b2Body* point_rotation_left = world->CreateBody(&body);

	

	//bumper left
	
	PhysBody* bumper_phys_left;// = CreateRectangle(60.5, 414, 22, 16);
	int width = 0;
	int height = 0;
	body.type = b2_dynamicBody;
	body.position.Set(PIXEL_TO_METERS(50), PIXEL_TO_METERS(404));

	b2Body* b = world->CreateBody(&body);

	b2Vec2 vertices[5];
	vertices[0].Set(PIXEL_TO_METERS(0), PIXEL_TO_METERS(0));
	vertices[1].Set(PIXEL_TO_METERS(1), PIXEL_TO_METERS(8));
	vertices[2].Set(PIXEL_TO_METERS(24), PIXEL_TO_METERS(9));
	vertices[3].Set(PIXEL_TO_METERS(8), PIXEL_TO_METERS(1));
	vertices[4].Set(PIXEL_TO_METERS(2), PIXEL_TO_METERS(0));
	int32 count = 5;
	b2PolygonShape box;
	box.Set(vertices, count);



	b2FixtureDef fixture;
	fixture.shape = &box;
	fixture.filter.groupIndex = -1;
	fixture.density = 10.0f;

	b->CreateFixture(&fixture);

	bumper_phys_left = new PhysBody();
	bumper_phys_left->body = b;
	b->SetUserData(bumper_phys_left);
	bumper_phys_left->width = width * 0.5f;
	bumper_phys_left->height = height * 0.5f;

	//joint bumper left
	App->scene_intro->bumper_left = bumper_phys_left;
	b2RevoluteJointDef bumper_joint_def;
	bumper_joint_def.bodyA = point_rotation_left;
	bumper_joint_def.bodyB = bumper_phys_left->body;
	bumper_joint_def.localAnchorA.Set(0, 0);
	bumper_joint_def.localAnchorB.Set(PIXEL_TO_METERS(6), PIXEL_TO_METERS(5));
	bumper_joint_def.collideConnected = false;
	bumper_joint_def.enableLimit = true;
	bumper_joint_def.referenceAngle = 0;
	bumper_joint_def.lowerAngle = -45 * DEGTORAD;
	bumper_joint_def.upperAngle = 0;
	bumper_joint_def.enableMotor = false;
	bumper_joint_def.maxMotorTorque = 5.0f;
	bumper_joint_def.motorSpeed = 0 * DEGTORAD;
	bumper_joint_left = (b2RevoluteJoint*)world->CreateJoint(&bumper_joint_def);


	// bumper right rotation
	body.type = b2_staticBody;
	body.position.Set(PIXEL_TO_METERS(107), PIXEL_TO_METERS(411));
	b2Body* point_rotation_right = world->CreateBody(&body);



	//bumper right

	PhysBody* bumper_phys_right;// = CreateRectangle(60.5, 414, 22, 16);
	body.type = b2_dynamicBody;
	body.position.Set(PIXEL_TO_METERS(100), PIXEL_TO_METERS(404));

	b2Body* b2 = world->CreateBody(&body);

	//b2Vec2 vertices[5];
	vertices[1].Set(PIXEL_TO_METERS(-1), PIXEL_TO_METERS(8));
	vertices[2].Set(PIXEL_TO_METERS(-24), PIXEL_TO_METERS(9));
	vertices[3].Set(PIXEL_TO_METERS(-8), PIXEL_TO_METERS(1));
	vertices[4].Set(PIXEL_TO_METERS(-2), PIXEL_TO_METERS(0));
	box.Set(vertices, count);



	fixture.shape = &box;
	fixture.filter.groupIndex = -1;
	fixture.density = 10.0f;

	b2->CreateFixture(&fixture);

	bumper_phys_right = new PhysBody();
	bumper_phys_right->body = b2;
	b2->SetUserData(bumper_phys_right);
	bumper_phys_right->width = width * 0.5f;
	bumper_phys_right->height = height * 0.5f;

	App->scene_intro->bumper_right = bumper_phys_right;
	bumper_joint_def.bodyA = point_rotation_right;
	bumper_joint_def.bodyB = bumper_phys_right->body;
	bumper_joint_def.localAnchorA.Set(0, 0);
	bumper_joint_def.localAnchorB.Set(PIXEL_TO_METERS(-6), PIXEL_TO_METERS(5));
	bumper_joint_def.collideConnected = false;
	bumper_joint_def.enableLimit = true;
	bumper_joint_def.referenceAngle = 0;
	bumper_joint_def.lowerAngle = 0;
	bumper_joint_def.upperAngle = 45 * DEGTORAD;
	bumper_joint_def.enableMotor = false;
	bumper_joint_def.maxMotorTorque = 5.0f;
	bumper_joint_def.motorSpeed = 0 * DEGTORAD;
	bumper_joint_right = (b2RevoluteJoint*)world->CreateJoint(&bumper_joint_def);


	//create rectangles to avoid ball falling while dead

	body.type = b2_staticBody;
	body.position.Set(PIXEL_TO_METERS(36), PIXEL_TO_METERS(430));

	b2 = world->CreateBody(&body);
	vertices[1].Set(PIXEL_TO_METERS(35), PIXEL_TO_METERS(20));
	vertices[2].Set(PIXEL_TO_METERS(35), PIXEL_TO_METERS(137));
	vertices[3].Set(PIXEL_TO_METERS(0), PIXEL_TO_METERS(137));
	count = 4;
	box.Set(vertices, count);

	fixture.shape = &box;
	fixture.filter.groupIndex = -1;
	
	b2->CreateFixture(&fixture);

	body.type = b2_staticBody;
	body.position.Set(PIXEL_TO_METERS(124), PIXEL_TO_METERS(430));

	b2 = world->CreateBody(&body);
	vertices[1].Set(PIXEL_TO_METERS(-35), PIXEL_TO_METERS(20));
	vertices[2].Set(PIXEL_TO_METERS(-35), PIXEL_TO_METERS(137));
	vertices[3].Set(PIXEL_TO_METERS(0), PIXEL_TO_METERS(137));
	box.Set(vertices, count);

	fixture.shape = &box;
	fixture.filter.groupIndex = -1;

	number_to_destroy = 0;
	b2->CreateFixture(&fixture);


	return true;
}

// 
update_status ModulePhysics::PreUpdate()
{
	world->Step(1.0f / 60.0f, 6, 2);

	for (b2Contact* c = world->GetContactList(); c; c = c->GetNext())
	{
		if (c->GetFixtureA()->IsSensor() && c->IsTouching())
		{
			PhysBody* pb1 = (PhysBody*)c->GetFixtureA()->GetBody()->GetUserData();
			PhysBody* pb2 = (PhysBody*)c->GetFixtureA()->GetBody()->GetUserData();
			if (pb1 && pb2 && pb1->listener)
				pb1->listener->OnCollision(pb1, pb2);
		}
	}

	return UPDATE_CONTINUE;
}

PhysBody* ModulePhysics::CreateCircle(int x, int y, int radius)
{
	b2BodyDef body;
	body.type = b2_staticBody;
	body.position.Set(PIXEL_TO_METERS(x), PIXEL_TO_METERS(y));

	b2Body* b = world->CreateBody(&body);

	b2CircleShape shape;
	shape.m_radius = PIXEL_TO_METERS(radius);
	b2FixtureDef fixture;
	fixture.shape = &shape;
	fixture.density = 1.0f;

	b->CreateFixture(&fixture);

	PhysBody* pbody = new PhysBody();
	pbody->body = b;
	b->SetUserData(pbody);
	pbody->width = pbody->height = radius;

	return pbody;
}

PhysBody* ModulePhysics::CreateRectangle(int x, int y, int width, int height, bool not_collide)
{
	b2BodyDef body;
	body.type = b2_dynamicBody;
	body.position.Set(PIXEL_TO_METERS(x), PIXEL_TO_METERS(y));

	b2Body* b = world->CreateBody(&body);
	b2PolygonShape box;
	box.SetAsBox(PIXEL_TO_METERS(width) * 0.5f, PIXEL_TO_METERS(height) * 0.5f);

	b2FixtureDef fixture;
	fixture.shape = &box;
	fixture.density = 1.0f;
	if (not_collide == true)fixture.filter.groupIndex = -1;

	b->CreateFixture(&fixture);

	PhysBody* pbody = new PhysBody();
	pbody->body = b;
	b->SetUserData(pbody);
	pbody->width = width * 0.5f;
	pbody->height = height * 0.5f;

	return pbody;
}

PhysBody* ModulePhysics::CreateChain(int x, int y, int* points, int size, bool not_collide_bumpers)
{
	b2BodyDef body;
	body.type = b2_staticBody;
	body.position.Set(PIXEL_TO_METERS(x), PIXEL_TO_METERS(y));

	b2Body* b = world->CreateBody(&body);

	b2ChainShape shape;
	b2Vec2* p = new b2Vec2[size / 2];

	for (uint i = 0; i < size / 2; ++i)
	{
		p[i].x = PIXEL_TO_METERS(points[i * 2 + 0]);
		p[i].y = PIXEL_TO_METERS(points[i * 2 + 1]);
	}

	shape.CreateLoop(p, size / 2);

	b2FixtureDef fixture;
	fixture.shape = &shape;
	if (not_collide_bumpers == true)fixture.filter.groupIndex = -1;
	b->CreateFixture(&fixture);

	delete p;

	PhysBody* pbody = new PhysBody();
	pbody->body = b;
	b->SetUserData(pbody);
	pbody->width = pbody->height = 0;

	return pbody;
}

// 
update_status ModulePhysics::PostUpdate()
{
	if (App->input->GetKey(SDL_SCANCODE_F1) == KEY_DOWN)
		debug = !debug;

	if (!debug) {
		if (want_to_destroy == true) {
			for (int i = 0; i < number_to_destroy; i++) {
				world->DestroyBody(body_to_destroy[i]->body);
				body_to_destroy[i] = NULL;
			}
			want_to_destroy = false;
			number_to_destroy = 0;
		}
		return UPDATE_CONTINUE;
	}

	// Bonus code: this will iterate all objects in the world and draw the circles
	// You need to provide your own macro to translate meters to pixels

	PhysBody* body_contact = nullptr;

	for (b2Body* b = world->GetBodyList(); b; b = b->GetNext())
	{
		for (b2Fixture* f = b->GetFixtureList(); f; f = f->GetNext())
		{
			switch (f->GetType())
			{
				// Draw circles ------------------------------------------------
			case b2Shape::e_circle:
			{
				b2CircleShape* shape = (b2CircleShape*)f->GetShape();
				b2Vec2 pos = f->GetBody()->GetPosition();
				App->renderer->DrawCircle(METERS_TO_PIXELS(pos.x), METERS_TO_PIXELS(pos.y), METERS_TO_PIXELS(shape->m_radius), 255, 0, 255);
			}
			break;

			// Draw polygons ------------------------------------------------
			case b2Shape::e_polygon:
			{
				b2PolygonShape* polygonShape = (b2PolygonShape*)f->GetShape();
				int32 count = polygonShape->GetVertexCount();
				b2Vec2 prev, v;

				for (int32 i = 0; i < count; ++i)
				{
					v = b->GetWorldPoint(polygonShape->GetVertex(i));
					if (i > 0)
						App->renderer->DrawLine(METERS_TO_PIXELS(prev.x), METERS_TO_PIXELS(prev.y), METERS_TO_PIXELS(v.x), METERS_TO_PIXELS(v.y), 255, 100, 100);

					prev = v;
				}

				v = b->GetWorldPoint(polygonShape->GetVertex(0));
				App->renderer->DrawLine(METERS_TO_PIXELS(prev.x), METERS_TO_PIXELS(prev.y), METERS_TO_PIXELS(v.x), METERS_TO_PIXELS(v.y), 255, 100, 100);
			}
			break;

			// Draw chains contour -------------------------------------------
			case b2Shape::e_chain:
			{
				b2ChainShape* shape = (b2ChainShape*)f->GetShape();
				b2Vec2 prev, v;

				for (int32 i = 0; i < shape->m_count; ++i)
				{
					v = b->GetWorldPoint(shape->m_vertices[i]);
					if (i > 0)
						App->renderer->DrawLine(METERS_TO_PIXELS(prev.x), METERS_TO_PIXELS(prev.y), METERS_TO_PIXELS(v.x), METERS_TO_PIXELS(v.y), 100, 255, 100);
					prev = v;
				}

				v = b->GetWorldPoint(shape->m_vertices[0]);
				App->renderer->DrawLine(METERS_TO_PIXELS(prev.x), METERS_TO_PIXELS(prev.y), METERS_TO_PIXELS(v.x), METERS_TO_PIXELS(v.y), 100, 255, 100);
			}
			break;

			// Draw a single segment(edge) ----------------------------------
			case b2Shape::e_edge:
			{
				b2EdgeShape* shape = (b2EdgeShape*)f->GetShape();
				b2Vec2 v1, v2;

				v1 = b->GetWorldPoint(shape->m_vertex0);
				v1 = b->GetWorldPoint(shape->m_vertex1);
				App->renderer->DrawLine(METERS_TO_PIXELS(v1.x), METERS_TO_PIXELS(v1.y), METERS_TO_PIXELS(v2.x), METERS_TO_PIXELS(v2.y), 100, 100, 255);
			}
			break;
			}
			if (App->input->GetMouseButton(SDL_BUTTON_LEFT) == KEY_DOWN && body_contact == nullptr) {
				PhysBody* bo = (PhysBody*)b->GetUserData();
				if (bo != NULL && bo->Contains(App->input->GetMouseX(), App->input->GetMouseY()) == true) {
					body_contact = (PhysBody*)b->GetUserData();
				}
			}
		}
	}

	if (mouse_joint == NULL && body_contact != nullptr) {
		b2MouseJointDef def;
		def.bodyA = ground;
		def.bodyB = body_contact->body;
		def.target = { PIXEL_TO_METERS(App->input->GetMouseX()),PIXEL_TO_METERS(App->input->GetMouseY()) };
		def.dampingRatio = 0.5f;
		def.frequencyHz = 2.0f;
		def.maxForce = 100.0f * body_contact->body->GetMass();
		mouse_joint = (b2MouseJoint*)world->CreateJoint(&def);
	}


	else if (App->input->GetMouseButton(SDL_BUTTON_LEFT) == KEY_REPEAT && mouse_joint != NULL) {
		mouse_joint->SetTarget({ PIXEL_TO_METERS(App->input->GetMouseX()), PIXEL_TO_METERS(App->input->GetMouseY()) });
		App->renderer->DrawLine(METERS_TO_PIXELS(mouse_joint->GetAnchorA().x), METERS_TO_PIXELS(mouse_joint->GetAnchorA().y), METERS_TO_PIXELS(mouse_joint->GetAnchorB().x), METERS_TO_PIXELS(mouse_joint->GetAnchorB().y), 255, 0, 0);
	}

	else if (App->input->GetMouseButton(SDL_BUTTON_LEFT) == KEY_UP && mouse_joint != NULL) {
		world->DestroyJoint(mouse_joint);
		mouse_joint = NULL;
		body_contact = nullptr;
	}
	if (want_to_destroy == true) {
		for (int i = 0; i < number_to_destroy; i++) {
			world->DestroyBody(body_to_destroy[i]->body);
			body_to_destroy[i] = NULL;
		}
		want_to_destroy = false;
		number_to_destroy = 0;
	}
	return UPDATE_CONTINUE;
}


// Called before quitting
bool ModulePhysics::CleanUp()
{
	LOG("Destroying physics world");

	// Delete the whole physics world!
	delete world;

	return true;
}

void PhysBody::GetPosition(int& x, int &y) const
{
	b2Vec2 pos = body->GetPosition();
	x = METERS_TO_PIXELS(pos.x) - (width);
	y = METERS_TO_PIXELS(pos.y) - (height);
}

float PhysBody::GetRotation() const
{
	return RADTODEG * body->GetAngle();
}

bool PhysBody::Contains(int x, int y) const
{
	b2Vec2 p(PIXEL_TO_METERS(x), PIXEL_TO_METERS(y));

	const b2Fixture* fixture = body->GetFixtureList();

	while (fixture != NULL)
	{
		if (fixture->GetShape()->TestPoint(body->GetTransform(), p) == true)
			return true;
		fixture = fixture->GetNext();
	}

	return false;
}

int PhysBody::RayCast(int x1, int y1, int x2, int y2, float& normal_x, float& normal_y) const
{
	int ret = -1;

	b2RayCastInput input;
	b2RayCastOutput output;

	input.p1.Set(PIXEL_TO_METERS(x1), PIXEL_TO_METERS(y1));
	input.p2.Set(PIXEL_TO_METERS(x2), PIXEL_TO_METERS(y2));
	input.maxFraction = 1.0f;

	const b2Fixture* fixture = body->GetFixtureList();

	while (fixture != NULL)
	{
		if (fixture->GetShape()->RayCast(&output, input, body->GetTransform(), 0) == true)
		{
			// do we want the normal ?

			float fx = x2 - x1;
			float fy = y2 - y1;
			float dist = sqrtf((fx*fx) + (fy*fy));

			normal_x = output.normal.x;
			normal_y = output.normal.y;

			return output.fraction * dist;
		}
		fixture = fixture->GetNext();
	}

	return ret;
}

void ModulePhysics::BeginContact(b2Contact* contact)
{
	PhysBody* physA = (PhysBody*)contact->GetFixtureA()->GetBody()->GetUserData();
	PhysBody* physB = (PhysBody*)contact->GetFixtureB()->GetBody()->GetUserData();

	if (physA && physA->listener != NULL)
		physA->listener->OnCollision(physA, physB);

	if (physB && physB->listener != NULL)
		physB->listener->OnCollision(physB, physA);
}


void ModulePhysics::MoveBumper(int bumper_num, bool move_up) { //how we move the bumpers
	if (bumper_num == 1) {
		if (move_up==true) {
			if (bumper_joint_left->GetJointAngle() == (-45 * DEGTORAD)) bumper_joint_left->SetMotorSpeed(0);
			else {
				bumper_joint_left->EnableMotor(true);
				bumper_joint_left->SetMotorSpeed(-1800 * DEGTORAD);
			}
		}
		else {
			if (bumper_joint_left->GetJointAngle() == 0) bumper_joint_left->EnableMotor(false);
			else {
				bumper_joint_left->EnableMotor(true);
				bumper_joint_left->SetMotorSpeed(1800 * DEGTORAD);
			}
		}
	}
	else if (bumper_num == 2) {
		if (move_up == true) {
			if (bumper_joint_right->GetJointAngle() == (45 * DEGTORAD)) bumper_joint_right->SetMotorSpeed(0);
			else {
				bumper_joint_right->EnableMotor(true);
				bumper_joint_right->SetMotorSpeed(1800 * DEGTORAD);
			}
		}
		else {
			if (bumper_joint_right->GetJointAngle() == 0) bumper_joint_right->EnableMotor(false);
			else {
				bumper_joint_right->EnableMotor(true);
				bumper_joint_right->SetMotorSpeed(-1800 * DEGTORAD);
			}
		}
	}
}

void ModulePhysics::DestroyBody(PhysBody* body) {
	want_to_destroy = true;
	body_to_destroy[number_to_destroy] = body;
	number_to_destroy++;
}