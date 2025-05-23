#pragma once

#include "GameObject.h"

class AudioComponent {
public:
	AudioComponent() {}
	~AudioComponent() {}

	void SetGameObject(GameObject* const obj) { obj_ = obj; }
	GameObject* GetGameObject() const { return obj_; }

	void Update();

private:
	GameObject* obj_ = nullptr;
};