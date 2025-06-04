#pragma once

#include <SFML/Graphics.hpp>
#include <stack>
#include "GameObject.h"

class VisualComponent {
public:
	VisualComponent(sf::RenderWindow* const window, 
		std::vector<std::vector<sf::Texture>> const &animations,
		int startAnim = 0) 
		:
		window_(window),
		animations_(animations),
		startAnim_(startAnim)
	{}
	~VisualComponent() {}

	void Update(GameObject &obj, float const deltaTime);

private:
	sf::RenderWindow* window_;

	std::vector<std::vector<sf::Texture>> animations_;
	std::array<sf::Sprite, 1> sprites_ = { sf::Sprite(animations_.front().front()) }; //there's only one sprite per entity but have to use a vector because you can't make an empty container for a single object

	int animationFrameRate_ = 10;
	float timeElapsedSinceLastFrame_ = 0;
	int currentFrame_ = 0;
	int previousAnimation_ = 0;

	bool setup = true;
	int startAnim_;
};