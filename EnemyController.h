#pragma once
#include "ControllerComponent.h"

class EnemyController : public ControllerComponent {
public:
	EnemyController(int const objId) : objId_(objId) 
	{
		srand(objId);
		currentDirection_ = directions_[rand() % 4];
	}
	~EnemyController() {}

	bool HandleInput(GameObject& obj, float const deltaTime);

	int const GetGameObjectId() const { return objId_; }

	sf::Vector2f ChangeDirection();

private:
	int objId_;

	bool alive_ = true;
	std::vector<sf::Vector2f> directions_ = { sf::Vector2f(1.f,0.f), sf::Vector2f(-1.f,0.f), sf::Vector2f(0.f,1.f), sf::Vector2f(0.f, -1.f) };
	sf::Vector2f currentDirection_;
	float randomTurnTimer_ = (rand() % 5) + 3;
	float stopTimer_ = (rand() % 2) + 1;
	bool turning_ = false;
};