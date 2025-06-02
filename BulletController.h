#pragma once

#include "ControllerComponent.h"

class BulletController : public ControllerComponent {
public:
	BulletController(int const objId) : objId_(objId) {}
	~BulletController() {}

	bool HandleInput(GameObject& obj, float const deltaTime);
	
	int const GetGameObjectId() const { return objId_; }

private:
	int objId_;

	int maxBounces_ = 2;
	int currentBounce_ = 0;
};