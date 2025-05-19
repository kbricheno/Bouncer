#include "Level.h"

#include <iostream>

Level::Level(sf::RenderWindow* window, const int levelId, const int tileSize, const int levelWidth, const int levelHeight, const std::vector<char> levelPlan) {
	levelId_ = levelId;

	sf::Texture wallTex_;
	wallTex_.loadFromFile("Assets/Environment/wall.png");
	sf::Sprite wallSprite_(wallTex_);
	wallSprites_.push_back(wallSprite_);


	int currentX = 0, currentY = 0;

	for (int i = 0; i < levelPlan.size() - 1; i++)
	{
		//std::cout << i << ": x = " << currentX << ", y = " << currentY << "\n";
		
		sf::Vector2i position(currentX * tileSize, currentY * tileSize);
		if (levelPlan[i] == 'W') {
			VisualComponent vc(wallSprites_);
			GameObject newWall(position, & vc);

			visualComponents_.push_back(vc);
			gameObjects_.push_back(newWall);
		}

		if (currentX >= levelWidth - 1) {
			currentY++;
			currentX = 0;
		}
		else currentX++;
	}
	std::cout << visualComponents_.size(); // 56
}

void Level::Update() {
	std::cout << visualComponents_.size(); // 0?

	for (int c = 0; c < controllerComponents_.size(); c++)
	{
		controllerComponents_[c].Update();
	}

	for (int p = 0; p < physicsComponents_.size(); p++)
	{
		physicsComponents_[p].Update();
	}

	for (int v = 0; v < visualComponents_.size(); v++)
	{
		window_->draw(visualComponents_[v].Update());
	}

	for (int a = 0; a < audioComponents_.size(); a++)
	{
		audioComponents_[a].Update();
	}
}