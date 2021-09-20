#include "Globals.h"
#include "Application.h"
#include "ModulePlayer.h"
#include "Primitive.h"
#include "PhysVehicle3D.h"
#include "PhysBody3D.h"

ModulePlayer::ModulePlayer(Application* app, bool start_enabled) : Module(app, start_enabled), vehicle(NULL)
{
	turn = acceleration = brake = 0.0f;
}

ModulePlayer::~ModulePlayer()
{}

// Load assets
bool ModulePlayer::Start()
{
	LOG("Loading player");
	offsetFloor = App->scene_intro->offsetOfFloor;

	// Load Fx
	dead = App->audio->LoadFx("Assets/Fx/player_die.wav");

	boings.add(App->audio->LoadFx("Assets/Fx/boing_1.wav"));
	boings.add(App->audio->LoadFx("Assets/Fx/boing_2.wav"));
	boings.add(App->audio->LoadFx("Assets/Fx/boing_3.wav"));
	boings.add(App->audio->LoadFx("Assets/Fx/boing_4.wav"));
	boings.add(App->audio->LoadFx("Assets/Fx/boing_5.wav"));
	boings.add(App->audio->LoadFx("Assets/Fx/boing_6.wav"));

	stops.add(App->audio->LoadFx("Assets/Fx/stop_1.wav"));
	stops.add(App->audio->LoadFx("Assets/Fx/stop_2.wav"));
	stops.add(App->audio->LoadFx("Assets/Fx/stop_3.wav"));
	stops.add(App->audio->LoadFx("Assets/Fx/stop_4.wav"));
	stops.add(App->audio->LoadFx("Assets/Fx/stop_5.wav"));
	stops.add(App->audio->LoadFx("Assets/Fx/stop_6.wav"));
	stops.add(App->audio->LoadFx("Assets/Fx/stop_7.wav"));

	jumpTime = new Timer();
	jumpTime->Start();
	VehicleInfo car;
	color.Set(1.0f, 1.0f, 1.0f, 1.0f);
	camIntro.Set(180, 62.38+ offsetFloor, 51.7);
	camLoop.Set(-8.88, 62.38+ offsetFloor, 51.7);
// ----------------------------------------Vehicle chassis----//
	car.chassis_size.Set(1, 0.5f, 2);
	car.chassis_offset.Set(0, 0.125f, 0.05); //FUYM

	car.chassis2_size.Set(0.90, 0.35, 1);
	car.chassis2_offset.Set(0, car.chassis_offset.y+0.35, car.chassis_offset.z -0.1);

	car.chassis3_size.Set(1.10, 0.10, 0.25);
	car.chassis3_offset.Set(0, car.chassis_offset.y + 0.50, car.chassis_offset.z- car.chassis_size.z/2.05f);

	car.chassis4_size.Set(0.1, 0.30, 0.1);
	car.chassis4_offset.Set(-0.25, car.chassis_offset.y + 0.70, car.chassis_offset.z-0.4);

	car.chassis5_size.Set(0.075, 0.60, 0.075);
	car.chassis5_offset.Set(-0.25, car.chassis_offset.y + 0.95, car.chassis_offset.z-0.4);
	
	
	//car.sensor_size.Set(1,1,1);
	//car.sensor_offset.Set(0,5, 0);

	// Car properties ----------------------------------------

	car.mass =130.0f;
	car.suspensionStiffness = 26.10f;
	car.suspensionCompression = 1.42f;
	car.suspensionDamping =2.35f;
	car.maxSuspensionTravelCm = 510;
	car.frictionSlip = 100.5;
	car.maxSuspensionForce = 1000.0f;


	// Wheel properties ---------------------------------------
	float connection_height = car.chassis_size.y- car.chassis_offset.z+0.75;
	float wheel_radius = 0.6f;
	float wheel_width = 0.75f;
	float suspensionRestLength = 1.25f;

	// Don't change anything below this line ------------------

	float half_width = car.chassis_size.x+0.28f;

	float half_length = car.chassis_size.z*0.6f;
	
	vec3 direction(0,-1,0);
	vec3 axis(-1,0,0);
	
	car.num_wheels = 4;
	car.wheels = new Wheel[4];

	// FRONT-LEFT ------------------------
	car.wheels[0].connection.Set(half_width - 0.4f * wheel_width, connection_height, half_length - wheel_radius + 0.2 + car.chassis_offset.z);
	car.wheels[0].direction = direction;
	car.wheels[0].axis = axis;
	car.wheels[0].suspensionRestLength = suspensionRestLength;
	car.wheels[0].radius = wheel_radius * 0.95;
	car.wheels[0].width = wheel_width * 0.9;
	car.wheels[0].front = true;
	car.wheels[0].drive = true;
	car.wheels[0].brake = false;
	car.wheels[0].steering = true;



	// FRONT-RIGHT ------------------------
	car.wheels[1].connection.Set(-half_width + 0.4f * wheel_width, connection_height, half_length - wheel_radius + 0.2 + car.chassis_offset.z);
	car.wheels[1].direction = direction;
	car.wheels[1].axis = axis;
	car.wheels[1].suspensionRestLength = suspensionRestLength;
	car.wheels[1].radius = wheel_radius*0.95;
	car.wheels[1].width = wheel_width * 0.9;
	car.wheels[1].front = true;
	car.wheels[1].drive = true;
	car.wheels[1].brake = false;
	car.wheels[1].steering = true;

	// REAR-LEFT ------------------------
	car.wheels[2].connection.Set(half_width - 0.3f * wheel_width, connection_height, -half_length + wheel_radius-0.3 + car.chassis_offset.z);
	car.wheels[2].direction = direction;
	car.wheels[2].axis = axis;
	car.wheels[2].suspensionRestLength = suspensionRestLength;
	car.wheels[2].radius = wheel_radius;
	car.wheels[2].width = wheel_width;
	car.wheels[2].front = false;
	car.wheels[2].drive = false;
	car.wheels[2].brake = true;
	car.wheels[2].steering = false;

	// REAR-RIGHT ------------------------
	car.wheels[3].connection.Set(-half_width + 0.3f * wheel_width, connection_height, -half_length + wheel_radius-0.3 + car.chassis_offset.z);
	car.wheels[3].direction = direction;
	car.wheels[3].axis = axis;
	car.wheels[3].suspensionRestLength = suspensionRestLength;
	car.wheels[3].radius = wheel_radius;
	car.wheels[3].width = wheel_width;
	car.wheels[3].front = false;
	car.wheels[3].drive = false;
	car.wheels[3].brake = true;
	car.wheels[3].steering = false;

	
	// Sensors
	{
		VehicleInfo sensorInf;

		sensorInf.sensor_size.Set(0.75f, 0.5f, 0.75f);
		sensorInf.sensor_offset.Set(0,0,0);
		sensorInf.mass = 0.001f;
		sensorInf.num_wheels = 0;


		sensorV = App->physics->AddVehicle(sensorInf);
		sensorV->body->setGravity({0,0,0});
		sensorV->color.Set(1,0.5,0.5);
		//sensorV->collision_listeners.add(this);
		sensorV->SetAsSensor(true);
		sensorV->body->setUserPointer(sensorV);
		sensorV->body->setCollisionFlags(sensorV->body->getCollisionFlags() | btCollisionObject::CO_GHOST_OBJECT);


		cubeSensor.SetPos(0, 10, 0);
		cubeSensor.size = {0.25,0.25,0.25 };
		cubeSensor.color = White;
		bodySensor =App->physics->AddBody(cubeSensor, 0);
		
		//App->physics->world->addCollisionObject(bodySensor);

		bodySensor->collision_listeners.add(this);
		//bodySensor->body->setUserPointer(bodySensor);
		bodySensor->SetAsSensor(true);
		bodySensor->body->setCollisionFlags(bodySensor->body->getCollisionFlags() | btCollisionObject::CO_GHOST_OBJECT);

		bodySensor->SetPos(0, 10, 0);


	}
	

	vehicle = App->physics->AddVehicle(car);

	vehicle->body->setFriction(1);
	vehicle->collision_listeners.add(this);
	vehicle->body->setUserPointer(vehicle);
	vec3 cam = App->scene_intro->checkopints.at(0).data->GetPos();
	float* pos = App->scene_intro->checkopints.at(0).data->transform.M;
	vehicle->SetTransform(pos);
	vehicle->SetPos(cam.x, cam.y, cam.z);

	return true;
}

