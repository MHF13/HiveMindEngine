#pragma once
#include "Module.h"
#include "Globals.h"
#include "glmath.h"

class ModuleCamera3D : public Module
{
public:
	ModuleCamera3D(Application* app, bool start_enabled = true);
	~ModuleCamera3D();

	bool Start();
	update_status Update(float dt);
	bool CleanUp();

	void Look(const vec3 &Position, const vec3 &Reference, bool RotateAroundReference = false);
	void LookAt(const vec3 &Spot);

	// Make the camera to look at the center of the object
	void CenterToObject(GameObject* object);

	void Move(const vec3 &Movement);
	float* GetViewMatrix();

private:

	void CalculateViewMatrix();

public:
	Frustum frustum;

	vec3 X, Y, Z, Position, Reference;

private:
	mat4x4 ViewMatrix, ViewMatrixInverse;
};