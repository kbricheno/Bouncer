#pragma once

#include "ControllerComponent.h"

class BulletController : public ControllerComponent {
public:
	BulletController(int const objIndex, sf::Vector2f startDir) : objIndex_(objIndex), startDir_(startDir) {}
	~BulletController() {}

	int GetGameObjectIndex() const { return objIndex_; }

	bool HandleInput(GameObject& obj, float const deltaTime);

private:
	int const objIndex_;

	sf::Vector2f const startDir_;
	float const speed_ = 1000.f;

	bool firstUpdate = true;
};