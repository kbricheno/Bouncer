#pragma once

#include <SFML/Graphics.hpp>
#include <stack>
#include <map>
#include "GameObject.h"

class VisualComponent {
public:
	VisualComponent(sf::RenderWindow* const window, 
		std::map<std::string, std::vector<sf::Texture>> const &animations,
		std::string startAnim = "idle")
		:
		window_(window),
		animations_(animations),
		startAnim_(startAnim)
	{}
	~VisualComponent() {}

	void Update(GameObject &obj, float const deltaTime);

private:
	sf::RenderWindow* window_;

	std::map<std::string, std::vector<sf::Texture>> animations_;
	std::array<sf::Sprite, 1> sprites_ = { sf::Sprite(animations_.begin()->second.front()) }; //there's only one sprite per entity but have to use a vector because you can't make an empty container for a single object

	int animationFrameRate_ = 10;
	float timeElapsedSinceLastFrame_ = 0;
	int currentFrame_ = 0;

	bool setup = true;
	std::string startAnim_;
	std::string previousAnimation_ = "idle";
};