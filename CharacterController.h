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

	bool HandleInput(GameObject &obj, float const deltaTime);
	
	bool ReceiveInput(GameObject &obj);
	sf::Vector2f CalculateDirection(GameObject& obj);
	sf::Angle CalculateRotation(GameObject& obj);
	bool ShootCommand(GameObject& obj);
	void ReloadCommand(GameObject& obj);
	void ButtonClickCommand();

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