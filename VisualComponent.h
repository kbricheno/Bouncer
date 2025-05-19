#pragma once
#include <vector>

#include <SFML/Graphics.hpp>

class VisualComponent {
public:
	VisualComponent(std::vector<sf::Sprite> sprites);
	~VisualComponent() {}

	sf::Sprite Update();

private:
	std::vector<sf::Sprite> sprites_;
};