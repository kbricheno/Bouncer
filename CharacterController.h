#pragma once
#include "ControllerComponent.h"

class CharacterController : public ControllerComponent {
public:
	CharacterController() {}
	~CharacterController() {}
	
	void Update();
};