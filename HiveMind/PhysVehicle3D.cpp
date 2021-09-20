#include "PhysVehicle3D.h"
#include "Primitive.h"
#include "Bullet/include/btBulletDynamicsCommon.h"



// ----------------------------------------------------------------------------
VehicleInfo::~VehicleInfo()
{
	//if(wheels != NULL)
		//delete wheels;
}

// ----------------------------------------------------------------------------
PhysVehicle3D::PhysVehicle3D(btRigidBody* body, btRaycastVehicle* vehicle, const VehicleInfo& info) : PhysBody3D(body), vehicle(vehicle), info(info)
{
	state = IDLE;
	//body->setCollisionFlags(body->getCollisionFlags() | btCollisionObject::CO_GHOST_OBJECT);

}

// ----------------------------------------------------------------------------
PhysVehicle3D::~PhysVehicle3D()
{
	delete vehicle;
}

// ----------------------------------------------------------------------------
void PhysVehicle3D::Render()
{
	Cylinder wheel;

	for(int i = 0; i < vehicle->getNumWheels(); ++i)
	{
		wheel.radius = info.wheels[i].radius;
		wheel.height = info.wheels[i].width;

		vehicle->updateWheelTransform(i);
		vehicle->getWheelInfo(i).m_worldTransform.getOpenGLMatrix(&wheel.transform);
		if ( state==TURBO)
			if((i == 3 || i == 2))
			wheel.color.Set(1.0f,0.0f,0.0f,0.2f);
			else wheel.color.Set(0.0f,0.0f,0.0f,0.2f);
		else wheel.color.Set(0.0f, 0.0f, 0.0f, 0.2f);
		wheel.Render();

	}
	// 
	btQuaternion q = vehicle->getChassisWorldTransform().getRotation();

	Cube chassis(info.chassis_size.x, info.chassis_size.y, info.chassis_size.z);
	vehicle->getChassisWorldTransform().getOpenGLMatrix(&chassis.transform);
	btVector3 offset(info.chassis_offset.x, info.chassis_offset.y, info.chassis_offset.z);
	offset = offset.rotate(q.getAxis(), q.getAngle());

	chassis.transform.M[12] += offset.getX();
	chassis.transform.M[13] += offset.getY();
	chassis.transform.M[14] += offset.getZ();
	chassis.color.Set(3.872222, 0.579737,0.157043);

	Cube chassis2(info.chassis2_size.x, info.chassis2_size.y, info.chassis2_size.z);
	vehicle->getChassisWorldTransform().getOpenGLMatrix(&chassis2.transform);
	btVector3 offset2(info.chassis2_offset.x, info.chassis2_offset.y, info.chassis2_offset.z);
	offset2 = offset2.rotate(q.getAxis(), q.getAngle());

	chassis2.transform.M[12] += offset2.getX();
	chassis2.transform.M[13] += offset2.getY();
	chassis2.transform.M[14] += offset2.getZ();
	chassis2.color.Set(3.872222, 0.579737, 0.257043);


	Cube chassis3(info.chassis3_size.x, info.chassis3_size.y, info.chassis3_size.z);
	vehicle->getChassisWorldTransform().getOpenGLMatrix(&chassis3.transform);
	btVector3 offset3(info.chassis3_offset.x, info.chassis3_offset.y, info.chassis3_offset.z);
	offset3 = offset3.rotate(q.getAxis(), q.getAngle());

	chassis3.transform.M[12] += offset3.getX();
	chassis3.transform.M[13] += offset3.getY();
	chassis3.transform.M[14] += offset3.getZ();
	chassis3.color = this->GetColor();

	Cube chassis4(info.chassis4_size.x, info.chassis4_size.y, info.chassis4_size.z);
	vehicle->getChassisWorldTransform().getOpenGLMatrix(&chassis4.transform);
	btVector3 offset4(info.chassis4_offset.x, info.chassis4_offset.y, info.chassis4_offset.z);
	offset4 = offset4.rotate(q.getAxis(), q.getAngle());

	chassis4.transform.M[12] += offset4.getX();
	chassis4.transform.M[13] += offset4.getY();
	chassis4.transform.M[14] += offset4.getZ();
	chassis4.color.Set(0.5f, 0.5f, 0.5f, 1.0f);


	Cube chassis5(info.chassis5_size.x, info.chassis5_size.y, info.chassis5_size.z);
	vehicle->getChassisWorldTransform().getOpenGLMatrix(&chassis5.transform);
	btVector3 offset5(info.chassis5_offset.x, info.chassis5_offset.y, info.chassis5_offset.z);
	offset5 = offset5.rotate(q.getAxis(), q.getAngle());

	chassis5.transform.M[12] += offset5.getX();
	chassis5.transform.M[13] += offset5.getY();
	chassis5.transform.M[14] += offset5.getZ();
	chassis5.color.Set(0.75f, 0.75f, 0.75f, 1.0f);


	Cube sensor(info.sensor_size.x, info.sensor_size.y, info.sensor_size.z);
	vehicle->getChassisWorldTransform().getOpenGLMatrix(&sensor.transform);
	btVector3 offsetSensor(info.sensor_offset.x, info.sensor_offset.y, info.sensor_offset.z);
	offsetSensor = offsetSensor.rotate(q.getAxis(), q.getAngle());
	sensor.color.Set(1, 0.75f, 0.75f, 1.0f);


	sensor.transform.M[12] += offsetSensor.getX();
	sensor.transform.M[13] += offsetSensor.getY();
	sensor.transform.M[14] += offsetSensor.getZ();

	



	chassis.Render();
	chassis2.Render();
	chassis3.Render();
	chassis4.Render();
	chassis5.Render();
	//sensor.Render();
}

// ----------------------------------------------------------------------------
void PhysVehicle3D::ApplyEngineForce(float force)
{
	//for(int i = 0; i < vehicle->getNumWheels(); ++i)
	//{
	//	if(info.wheels[i].drive == true)
	//	{
	//		vehicle->applyEngineForce(force, i);
	//	}
	//}
	vehicle->applyEngineForce(force, 2);
	vehicle->applyEngineForce(force, 3);

}

// ----------------------------------------------------------------------------
void PhysVehicle3D::Brake(float force)
{
	for(int i = 0; i < vehicle->getNumWheels(); ++i)
	{
		if(info.wheels[i].brake == true)
		{
			vehicle->setBrake(force, i);
		}
	}

}

// ----------------------------------------------------------------------------
void PhysVehicle3D::Turn(float degrees)
{
	for(int i = 0; i < vehicle->getNumWheels(); ++i)
	{
		if(info.wheels[i].steering == true)
		{
			vehicle->setSteeringValue(degrees, i);
		}
	}
}

// ----------------------------------------------------------------------------
float PhysVehicle3D::GetKmh() const
{
	return vehicle->getCurrentSpeedKmHour();
}