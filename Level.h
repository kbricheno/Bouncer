#pragma once

#include <SFML/Graphics.hpp>

#include "GameObject.h"
#include "CharacterController.h"
#include "PhysicsComponent.h"
#include "VisualComponent.h"
#include "AudioComponent.h"

class Level {
public:
	Level(sf::RenderWindow* const window, const int levelId, const int tileSize, const int levelWidth, const int levelHeight, const std::vector<char> &levelPlan);
	~Level() {}

	void HandleInput();
	void Update(float deltaTime);
	void Draw();

	int GetLevelId() const { return levelId_; }
	GameObject GetGameObject(int index) { return gameObjects_[index]; }
	CharacterController GetCharacterController(int index) { return characterControllers_[index]; }
	PhysicsComponent GetPhysicsComponent(int index) { return physicsComponents_[index]; }
	VisualComponent GetVisualComponent(int index) { return visualComponents_[index]; }
	AudioComponent GetAudioComponent(int index) { return audioComponents_[index]; }

private:
	int levelId_ = 0;

	sf::RenderWindow* window_ = nullptr;

	std::vector<GameObject> gameObjects_;
	std::vector<CharacterController> characterControllers_;
	std::vector<PhysicsComponent> physicsComponents_;
	std::vector<VisualComponent> visualComponents_;
	std::vector<AudioComponent> audioComponents_;

	bool firstUpdate_;

	std::vector<sf::Texture> wallTextures_;
	std::vector<sf::Sprite> wallSprites_;
};