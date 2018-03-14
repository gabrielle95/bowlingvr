#pragma once

#include "BulletUtils.h"
#include "Model.h"


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

		this->collisionShape = new btConeShape(radius, height);

		btTransform trans = BulletUtils::btTransFrom(this->modelMatrix);
		//
		trans.setOrigin(origin);

		this->motionstate = new btDefaultMotionState(trans);
		btVector3 inertia = btVector3(0, 0, 0);

		if (mass != 0.f)
		{
			this->collisionShape->calculateLocalInertia(mass, inertia);
		}

		btRigidBody::btRigidBodyConstructionInfo rbInfo
		(mass, this->motionstate, collisionShape, inertia);

		rbInfo.m_friction = 0.9f;
		rbInfo.m_rollingFriction = 0.01f;
		rbInfo.m_spinningFriction = 0.01f;
		this->rigidBody = new btRigidBody(rbInfo);
	}
	~Pin()
	{

	}
};

class Alley : public Model
{
public:
	btConvexHullShape *convexHullShape;
	btTriangleMesh *trimesh;

	Alley(Shader * shader,
		const std::string& path
		//btScalar mass = btScalar(0)
		//btScalar radius = btScalar(0),
		//btScalar height = btScalar(0),
		//btVector3 origin = btVector3(0, 0, 0)
	) : Model(shader, path)
	{
	}
	Alley(Shader * shader,
		std::vector<Mesh*> meshEntries
		//btScalar mass = btScalar(0)
		//btVector3 origin = btVector3(0, 0, 0)
	) : Model(shader, meshEntries)
	{
	}
	void pInit(
		btScalar mass = btScalar(0),
		btVector3 origin = btVector3(0, 0, 0)
	)
	{
		btTransform trans = BulletUtils::btTransFrom(this->modelMatrix);
		trans.setOrigin(origin);
		this->motionstate = new btDefaultMotionState(trans);
		btVector3 inertia = btVector3(0, 0, 0);

		this->convexHullShape = new btConvexHullShape(0,0,0);
		this->trimesh = new btTriangleMesh();

		int msize = this->meshes.size();
		
		/*for (int i = 0; i < msize; i++)
		{
			int vsize = this->meshes[i]->vertices.size();
			for (int j = 0; j < vsize; j++)
			{
				this->convexHullShape->addPoint(
					btVector3(
						this->meshes[i]->vertices[j].Position.x,
						this->meshes[i]->vertices[j].Position.y,
						this->meshes[i]->vertices[j].Position.z));
				//trimesh->addTriangle(
				//btVector3(
				//this->meshes[i]->vertices[j].Position.x,
				//this->meshes[i]->vertices[j].Position.y,
				//this->meshes[i]->vertices[j].Position.z));
			}
		}*/

		for (int i = 0; i < msize; i++)
		{
			int isize = this->meshes[i]->indices.size();
			for (int j = 0; j <= isize-3; j = j + 3)
			{
				int i0 = this->meshes[i]->indices[j];
				int i1 = this->meshes[i]->indices[j + 1];
				int i2 = this->meshes[i]->indices[j + 2];

				btVector3 v0(meshes[i]->vertices[i0].Position.x, meshes[i]->vertices[i0].Position.y, meshes[i]->vertices[i0].Position.z);
				btVector3 v1(meshes[i]->vertices[i1].Position.x, meshes[i]->vertices[i1].Position.y, meshes[i]->vertices[i1].Position.z);
				btVector3 v2(meshes[i]->vertices[i2].Position.x, meshes[i]->vertices[i2].Position.y, meshes[i]->vertices[i2].Position.z);

				trimesh->addTriangle(v0, v1, v2);
			}
		}

		btCollisionShape* tmpshape = new btBvhTriangleMeshShape(trimesh, true);
		/*btConvexShape *tmpshape = new btConvexTriangleMeshShape();
		btShapeHull *hull = new btShapeHull(this->convexHullShape);
		btScalar margin = this->convexHullShape->getMargin();
		hull->buildHull(margin);
		this->convexHullShape->setUserPointer(hull);*/
		
		if (mass != 0.f)
		{
			tmpshape->calculateLocalInertia(mass, inertia);
		}

		btRigidBody::btRigidBodyConstructionInfo rbInfo
		(mass, this->motionstate, tmpshape, inertia);

		rbInfo.m_friction = 0.1f;
		rbInfo.m_rollingFriction = 0.01f;
		rbInfo.m_spinningFriction = 0.01f;
		this->rigidBody = new btRigidBody(rbInfo);
	}
};