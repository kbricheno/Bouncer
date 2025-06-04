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
	Level(sf::RenderWindow* const window, const int levelId, const int tileSize, const int levelWidth, const int levelHeight, const std::vector<char> &levelPlan);
	~Level() {}

	void GenerateTextures();
	bool GenerateSoundEffects();
	void GenerateLevel(const int tileSize, const int levelWidth, const int levelHeight, const std::vector<char>& levelPlan);

	void SpawnBullet(sf::Vector2f const startPos, sf::Vector2f const startDir);
	void CleanUpDeadObjects();

	void HandleInput(float const deltaTime);
	void Update(float const deltaTime);
	void Draw(float const deltaTime);

	//getters
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

	//textures
	std::vector<std::vector<sf::Texture>> characterAnimations_;
	std::vector<std::vector<sf::Texture>> enemyAnimations_;
	std::vector<std::vector<sf::Texture>> bulletAnimations_;
	std::vector<std::vector<sf::Texture>> wallAnimations_;
	std::vector<std::vector<sf::Texture>> doorHorAnimations_;
	std::vector<std::vector<sf::Texture>> doorVerAnimations_;
	std::vector<std::vector<sf::Texture>> backgroundAnimations_;

	//sounds
	std::map<std::string, sf::SoundBuffer> characterSoundEffects_;
	std::map<std::string, sf::SoundBuffer> enemySoundEffects_;
	std::map<std::string, sf::SoundBuffer> bulletSoundEffects_;
	std::map<std::string, sf::SoundBuffer> doorSoundEffects_;
	std::map<std::string, sf::SoundBuffer> globalSoundEffects;

};
