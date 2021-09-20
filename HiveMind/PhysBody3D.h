#ifndef __PhysBody3D_H__
#define __PhysBody3D_H__

#include "p2List.h"
#include "glmath.h"

class btRigidBody;
class Module;

// =================================================
struct PhysBody3D
{

public:
	PhysBody3D(btRigidBody* body);
	~PhysBody3D();

	void Push(float x, float y, float z);
	void GetTransform(float* matrix) const;
	void SetTransform(const float* matrix) const;
	void SetPos(float x, float y, float z);
	void SetPos(vec3& vecPos);
	void SetAsSensor(bool is_sensor);
	bool GetIsSensor() { return isSensor; }

private:
	bool isSensor;
public:
	btRigidBody* body = nullptr;
	p2List<Module*> collision_listeners;
};

#endif // __PhysBody3D_H__