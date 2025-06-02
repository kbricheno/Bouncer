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

	CharacterController(int const objId, sf::RenderWindow* const window) : objId_(objId), window_(window) 
	{
		window_->setKeyRepeatEnabled(false);
		moveUp_ = false;
		moveDown_ = false;
		moveLeft_ = false;
		moveRight_ = false;
	}
	~CharacterController() {}

	bool HandleInput(GameObject &obj, float const deltaTime);
	
	int const GetGameObjectId() const { return objId_; }

	bool ReceiveInput(GameObject &obj);
	sf::Vector2f CalculateDirection();
	sf::Angle CalculateRotation(sf::Vector2f objDirection);
	int CalculateAnimation();
	bool ShootCommand();
	void ReloadCommand();
	void ButtonClickCommand();

private:
	int objId_;
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
	float shootAnimationTimer = 0.f;
	float shootAnimationDuration = 0.2f;
	float reloadAnimationTimer = 0.f;
	float reloadAnimationDuration = 0.3f;
};