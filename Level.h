#pragma once

#include <SFML/Graphics.hpp>

#include "GameObject.h"
#include "CharacterController.h"
#include "BulletController.h"
#include "EnemyController.h"
#include "PhysicsComponent.h"
#include "VisualComponent.h"
#include "AudioComponent.h"

class Level {
public:
	Level(sf::RenderWindow* const window, const int levelId, const int tileSize, const int levelWidth, const int levelHeight, const std::vector<char> &levelPlan);
	~Level() {}

	void GenerateTexturesAndSprites();
	void GenerateLevel(const int tileSize, const int levelWidth, const int levelHeight, const std::vector<char>& levelPlan);

	void SpawnBullet(sf::Vector2f const startPos, sf::Vector2f const startDir);

	void HandleInput(float const deltaTime);
	void Update(float const deltaTime);
	void Draw(float const deltaTime);

	//getters
	int GetLevelId() const { return levelId_; }
	GameObject GetGameObject(int index) { return gameObjects_[index]; }
	CharacterController GetCharacterController(int index) { return characterControllers_[index]; }
	PhysicsComponent GetPhysicsComponent(int index) { return physicsComponents_[index]; }
	VisualComponent GetVisualComponent(int index) { return visualComponents_[index]; }
	AudioComponent GetAudioComponent(int index) { return audioComponents_[index]; }

private:
	int levelId_ = 0;

	sf::RenderWindow* window_ = nullptr;

	//counter for the number of GameObjects created over the course of the Level's existence
	//used to generate IDs for GameObjects and Components to identify each other
	//can't use the size of the GameObject vector because indexes get messed up as objects are destroyed
	int currentObjectId = 0;

	std::vector<GameObject> gameObjects_;
	std::vector<CharacterController> characterControllers_;
	std::vector<BulletController> bulletControllers_;
	std::vector<EnemyController> enemyControllers_;
	std::vector<PhysicsComponent> physicsComponents_;
	std::vector<VisualComponent> visualComponents_;
	std::vector<AudioComponent> audioComponents_;

	//textures
	std::vector<std::vector<sf::Texture>> characterAnimations_;
	std::vector<std::vector<sf::Texture>> enemyAnimations_;
	std::vector<std::vector<sf::Texture>> bulletAnimations_;
	std::vector<std::vector<sf::Texture>> wallAnimations_;
	std::vector<std::vector<sf::Texture>> doorAnimations_;
	std::vector<std::vector<sf::Texture>> backgroundAnimations_;
};
