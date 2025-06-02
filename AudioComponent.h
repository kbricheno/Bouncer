#pragma once

#include "GameObject.h"

class AudioComponent {
public:
	AudioComponent(int const objId) : objId_(objId) {}
	~AudioComponent() {}

	int GetGameObjectId() const { return objId_; }

	void Update(GameObject& obj);

private:
	int objId_;
};