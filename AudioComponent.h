#pragma once

#include "GameObject.h"

class AudioComponent {
public:
	AudioComponent(int const objIndex) : objIndex_(objIndex) {}
	~AudioComponent() {}

	int GetGameObjectIndex() const { return objIndex_; }

	void Update(GameObject& obj);

private:
	int const objIndex_;
};