// Unload assets
bool ModulePlayer::CleanUp()
{
	LOG("Unloading player");
	//vehicle->body->~btRigidBody();
	return true;
}

// Update: draw background
update_status ModulePlayer::Update(float dt)
{
	positionCM = vehicle->body->getCenterOfMassPosition();
	brake = 2.5f;
	turn = acceleration = 0.0f;
	AssistDirection(90.0f);
	vehicle->vehicle->getChassisWorldTransform();

	// reposiciono el sensor con respecto al chasis	
	{
		btQuaternion q = vehicle->vehicle->getChassisWorldTransform().getRotation();

		cubeSensor.SetPos(positionCM.getX(), positionCM.getY() - 0.55, positionCM.getZ());
		vehicle->vehicle->getChassisWorldTransform().getOpenGLMatrix(&cubeSensor.transform);
		btVector3 offset(0, -0.55, 0);
		offset = offset.rotate(q.getAxis(), q.getAngle());

		cubeSensor.transform.M[12] += offset.getX();
		cubeSensor.transform.M[13] += offset.getY();
		cubeSensor.transform.M[14] += offset.getZ();
		float* pos = cubeSensor.transform.M;
		bodySensor->SetTransform(pos);
		sensorV->SetTransform(pos);
		
	}
	
	// Verifico que el jugador haya pasado el límite mínimo jugable y repoduzco fx die.
	// OffsetFloor = distancia al 0,0,0 sobre el eje y
	if (vehicle->body->getCenterOfMassPosition().getY() < offsetFloor-1 )
	{
		if(!falling)App->audio->PlayFx(dead);
		falling = true;
	}
	// Verifico que el jugador haya pasado el límite mínimo para morir, entonces le hago reaparecer. 
	//OffsetFloor = distancia al 0,0,0 sobre el eje y
	if (vehicle->body->getCenterOfMassPosition().getY() < offsetFloor - 40) respawn = true;

	// Si la reaparición es verdadera, el jugador reaparece en el último punto de control.
	// Si el jugador presiona cualquier tecla de punto de control, reaparecerá en ese punto de control.
	CheckPoints();

	vehicle->SetColor( color);
	color.Set(1.0f, 1.0f, 1.0f, 1.0f);
	forwardVector = vehicle->vehicle->getForwardVector().normalize();
	//btVector3 per = { q.getAxis().getX() ,q.getAxis().getY() ,q.getAxis().getZ() };
	perpendicularVector = { -forwardVector.getZ(), forwardVector.getY(), forwardVector.getX() };

	if(!App->GetDebugMode() && !App->scene_intro->win && introFinish)PlayerControls();

	vehicle->ApplyEngineForce(acceleration);
	vehicle->Turn(turn);
	vehicle->Brake(brake);
	vehicle->Render();

	// Resetea la escena Intro por si el jugador quiere volver a verla
	if (App->input->GetKey(SDL_SCANCODE_I) == KEY_DOWN)
	{
		introFinish = false;
		camIntro.Set(180, 62.38 + offsetFloor, 51.7);
	}
	if (App->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN)introFinish = true;
	if (introFinish)
	{
		if (!App->scene_intro->win)CameraPlayer(dt);
		if (App->scene_intro->win && App->scene_intro->cameraWin)CameraWin(dt);
	

	}
	else CameraIntro(dt);

	return UPDATE_CONTINUE;
}

