#pragma once
#include <SFML/System.hpp>
#include <SFML/Window.hpp>

#include "ControllerComponent.h"

class CharacterController : public ControllerComponent {
public:
	enum class CharacterState {
		IDLING,
		WALKING,
		SHOOTING,
		RELOADING
	};

	CharacterController(sf::RenderWindow* const window) : window_(window) 
	{
		window_->setKeyRepeatEnabled(false);
		moveUp_ = false;
		moveDown_ = false;
		moveLeft_ = false;
		moveRight_ = false;
	}
	~CharacterController() {}

	void Update(GameObject &obj, float const deltaTime);
	
	void SetMoveLeft(bool moving) { moveLeft_ = moving; }
	void SetMoveRight(bool moving) { moveRight_ = moving; }
	void SetMoveUp(bool moving) { moveUp_ = moving; }
	void SetMoveDown(bool moving) { moveDown_ = moving; }
	
	sf::Vector2f CalculateCharacterDirection(GameObject& obj);
	sf::Angle CalculateCharacterRotation(GameObject& obj);
	bool ValidateShootCommand(GameObject& obj);
	void Shoot(GameObject& obj);
	void Reload(GameObject& obj);

private:
	sf::RenderWindow* window_;

	//movement
	bool moveUp_, moveDown_, moveLeft_, moveRight_;

	//shooting/reloading
	int characterMaxBullets = 6;
	float timeBetweenShots = 0.5f;
	int characterCurrentBullets = 6;
	float timeSinceLastShot = 0.f;

	//animation
	CharacterState state_ = CharacterState::IDLING;
};