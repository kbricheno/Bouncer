#pragma once

#include "GameObject.h"

class AudioComponent {
public:
	AudioComponent(int const objIndex) : objIndex_(objIndex) {}
	~AudioComponent() {}

	int GetGameObjectIndex() const { return objIndex_; }
	void SetGameObject(GameObject* const obj) { obj_ = obj; }
	GameObject* GetGameObject() const { return obj_; }

	void Update();

private:
	GameObject* obj_ = nullptr;

	int const objIndex_;
};