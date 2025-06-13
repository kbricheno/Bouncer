#pragma once
#include <map>
#include <SFML/Graphics.hpp>

class Button {
public:
	//Constructor for a regular Button
	Button(std::map<std::string, sf::Texture> const& textures, sf::Vector2f const position = sf::Vector2f());

	//Constructor for a left-to-right bar slider Button
	Button(std::map<std::string, sf::Texture> const& textures, sf::Vector2f const position, float const leftMostPos, float const rightMostPos);

	~Button() {}

	//get the sprite for drawing
	sf::Sprite GetSprite() { return sprite_; }

	//get the x boundaries that the bar slider Button is allowed to move within
	float GetMinX() { return m_leftMostPos; }
	float GetMaxX() { return m_rightMostPos; }

	//check if the bounds of the Button contain the mouse position
	bool CheckIsHovered(sf::Vector2f const mousePos);

private:
	std::map<std::string, sf::Texture> const& textures_;
	sf::Sprite sprite_ = sf::Sprite(textures_.at("unhovered"));

	float m_leftMostPos = 0;
	float m_rightMostPos = 0;
};