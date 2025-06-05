#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <map>

#include "GameObject.h"
#include "CharacterController.h"
#include "BulletController.h"
#include "EnemyController.h"
#include "PhysicsComponent.h"
#include "VisualComponent.h"
#include "AudioComponent.h"

class Level {
public:
	Level(sf::RenderWindow* const window,
		const int levelId, const int tileSize,
		const int levelWidth, const int levelHeight,
		const std::vector<char> &levelPlan,
		const std::map<std::string, std::map<std::string, std::vector<sf::Texture>>> &allAnimations,
		const std::map<std::string, std::map<std::string, sf::SoundBuffer>> &allSoundEffects)
		:
		levelId_(levelId),
		window_(window),
		allAnimations_(allAnimations),
		allSoundEffects_(allSoundEffects)
	{
		GenerateLevel(tileSize, levelWidth, levelHeight, levelPlan);
	}
	~Level() {}

	void GenerateLevel(const int tileSize, const int levelWidth, const int levelHeight, const std::vector<char>& levelPlan);

	void SpawnBullet(sf::Vector2f const startPos, sf::Vector2f const startDir);
	void CleanUpDeadObjects();

	//game loop
	void HandleInput(float const deltaTime);
	void Update(float const deltaTime);
	void Draw(float const deltaTime);

	int GetLevelId() const { return levelId_; }

private:
	int levelId_ = 0;

	sf::RenderWindow* window_ = nullptr;

	//counter for the number of GameObjects created over the course of the Level's existence
	//used to generate IDs for GameObjects and Components to identify each other
	//can't use the size of the GameObject vector because indexes get messed up as objects are destroyed
	int currentObjectId = 0;

	std::map<int, GameObject> gameObjects_;
	std::map<int, CharacterController> characterControllers_;
	std::map<int, BulletController> bulletControllers_;
	std::map<int, EnemyController> enemyControllers_;
	std::map<int, PhysicsComponent> physicsComponents_;
	std::map<int, VisualComponent> visualComponents_;
	std::map<int, AudioComponent> audioComponents_;

	//reference to all textures
	std::map<std::string, std::map<std::string, std::vector<sf::Texture>>> const &allAnimations_;

	//reference to all sound effects
	std::map<std::string, std::map<std::string, sf::SoundBuffer>> const &allSoundEffects_;
};
