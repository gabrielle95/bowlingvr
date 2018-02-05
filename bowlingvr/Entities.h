#pragma once

#include "BulletUtils.h"
#include "Model.h."

class Ball : public Model
{
public:
	Ball(
		Shader * shader,
		const std::string& path,
		btScalar mass = btScalar(0),
		btScalar radius = btScalar(0),
		btVector3 origin = btVector3(0, 0, 0)
		) : Model(shader, path)
	{
		pInit(mass, radius, origin);
	};

	Ball(
		Shader * shader,
		std::vector<Mesh*> meshEntries,
		btScalar mass = btScalar(0),
		btScalar radius = btScalar(0),
		btVector3 origin = btVector3(0, 0, 0)
		) : Model(shader, meshEntries) 
	{
		pInit(mass, radius, origin);
	};

	void pInit(
		btScalar mass = btScalar(0),
		btScalar radius = btScalar(0),
		btVector3 origin = btVector3(0, 0, 0)
	)
	{
		btTransform trans = BulletUtils::btTransFrom(this->modelMatrix);
		trans.setOrigin(origin);
		this->motionstate = new btDefaultMotionState(trans);
		btVector3 inertia = btVector3(0, 0, 0);

		this->collisionShape = new btSphereShape(radius);

		if (mass != 0.f)
		{
			this->collisionShape->calculateLocalInertia(mass, inertia);
		}

		btRigidBody::btRigidBodyConstructionInfo rbInfo
		(mass, this->motionstate, this->collisionShape, inertia);
		rbInfo.m_friction = 1.f;
		rbInfo.m_rollingFriction = 0.1f;
		rbInfo.m_spinningFriction = 0.1f;
		//rbInfo.m_linearDamping = 0.3f;
		this->rigidBody = new btRigidBody(rbInfo);
	}

	~Ball();
};

class Box : public Model
{
public:
	Box(Shader * shader,
		const std::string& path,
		btScalar mass = btScalar(0),
		btVector3 dimension = btVector3(0, 0, 0),
		btVector3 origin = btVector3(0, 0, 0)
		) : Model(shader, path)
	{
		pInit(mass, dimension, origin);
	}

	Box(Shader * shader,
		std::vector<Mesh*> meshEntries,
		btScalar mass = btScalar(0),
		btVector3 dimension = btVector3(0, 0, 0),
		btVector3 origin = btVector3(0, 0, 0)
	) : Model(shader, meshEntries)
	{
		pInit(mass, dimension, origin);
	}

	void pInit(
		btScalar mass = btScalar(0),
		btVector3 dimension = btVector3(0, 0, 0),
		btVector3 origin = btVector3(0, 0, 0)
	)
	{
		btTransform trans = BulletUtils::btTransFrom(this->modelMatrix);
		trans.setOrigin(origin);
		this->motionstate = new btDefaultMotionState(trans);
		btVector3 inertia = btVector3(0, 0, 0);
		this->collisionShape = new btBoxShape(dimension);
		if (mass != 0.f)
		{
			this->collisionShape->calculateLocalInertia(mass, inertia);
		}


		btRigidBody::btRigidBodyConstructionInfo rbInfo
		(mass, this->motionstate, this->collisionShape, inertia);

		rbInfo.m_friction = 0.1f;
		this->rigidBody = new btRigidBody(rbInfo);

	}
	~Box();
};

class Pin : public Model
{
public:
	Pin(Shader * shader,
		const std::string& path,
		btScalar mass = btScalar(0),
		btScalar radius = btScalar(0),
		btScalar height = btScalar(0),
		btVector3 origin = btVector3(0, 0, 0)
	) : Model(shader, path)
	{
		pInit(mass, radius, height, origin);
	}
	Pin(Shader * shader,
		std::vector<Mesh*> meshEntries,
		btScalar mass = btScalar(0),
		btScalar radius = btScalar(0),
		btScalar height = btScalar(0),
		btVector3 origin = btVector3(0, 0, 0)
	) : Model(shader, meshEntries)
	{
		pInit(mass, radius, height, origin);
	}
	void pInit(
		btScalar mass = btScalar(0),
		btScalar radius = btScalar(0),
		btScalar height = btScalar(0),
		btVector3 origin = btVector3(0, 0, 0)
	) //let's try cone for now
	{
		btTransform trans = BulletUtils::btTransFrom(this->modelMatrix);
		trans.setOrigin(origin);
		this->motionstate = new btDefaultMotionState(trans);
		btVector3 inertia = btVector3(0, 0, 0);

		this->collisionShape = new btConeShape(radius, height);
		if (mass != 0.f)
		{
			this->collisionShape->calculateLocalInertia(mass, inertia);
		}

		btRigidBody::btRigidBodyConstructionInfo rbInfo
		(mass, this->motionstate, this->collisionShape, inertia);

		rbInfo.m_friction = 0.9f;
		rbInfo.m_rollingFriction = 0.01f;
		rbInfo.m_spinningFriction = 0.01f;
		this->rigidBody = new btRigidBody(rbInfo);
	}
	~Pin()
	{

	}
};