void ModulePlayer::PlayerControls()
{

	if(jumpTime->Read()>500)
	if (App->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN && vehicle->state != State::IN_AIR && !isJumped)
	{
		jumpTime->Start();
		isJumped = true;
		// FUYM simula un salto de amortiguación, este salto se puede realizar únicamente mientras el sensor este colisionando
		vehicle->vehicle->getRigidBody()->applyCentralForce({ 0,51000,0 });
		// Reproduce un efecto de sonido de salto diferente de forma aleatoria del 0 al 6 por salto
		App->audio->PlayFx(boings.at((rand() % 5) ).data);
	}

	//if (!vehicle->state == State::IN_AIR) vehicle->state = IDLE;

	// Duplica la velocidad en estado turbo
	if (App->input->GetKey(SDL_SCANCODE_LSHIFT) == KEY_REPEAT &&
		(vehicle->state != State::IN_AIR || vehicle->state == State::TURBO) &&
		App->input->GetKey(SDL_SCANCODE_S) != KEY_REPEAT)
	{
		vel = MAX_ACCELERATION * 2;
		vehicle->state = TURBO;
		// FUYM inclinacion del coche
		vehicle->vehicle->getRigidBody()->applyCentralForce({ 0,-99,0 });
		//vehicle->body->applyTorque(per * 80);
	}
	else
	{
		vel = MAX_ACCELERATION;
		if (vehicle->state != State::IN_AIR)vehicle->state = State::IDLE;

	}

	if (App->input->GetKey(SDL_SCANCODE_W) == KEY_REPEAT)
	{
		if (vehicle->state != State::TURBO && vehicle->state != State::IN_AIR)vehicle->state = State::WALK;
		vehicle->vehicle->getRigidBody()->applyCentralForce({ 0,-70,0 });

		// Si el jugador cambia de dirección de marcha atrás hacia delante, se aplica un freno para simular el comportamiento de un coche radiocontrol
		if (vehicle->vehicle->getCurrentSpeedKmHour() <= -2.25)
		{
			// Se reproduce un sonido de frenada aleatorio del 0 al 7
			if (vehicle->state != State::IN_AIR)App->audio->PlayFx(stops.at((rand() % 6)).data);
			brake = BRAKE_POWER / 1.5f;
			// Cambia el color del alerón para simular la frenada
			color.Set(1.0f, 0.0f, 0.0f, 1.0f);
			// FUYM para simular el rebote de la amortiguación de un radiocontrol	
			vehicle->vehicle->getRigidBody()->applyCentralForce({ 0,-200,0 });
		}
		else
			acceleration = vel;
		vehicle->body->applyTorque(perpendicularVector * -40);

	}

	if (App->input->GetKey(SDL_SCANCODE_S) == KEY_REPEAT)
	{
		if (vehicle->state != State::TURBO && vehicle->state != State::IN_AIR)vehicle->state = State::WALK;

		// Si el jugador cambia de dirección de marcha delante hacia atras, se aplica un freno para simular el comportamiento de un coche radiocontrol
		if (vehicle->vehicle->getCurrentSpeedKmHour() > +2.25)
		{
			brake = BRAKE_POWER / 1.5f;
			// Cambia el color del alerón para simular la frenada
			color.Set(1.0f, 0.0f, 0.0f, 1.0f);
			// FUYM para simular el rebote de la amortiguación de un radiocontrol	
			vehicle->vehicle->getRigidBody()->applyCentralForce({ 0,-200,0 });
			if(vehicle->state!= State::IN_AIR)App->audio->PlayFx(stops.at((rand() % 7)).data);

		}
		else
			acceleration = vel * -1;
		vehicle->body->applyTorque(perpendicularVector * 80);
	}

	if (App->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT)
	{
		if (turn < TURN_DEGREES)
			turn += (TURN_DEGREES)-assistDirection;
		brake = 15;

		if (vehicle->state == State::IN_AIR)
		{
			vehicle->body->applyTorque(forwardVector * -45);
		}
		else
		{
			vehicle->body->applyTorque(forwardVector * -200);
		}

	}

	if (App->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT)
	{
		if (turn > -TURN_DEGREES)
			turn -= (TURN_DEGREES)-assistDirection;
		brake = 15;

		if (vehicle->state == State::IN_AIR)
		{
			vehicle->body->applyTorque(forwardVector * 45);
		}
		else
		{
			vehicle->body->applyTorque(forwardVector * 200);
			//vehicle->vehicle->getRigidBody()->applyCentralForce({ -1000,0,0 });

		}
		//LOG("%d ", (int)vehicle->body->getTotalTorque().length());
	}
}

