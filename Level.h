#pragma once

#include <vector>

#include <SFML/Graphics.hpp>

#include "GameObject.h"
#include "ControllerComponent.h"
#include "PhysicsComponent.h"
#include "VisualComponent.h"
#include "AudioComponent.h"

class Level {
public:
	Level(sf::RenderWindow* window, const int levelId, const int tileSize, const int levelWidth, const int levelHeight, const std::vector<char> levelPlan);

	const int GetLevelId() { return levelId_; }
	void Update();

private:	
	int levelId_;

	sf::RenderWindow* window_ = nullptr;

	std::vector<GameObject> gameObjects_;
	std::vector<ControllerComponent> controllerComponents_;
	std::vector<PhysicsComponent> physicsComponents_;
	std::vector<VisualComponent> visualComponents_;
	std::vector<AudioComponent> audioComponents_;

	std::vector<sf::Sprite> wallSprites_;
};