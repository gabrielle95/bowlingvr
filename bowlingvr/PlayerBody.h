#pragma once

#include <bullet/btBulletDynamicsCommon.h>

//! Creates a player rigid body in the non-VR version. 
/*!
The player body can collide with objects and jump using space.
*/
class PlayerBody
{
public:
	btRigidBody * rigidBody;
	glm::mat4 modelMatrix;
	btMotionState * motionstate = nullptr;

	PlayerBody(btVector3 origin = btVector3(0, 0, 0));

private:	
	btCollisionShape *collisionShape;
};