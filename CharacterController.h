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

	CharacterController(int const objIndex, sf::RenderWindow* const window) : objIndex_(objIndex), window_(window) 
	{
		window_->setKeyRepeatEnabled(false);
		moveUp_ = false;
		moveDown_ = false;
		moveLeft_ = false;
		moveRight_ = false;
	}
	~CharacterController() {}

	int GetGameObjectIndex() const { return objIndex_; }

	bool HandleInput(GameObject &obj, float const deltaTime);

	bool ReceiveInput(GameObject &obj);
	sf::Vector2f CalculateDirection();
	sf::Angle CalculateRotation(sf::Vector2f objDirection);
	int CalculateAnimation();
	bool ShootCommand();
	void ReloadCommand();
	void ButtonClickCommand();

private:
	int const objIndex_;
	sf::RenderWindow* window_;

	//movement
	bool moveUp_, moveDown_, moveLeft_, moveRight_;

	//shooting/reloading
	int const characterMaxBullets = 6;
	float const timeBetweenShots = 0.5f;
	int characterCurrentBullets = 6;
	float timeSinceLastShot = 0.f;

	//animation
	CharacterState state_ = CharacterState::IDLING;
};