void ModulePlayer::CheckPoints()
{
	// Si el jugador muere, respawneara en el ultimo checkpoint activado
	if(respawn) Teleport(lastChekPoint);
	// Si el jugador pulsa la letra O mientras esta en freeCam, spawneara el coche ne su posicion 
	if (App->input->GetKey(SDL_SCANCODE_O) == KEY_DOWN && App->GetDebugMode())
	{
		vec3 cam = App->camera->Position;
		vehicle->SetPos(cam.x, cam.y - 5, cam.z);
	}
	// Si el jugador pulsa cualquier tecla de respawn, se le teletransportara a la asociada a esta
	if (App->input->GetKey(SDL_SCANCODE_R) == KEY_DOWN) Teleport(0);
	if (App->input->GetKey(SDL_SCANCODE_1) == KEY_DOWN) Teleport(1);
	if (App->input->GetKey(SDL_SCANCODE_2) == KEY_DOWN) Teleport(2);
	if (App->input->GetKey(SDL_SCANCODE_3) == KEY_DOWN) Teleport(3);
	if (App->input->GetKey(SDL_SCANCODE_4) == KEY_DOWN) Teleport(4);
	if (App->input->GetKey(SDL_SCANCODE_5) == KEY_DOWN) Teleport(5);
	if (App->input->GetKey(SDL_SCANCODE_6) == KEY_DOWN) Teleport(6);
	if (App->input->GetKey(SDL_SCANCODE_7) == KEY_DOWN) Teleport(7);
}

