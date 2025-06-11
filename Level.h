#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <map>

#include "GameObject.h"
#include "ControllerComponent.h"
#include "PhysicsComponent.h"
#include "VisualComponent.h"
#include "AudioComponent.h"

class Level {
public:
	Level(sf::RenderWindow& const window,
		int const levelId, int const tileSize,
		std::vector<char> const &levelPlan,
		sf::Vector2i const levelSize,
		std::map<std::string, std::map<std::string, std::vector<sf::Texture>>> const &allAnimations,
		std::map<std::string, std::map<std::string, sf::SoundBuffer>> const &allSoundEffects)
		:
		levelId_(levelId),
		window_(window),
		levelSizePixels_(tileSize * levelSize),
		allAnimations_(allAnimations),
		allSoundEffects_(allSoundEffects)
	{
		GenerateLevel(tileSize, levelPlan, levelSize);
	}
	~Level() {}

	//game control (hud and progression variables)
	int GetLevelId() const { return levelId_; }
	//win state
	int GetEnemyCount() const { return enemiesAlive_; }
	//3 lose states
	bool CheckCharacterDetected() const { return characterDetected_; }
	bool CheckCharacterShot() const { return characterHitByBullet_; }
	bool CheckBodyDetected() const { return bodyDetected; }
	//hud text update functions
	std::string CurrentAmmoString();
	std::string EnemiesRemainingString();

	//level control (spawning and deleting objects, camera updating)
	void GenerateLevel(const int tileSize, const std::vector<char>& levelPlan, const sf::Vector2i levelSize);
	void SpawnBullet(sf::Vector2f const startPos, sf::Vector2f const startDir);
	void CleanUpDeadEntities();
	void UpdateView();

	//game loop
	void Update(float const deltaTime);
	void Draw(float const deltaTime);

	//user input commands
	void CommandMoveLeft(bool moving) { controllerComponents_.at(characterId).SetMoveLeft(moving); }
	void CommandMoveRight(bool moving) { controllerComponents_.at(characterId).SetMoveRight(moving); }
	void CommandMoveUp(bool moving) { controllerComponents_.at(characterId).SetMoveUp(moving); }
	void CommandMoveDown(bool moving) { controllerComponents_.at(characterId).SetMoveDown(moving); }
	void CommandShoot();
	void CommandReload();


private:
	int levelId_ = 0;
	sf::Vector2f levelSizePixels_;

	sf::RenderWindow& const window_;
	sf::View levelView;

	//counter for the number of GameObjects created over the course of the Level's existence
	//used to generate IDs for GameObjects and Components to identify each other
	//can't use the size of the GameObject vector because indexes get messed up as objects are destroyed
	int currentObjectId = 0;

	//keep a specific record of the player character's id so GameManager can pass user input commands to it easily
	int characterId = 0;

	//variables for game control & hud
	bool characterDetected_ = false;
	bool characterHitByBullet_ = false;
	bool bodyDetected = false;
	int startEnemies_ = 0;
	int enemiesAlive_ = 0;
	int ammoCount_ = 0;

	//maps used to store every GameObject and Component in the level
	std::map<int, GameObject> gameObjects_;
	std::map<int, ControllerComponent> controllerComponents_;
	std::map<int, PhysicsComponent> physicsComponents_;
	std::map<int, VisualComponent> visualComponents_;
	std::map<int, AudioComponent> audioComponents_;

	//reference to all textures
	std::map<std::string, std::map<std::string, std::vector<sf::Texture>>> const &allAnimations_;

	//reference to all sound effects
	std::map<std::string, std::map<std::string, sf::SoundBuffer>> const &allSoundEffects_;
};
