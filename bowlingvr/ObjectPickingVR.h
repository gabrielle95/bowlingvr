#pragma once

#include <iostream>
#include <openvr/openvr.h>
#include <bullet/btBulletCollisionCommon.h>
#include <bullet/btBulletDynamicsCommon.h>

struct PickedBodyData
{
	uint32_t controllerIndex;
	btRigidBody *body;
	btVector3 pickPos;
	int savedActivationState;
	btPoint2PointConstraint *constraint;
	btScalar oldPickingDist;
	btScalar oldFriction;
};

//! This class takes care of object picking using Bullet constraints. 
/*!
Performs ray test and creates constraints to interact with dynamic bodies.
*/
class ObjectPickingVR
{
public:

	/**
	* Sets the camera rotation to a certain absolute value.
	* @param pHmd pointer to the VR system 
	* @param dynamicWorld pointer to the BulletPhysics world
	*/
	ObjectPickingVR(vr::IVRSystem *pHmd, btDynamicsWorld *dynamicWorld);
	/**
	* Picks up a body. Performs ray test and creates a constraint.
	* @param origin the origin of the raycast
	* @param direction the direction of the raycast
	*/
	bool pickBody(btVector3 origin, btVector3 direction);

	/**
	* Moves a picked a body. Manipulates the constraint appropriately.
	* @param origin the updated origin of the parent body
	* @param direction the direction of the parent body movement
	*/
	bool movePickedBody(btVector3 origin, btVector3 direction);
	void removePickingConstraint();

private:
	vr::IVRSystem * pHmd = NULL;
	btDynamicsWorld *dynamicWorld;
	PickedBodyData pickData;

};