void ModulePlayer::Teleport(int num)
{
	respawn = false;
	// seteo tanto la velocidad lineal como la angular a 0
	vehicle->body->setLinearVelocity({ 0,0,0 });
	vehicle->body->setAngularVelocity({ 0,0,0 });
	// Copio la direccion del CheckPoint y su posicion en el vehiculo 
	vec3 cam = App->scene_intro->checkopints.at(num).data->GetPos();
	float* pos = App->scene_intro->checkopints.at(num).data->transform.M;
	vehicle->SetTransform(pos);
	vehicle->SetPos(cam.x, cam.y, cam.z);
	falling = false;

}

void ModulePlayer::CameraPlayer(float dt)
{
	if (!App->GetDebugMode())
	{
		vec3 myCamera;
		vec3 myCameraLook;
		float distanceCamara2CM = -12;
		
			if (((camLoop.x - 36) < positionCM.getX() && (camLoop.x + 36) > positionCM.getX())
				&& (((camLoop.y) - 54 < positionCM.getY() && (camLoop.y + 54) > positionCM.getY()))
				&& (((camLoop.z - 54) < positionCM.getZ() && (camLoop.z + 54) > positionCM.getZ())))
			{
				myCamera = camLoop;
			}
			else
			{
				if (!falling)
				{
					// Posiciono la cámara detrás del jugador en esta variable 
					myCamera.x = positionCM.getX() + forwardVector.getX() * distanceCamara2CM;
					myCamera.y = positionCM.getY() + forwardVector.getY() + 6;
					myCamera.z = positionCM.getZ() + forwardVector.getZ() * distanceCamara2CM;

					lastCam = myCamera;
				}
				else
				{
					// Aquí hago la animación encargada del efecto derrota
					if (lastCam.y < 1035) lastCam.y += 0.50;
					myCamera = lastCam;
				}

			}

		// Oriento la cámara para que mire al coche y guardo el valor en esta variable para después setearlo	
		myCameraLook.x = vehicle->body->getCenterOfMassPosition().getX();
		myCameraLook.y = vehicle->body->getCenterOfMassPosition().getY() + 4;
		myCameraLook.z = vehicle->body->getCenterOfMassPosition().getZ();
		if(App->input->GetKey(SDL_SCANCODE_P)== KEY_DOWN)LOG("Position Player \n x: %f \t z: %f ", myCamera.x, myCamera.z);

		// Utilizo la variable myCamera cambiada anteriormente para setear la posición	
		App->camera->Position = myCamera;
		// Utilizo la variable myCameraLook cambiada anteriormente para setear la orientación
		App->camera->LookAt(myCameraLook);
	}
}

