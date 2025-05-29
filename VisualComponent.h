#pragma once

#include <SFML/Graphics.hpp>
#include "GameObject.h"

class VisualComponent {
public:
	VisualComponent(int const objIndex, sf::RenderWindow* const window, std::vector<std::vector<sf::Texture>> const &animations, sf::Sprite const &sprite) :
		objIndex_(objIndex),
		window_(window),
		animations_(animations),
		sprite_(sprite)
	{}
	~VisualComponent() {}

	int GetGameObjectIndex() const { return objIndex_; }

	void Update(GameObject &obj, float const deltaTime);

private:
	int const objIndex_;

	sf::RenderWindow* const window_;

	std::vector<std::vector<sf::Texture>> const animations_;
	sf::Sprite sprite_;
	int const animationFrameRate = 5;
	float timeElapsedSinceLastFrame_ = 0;
	int lastAnimation_ = 0;
	int currentFrame_ = 0;
};