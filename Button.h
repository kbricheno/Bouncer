#pragma once
#include <map>
#include <SFML/Graphics.hpp>

class Button {
public:
	//Constructor for a regular Button
	Button(std::map<std::string, sf::Texture> const& inTextures, sf::Vector2f const inPos = sf::Vector2f());
	//Constructor for a left-to-right bar slider Button
	Button(std::map<std::string, sf::Texture> const& inTextures, sf::Vector2f const inPos, float const inMinXPos, float const inMaxXPos);
	~Button() {}

	//Get the Button's Sprite for drawing and position altering
	sf::Sprite& const GetSprite() { return m_sprite; }

	//Get the X boundaries that the bar slider Button is allowed to move within
	float GetMinX() { return m_minXPos; }
	float GetMaxX() { return m_maxXPos; }

	//Check if the bounds of the Button contain the mouse's position
	bool CheckIsHovered(sf::Vector2f const inMousePos);

private:
	std::map<std::string, sf::Texture> const& m_textures; //a reference to the Textures used by this Button
	sf::Sprite m_sprite = sf::Sprite(m_textures.begin()->second); //sf::Sprite doesn't have a default constructor so just use any texture to construct it

	float m_minXPos = 0;
	float m_maxXPos = 0;
};