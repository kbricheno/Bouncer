#pragma once
#include "ControllerComponent.h"

class EnemyController : public ControllerComponent {
public:
	EnemyController(int objId)
	{
		srand(objId);
		currentDirection_ = directions_[rand() % 4];
	}
	~EnemyController() {}

	bool HandleInput(GameObject& obj, float const deltaTime);

	sf::Vector2f ChangeDirection();

private:
	bool active_ = true;
	std::vector<sf::Vector2f> directions_ = { sf::Vector2f(1.f,0.f), sf::Vector2f(-1.f,0.f), sf::Vector2f(0.f,1.f), sf::Vector2f(0.f, -1.f) };
	sf::Vector2f currentDirection_;
	float randomTurnTimer_ = 0;
	float stopTimer_ = 0;
	bool turning_ = false;
};