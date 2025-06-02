#pragma once
#include "GameObject.h"

// abstract Controller interface; each GameObject with unique behaviour will have its own Controller
class ControllerComponent{
public:
	virtual ~ControllerComponent() {}

	virtual bool HandleInput(GameObject& obj, float const deltaTime) = 0;
};