#pragma once
#include "GameObject.h"

class ControllerComponent {
public:
	enum class EnemyState {
		WALKING,
		PAUSED,
		DEAD
	};

	ControllerComponent(int const objId = 0)
	{
		moveUp_ = false;
		moveDown_ = false;
		moveLeft_ = false;
		moveRight_ = false;

		srand(objId);
		currentDirection_ = directions_[rand() % 4];
	}
	~ControllerComponent() {}

	void Update(GameObject &obj, float const deltaTime, sf::Vector2f mousePos = sf::Vector2f());
	
	//character-specific methods -----------------------------------------------------------------------------------------------------------------------------------------------
	void SetMoveLeft(bool moving) { moveLeft_ = moving; }
	void SetMoveRight(bool moving) { moveRight_ = moving; }
	void SetMoveUp(bool moving) { moveUp_ = moving; }
	void SetMoveDown(bool moving) { moveDown_ = moving; }
	
	sf::Vector2f CalculateCharacterDirection(GameObject& obj);
	sf::Angle CalculateCharacterRotation(GameObject& obj, sf::Vector2f mousePos);
	void CalculateCharacterAnimation(GameObject& obj, std::string anim = "");
	int ValidateShootCommand(GameObject& obj);
	int ValidateReloadCommand(GameObject& obj);
	void Shoot(GameObject& obj);
	void Reload(GameObject& obj);

	//enemy-specific methods ----------------------------------------------------------------------------------------------------------------------------------------------------
	EnemyState GetEnemyState() { return state_; }

	void UpdateEnemyState(GameObject& obj, float const deltaTime);
	void Stop(GameObject& obj);
	void Turn(GameObject& obj);
	void Die(GameObject& obj);
	void CalculateEnemyAnimation(GameObject& obj);

	//bullet-specific methods ---------------------------------------------------------------------------------------------------------------------------------------------------
	void BulletSolidCollision(GameObject& obj, const bool horizontal);

private:
	//character-specific fields -------------------------------------------------------------------------------------------------------------------------------------------------
	
	//movement
	bool moveUp_, moveDown_, moveLeft_, moveRight_;

	//shooting/reloading
	int characterMaxAmmo_ = 6;
	float timeBetweenShots_ = 0.5f;
	int characterCurrentAmmo_ = 6;
	float timeSinceLastShot_ = 0.f;


	//enemy-specific fields -----------------------------------------------------------------------------------------------------------------------------------------------------
	
	//movement
	std::vector<sf::Vector2f> directions_ = { sf::Vector2f(1.f,0.f), sf::Vector2f(-1.f,0.f), sf::Vector2f(0.f,1.f), sf::Vector2f(0.f, -1.f) };
	sf::Vector2f currentDirection_;
	float randomTurnTimer_ = 0;
	float pauseTimer_ = 0;
	
	//animation
	EnemyState state_ = EnemyState::PAUSED;
	

	//bullet-specific fields ----------------------------------------------------------------------------------------------------------------------------------------------------
	int maxBounces_ = 2;

};