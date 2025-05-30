#pragma once

#include <SFML/Graphics.hpp>
#include "GameObject.h"

class VisualComponent {
public:
	VisualComponent(int const objIndex, sf::RenderWindow* const window, std::vector<std::vector<sf::Texture>> const &animations, int const spriteIndex) :
		objIndex_(objIndex),
		window_(window),
		animations_(animations),
		spriteIndex_(spriteIndex)
	{}
	~VisualComponent() {}

	int const GetGameObjectIndex() const { return objIndex_; }
	int const GetSpriteIndex() const { return spriteIndex_; }

	void Update(GameObject &obj, float const deltaTime, sf::Sprite &sprite);

private:
	int const objIndex_;

	sf::RenderWindow* const window_;

	std::vector<std::vector<sf::Texture>> const animations_;
	int const spriteIndex_;
	int const animationFrameRate = 5;
	float timeElapsedSinceLastFrame_ = 0;
	int lastAnimation_ = 0;
	int currentFrame_ = 0;
};