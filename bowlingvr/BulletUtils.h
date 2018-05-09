#pragma once

#include <bullet/btBulletDynamicsCommon.h>
#include <glm/gtc/type_ptr.hpp>

//!  Static utility class. 
/*!
Handles conversions of Bullet related formats.
*/
class BulletUtils
{
public:
	BulletUtils(void);
	~BulletUtils(void);

	/**
	* Gets the Bullet btTransform from origin and quaternion.
	* @param origin vector.
	* @param rot rotation quaternion.
	* @return Bullet transform
	*/
	static btTransform btTransFrom(const btVector3 & origin, const btQuaternion & rot = btQuaternion(0, 0, 0));

	/**
	* Converts glm::mat4 to btTransform.
	* @param m glm matrix to be converted.
	* @return Bullet transform
	*/
	static btTransform btTransFrom(const glm::mat4 & m);

	/**
	* Converts btTransform to glm::mat4.
	* @param trans Bullet transform to be converted.
	* @return glm::mat4 matrix
	*/
	static glm::mat4 glmMat4From(const btTransform & trans);
	/**
	* A debug method creating invisible box rigid bodies.
	* @param mass of the body.
	* @param dimension of the edge.
	* @param origin for the body.
	* @param m model matrix, can be identity.
	* @return btRigidBody pointer
	*/
	static btRigidBody *createInvisibleBoxCollider(btScalar mass, btVector3 dimension, btVector3 origin, glm::mat4 m);
};