#include "BulletUtils.h"


BulletUtils::BulletUtils(void)
{
}


BulletUtils::~BulletUtils(void)
{
}

btTransform BulletUtils::btTransFrom(const btVector3 & origin, const btQuaternion & rot)
{
	btTransform ret;
	ret.setIdentity();
	ret.setRotation(rot);
	ret.setOrigin(origin);

	return ret;
}

btTransform BulletUtils::btTransFrom(const glm::mat4 & m)
{
	btMatrix3x3 mat(m[0].x, m[1].x, m[2].x,
		m[0].y, m[1].y, m[2].y,
		m[0].z, m[1].z, m[2].z);
	btVector3 vec(m[3].x, m[3].y, m[3].z);

	return btTransform(mat, vec);
}

glm::mat4 BulletUtils::glmMat4From(const btTransform & trans)
{
	btMatrix3x3 m = trans.getBasis();
	btVector3 v = trans.getOrigin();

	return glm::mat4(m[0].x(), m[1].x(), m[2].x(), 0,
		m[0].y(), m[1].y(), m[2].y(), 0,
		m[0].z(), m[1].z(), m[2].z(), 0,
		v.x(), v.y(), v.z(), 1);
}

btRigidBody * BulletUtils::createInvisibleBoxCollider(btScalar mass, btVector3 dimension, btVector3 origin, glm::mat4 m)
{
	btDefaultMotionState *motionstate;
	btTransform trans = BulletUtils::btTransFrom(m);
	trans.setOrigin(origin);
	motionstate = new btDefaultMotionState(trans);
	btVector3 inertia = btVector3(0, 0, 0);

	btCollisionShape *collisionShape;
	collisionShape = new btBoxShape(dimension);
	if (mass != 0.f)
	{
		collisionShape->calculateLocalInertia(mass, inertia);
	}

	btRigidBody::btRigidBodyConstructionInfo rbInfo
	(mass, motionstate, collisionShape, inertia);

	btRigidBody *rigidBody;
	rbInfo.m_friction = 0.1f;
	rigidBody = new btRigidBody(rbInfo);
	return rigidBody;
}
