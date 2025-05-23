#pragma once

// abstract Controller interface; each GameObject with unique behaviour will have its own Controller
class ControllerComponent{
public:
	virtual ~ControllerComponent() {}
	virtual void Update() = 0;
};