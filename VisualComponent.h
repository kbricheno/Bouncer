#pragma once

#include <SFML/Graphics.hpp>
#include "GameObject.h"

class VisualComponent {
public:
	VisualComponent(int const objId, sf::RenderWindow* const window, std::vector<std::vector<sf::Texture>> const &animations) :
		objId_(objId),
		window_(window),
		animations_(animations)
	{
		sf::Sprite sprite(animations.front().front());
		sprite.setOrigin(sprite.getLocalBounds().size / 2.f);
		sprites_.push_back(sprite);
	}
	~VisualComponent() {}

	int const GetGameObjectId() const { return objId_; }

	void Update(GameObject &obj, float const deltaTime);

private:
	int objId_;

	sf::RenderWindow* window_;

	std::vector<std::vector<sf::Texture>> animations_;
	std::vector<sf::Sprite> sprites_; //there's only one sprite per entity but have to use a vector because you can't make an empty container for an object
	int animationFrameRate = 10;
	float timeElapsedSinceLastFrame_ = 0;
	int lastAnimation_ = 0;
	int currentFrame_ = 0;
};