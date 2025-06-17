#pragma once
#include "GameObject.h"

class ControllerComponent {
public:
	//Enum for the states an enemy can be in
	enum class EnemyState {
		WALKING,
		PAUSED,
		DEAD
	};

	//Create a ControllerComponent, passing in its object ID to seed random for enemy entities' direction choices
	ControllerComponent(int const inObjId = 0)
	{
		m_isHeroMovingUp = false;
		m_isHeroMovingDown = false;
		m_isHeroMovingLeft = false;
		m_isHeroMovingRight = false;

		srand(inObjId);
		m_currentEnemyDirection = m_enemyDirections[rand() % 4];
	}

	~ControllerComponent() {}

	//Update is called by the Level instance once per frame
	void Update(GameObject &obj, float const deltaTime, sf::Vector2f inMousePos = sf::Vector2f());

	void SetHeroMoveLeft(bool isMoving) { m_isHeroMovingLeft = isMoving; }
	void SetHeroMoveRight(bool isMoving) { m_isHeroMovingRight = isMoving; }
	void SetHeroMoveUp(bool isMoving) { m_isHeroMovingUp = isMoving; }
	void SetHeroMoveDown(bool isMoving) { m_isHeroMovingDown = isMoving; }

	//Command called from the Level instance -- check if the hero is able to shoot
	int ValidateShootCommand(GameObject& obj);
	//Command called from the Level instance -- make the hero reload
	int ValidateReloadCommand(GameObject& obj);

private:
	//HERO-SPECIFIC MEMBERS - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 

	//Update the hero GameObject's direction variable based on any input received earlier (keys pressed)
	sf::Vector2f CalculateHeroDirection(GameObject& obj);
	//Update the hero GameObject's rotation variable based on the mouse position and GameObject position
	sf::Angle CalculateHeroRotation(GameObject& obj, sf::Vector2f inMousePos);
	//Update the hero GameObject's animation stack based on input received earlier
	void CalculateHeroAnimation(GameObject& obj, std::string inAnimName = "");
	
	//Make the hero perform shooting behaviour (animation change, ammo update), does NOT spawn a bullet
	void HeroShoot(GameObject& obj);
	//Make the hero reload (animation change, ammo update)
	void HeroReload(GameObject& obj);

	//bools set by the GameManager during input handling and used to determine the hero's direction
	bool m_isHeroMovingUp, m_isHeroMovingDown, m_isHeroMovingLeft, m_isHeroMovingRight;

	int const m_heroMaxAmmo = 6;
	float const m_timeBetweenHeroShots = 0.5f; //prevents the hero from spam-shooting
	int m_heroCurrentAmmo = 6;
	float m_timeSinceHeroLastShot = 0.f;


	//ENEMY-SPECIFIC MEMBERS  - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
	
	//Update the enemy's current behavioural state
	void UpdateEnemyState(GameObject& obj, float const deltaTime);
	
	//Stop the enemy's movement
	void EnemyStop(GameObject& obj);
	//Change the enemy's direction and re-start its movement
	void EnemyTurn(GameObject& obj);
	//"Kill" the enemy
	void EnemyDie(GameObject& obj);

	//Update the enemy GameObject's animation stack based on what state the enemy is in
	void CalculateEnemyAnimation(GameObject& obj);

	EnemyState m_currentEnemyState = EnemyState::WALKING; //enemy entity's current behavioural state
	std::vector<sf::Vector2f> m_enemyDirections //directions enemy entities can move in
	{ 
		sf::Vector2f(1.f,0.f), 
		sf::Vector2f(-1.f,0.f), 
		sf::Vector2f(0.f,1.f), 
		sf::Vector2f(0.f, -1.f),
		sf::Vector2f(0.5f, 0.5f),
		sf::Vector2f(-0.5f, 0.5f),
		sf::Vector2f(0.5f, -0.5f),
		sf::Vector2f(-0.5f, -0.5f)
	};
	sf::Vector2f m_currentEnemyDirection; //current direction the enemy entity is moving in
	float m_enemyRandomTurnTimer = 0; //used to make enemy entities stop and turn occasionally, rather than just walking until they collide with something
	float m_enemyPauseTimer = 0; //used to make enemy entities stop moving for a random amount of time before switching directions


	//BULLET-SPECIFIC MEMBERS - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
	
	//Update the bullet's direction and bounce counter
	void BulletSolidCollision(GameObject& obj, const bool inIsCollisionHorizontal);

	int const m_maxBulletBounces = 2; //the number of times a bullet is allowed to bounce before it gets destroyed
};