void ModulePlayer::CameraIntro(float dt)
{
	vec3 myCamera = camIntro;
	vec3 myCameraLook = camIntro;
	vec3 posTurnCam = { -136, (float)62.38 + offsetFloor, -17.15};
	float distanceCamara2CM = 68.85;

	// Mientras no ha llegado a la posición, se mueve en línea recta1
	if (camIntro.x > -150 && camIntro.y > 1015 && camIntro.z > 50)
	{
		camIntro.x -= 50 * dt;
		myCameraLook.x = camIntro.x - 1;
	}
	// Una vez llegada la posición, dibujo media circunferencia reduciendo al mismo tiempo mi eje Y
	else if(camIntro.x < -122 || camIntro.y > 1007)
	{
		angle -= dt / 2;
		if (camIntro.x < -122) camIntro.x = distanceCamara2CM * sin(angle) + posTurnCam.x;
		if (camIntro.y > 1007) camIntro.y -= 14*dt;
		if (camIntro.z >  -85) camIntro.z = distanceCamara2CM * cos(angle) + posTurnCam.z;

		if (camIntro.x < -122) myCameraLook.x = distanceCamara2CM * sin(angle - (dt/2)) + posTurnCam.x;
		if (camIntro.z >  -85) myCameraLook.z = distanceCamara2CM * cos(angle - (dt/2)) + posTurnCam.z;
	}
	else 
	{
		angle = 0;
		introFinish = true;
	}

	App->camera->Position = myCamera;
	App->camera->LookAt(myCameraLook);
}

void ModulePlayer::CameraWin(float dt)
{
	
	vehicle->body->setLinearVelocity({ 0,0,0 });

	vec3 myCamera;
	vec3 myCameraLook = { -50,50.0f+ offsetFloor,0 };
	float distanceCamara2CM = -250;

	angle -= dt / 4;
	myCamera.x = distanceCamara2CM * sin(angle);
	myCamera.y = 140+offsetFloor;
	myCamera.z = distanceCamara2CM * cos(angle);

	App->camera->Position = myCamera;
	App->camera->LookAt(myCameraLook);

	// Si la cámara ha dado dos vueltas o pulsa espacio, reinicia el nivel
	if (angle < -(4 * PI) || App->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN)
	{
		App->scene_intro->cameraWin = false;
		angle = 0;
		App->scene_intro->win = false;
		App->audio->PlayMusic("Assets/Music/fall_guys.ogg");
		Teleport(0);
		
	}
}

void ModulePlayer::AssistDirection(float hardness)
{
	// FUYM que reduce la cantidad de giro que puede ejercer la rueda respecto a la cantidad de velocidad
	float turnDegrees = (TURN_DEGREES / DEGTORAD);
	calculate = (vehicle->GetKmh() / 16) * (hardness/100.0f);
	if (calculate <= turnDegrees-5)
		assistDirection = calculate * DEGTORAD;
	else assistDirection = (turnDegrees-5) * DEGTORAD;

}
void ModulePlayer::OnCollision(PhysBody3D* body1, PhysBody3D* body2)
{

}

// Aquí hice la normal antes de averiguar que el motor ya la podía calcular
//btVector3 ModulePlayer::Norm(btVector3 vec)
//{
//	btVector3 vecNorm = vec;
//
//	
//	vecNorm.x= vecNorm.x/(sqrt((vecNorm.x* vecNorm.x)+(vecNorm.y* vecNorm.y)+(vecNorm.z* vecNorm.z)));
//	vecNorm.y = vecNorm.y / (sqrt((vecNorm.x * vecNorm.x) + (vecNorm.y * vecNorm.y) + (vecNorm.z * vecNorm.z)));;
//	vecNorm.z = vecNorm.z / (sqrt((vecNorm.x * vecNorm.x) + (vecNorm.y * vecNorm.y) + (vecNorm.z * vecNorm.z)));;
//	return vecNorm;
//}



