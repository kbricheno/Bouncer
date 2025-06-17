#pragma once
#include "GameObject.h"
#include "ControllerComponent.h"
#include "PhysicsComponent.h"
#include "VisualComponent.h"
#include "AudioComponent.h"

class Level {
public:
	Level(sf::RenderWindow& const inWindowRef,
		int const levelId, int const tileSize,
		std::vector<char> const &inLevelPlan,
		sf::Vector2i const inLevelSize,
		std::map<std::string, std::map<std::string, std::vector<sf::Texture>>>& const inAllAnimationsRef,
		std::map<std::string, std::map<std::string, sf::SoundBuffer>>& const inAllSoundBuffersRef)
		:
		m_levelId(levelId),
		m_windowRef(inWindowRef),
		m_levelSizePixels(tileSize * inLevelSize),
		m_allAnimationsRef(inAllAnimationsRef),
		m_allSoundBuffersRef(inAllSoundBuffersRef)
	{
		GenerateLevel(tileSize, inLevelPlan, inLevelSize);
	}
	~Level() {}


	//GAME CONTROL - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
	
	//Get the current Level number
	int GetLevelId() const { return m_levelId; }
	//Check the win condition
	bool AreAllEnemiesDead() const { return m_enemiesAlive == 0 ? true : false; }
	//Check the lose condition: has the hero been detected by a guard?
	bool IsHeroDetected() const { return m_isHeroDetected; }
	//Check the lose condition: has the hero been hit by a bullet?
	bool IsHeroShot() const { return m_isHeroHitByBullet; }
	//Check the lose condition: has a dead body been found by a guard?
	bool IsBodyDetected() const { return m_isBodyDetected; }
	
	//In-level HUD Text updaters
	std::string GetCurrentAmmoString() const { return "Ammo: " + std::to_string(m_ammoCount) + "/6"; }
	std::string GetEnemiesRemainingString() const { return "Enemies remaining: " + std::to_string(m_enemiesAlive); }


	//GAME LOOP - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
	
	//Update every behaviour-altering Component in the level
	void Update(float const deltaTime);
	//Play Sounds and draw every Sprite in the level
	void Draw(float const deltaTime);


	//USER INPUT COMMANDS - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
	
	void CommandMoveLeft(bool isMoving) { m_controllerComponents.at(m_heroId).SetHeroMoveLeft(isMoving); }
	void CommandMoveRight(bool isMoving) { m_controllerComponents.at(m_heroId).SetHeroMoveRight(isMoving); }
	void CommandMoveUp(bool isMoving) { m_controllerComponents.at(m_heroId).SetHeroMoveUp(isMoving); }
	void CommandMoveDown(bool isMoving) { m_controllerComponents.at(m_heroId).SetHeroMoveDown(isMoving); }
	//Ask the hero if it's okay to shoot
	void CommandShoot();
	//Ask the hero to reload their weapon
	void CommandReload();


private:
	int const m_levelId = 0; //Level instances are numbered so the GameManager can keep track of the game state
	sf::Vector2f const m_levelSizePixels; //total size of the current level in pixels
	sf::RenderWindow& const m_windowRef; //reference to the window so this Level can pass it to each VisualComponent to draw Sprites
	sf::View m_levelView; //the view used to display this Level's content


	//LEVEL CONTROL - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	//Create every GameObject and Component the level starts with
	void GenerateLevel(const int tileSize, const std::vector<char>& levelPlan, const sf::Vector2i levelSize);
	//Create a new bullet
	void SpawnBullet(sf::Vector2f const startPos, sf::Vector2f const startDir);
	//Remove "dead" GameObjects and their Components from storage maps
	void CleanUpDeadEntities();
	//Update the level view's position
	void UpdateView();


	//LEVEL GENERATION - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
	
	int m_currentObjectId = 0; //the number of GameObjects created over the course of the Level's existence, used by Components to identify the specific GameObject they affect
	int m_heroId = 0; //keep a specific record of the player hero's ID so the GameManager can pass user input commands to it easily

	std::map<std::string, std::map<std::string, std::vector<sf::Texture>>>& const m_allAnimationsRef; //reference to all Textures, passed to VisualComponents
	std::map<std::string, std::map<std::string, sf::SoundBuffer>>& const m_allSoundBuffersRef; //reference to all SoundBuffers, passed to AudioComponents

	//maps used to store every GameObject and Component in this Level (the int is the current object ID)
	std::map<int, GameObject> m_gameObjects;
	std::map<int, ControllerComponent> m_controllerComponents;
	std::map<int, PhysicsComponent> m_physicsComponents;
	std::map<int, VisualComponent> m_visualComponents;
	std::map<int, AudioComponent> m_audioComponents;


	//GAME CONTROL - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	//bools to identify the lose conditions
	bool m_isHeroDetected = false;
	bool m_isHeroHitByBullet = false;
	bool m_isBodyDetected = false;

	//ints to calculate the level cleared condition
	int m_startEnemies = 0;
	int m_enemiesAlive = 0;

	int m_ammoCount = 0; //keep track of the player's ammo so it can be used in a HUD Text object
};
