#pragma once

#include <SFML/Graphics.hpp>
#include <stack>
#include <map>
#include "GameObject.h"

class VisualComponent {
public:
	VisualComponent(sf::RenderWindow& const window, 
		std::map<std::string, std::vector<sf::Texture>> const &animations)
		:
		window_(window),
		animations_(animations)
	{
		sprites_.at(0).setOrigin(sprites_[0].getLocalBounds().size / 2.f); //set the sprite's origin to its center
	}
	~VisualComponent() {}

	void Update(GameObject &obj, float const deltaTime);

private:
	sf::RenderWindow& const window_;

	std::map<std::string, std::vector<sf::Texture>> const animations_;
	std::array<sf::Sprite, 1> sprites_ = { sf::Sprite(animations_.begin()->second.front()) }; //there's only one sprite per entity but have to use a vector because you can't make an empty container for a single object

	int const animationFrameRate_ = 10;
	float timeElapsedSinceLastFrame_ = 0;
	int currentFrame_ = 0;

	std::string previousAnimation_ = "idle";
};