#pragma once

#include <bullet/btBulletDynamicsCommon.h>

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

