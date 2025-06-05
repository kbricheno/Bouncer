#pragma once

#include "ControllerComponent.h"

class BulletController : public ControllerComponent {
public:
	BulletController() {}
	~BulletController() {}

	void Update(GameObject& obj, float const deltaTime);
	
private:
	int maxBounces_ = 2;
};