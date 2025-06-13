#include "Button.h"

Button::Button(std::map<std::string, sf::Texture> const& textures, sf::Vector2f const position) : textures_(textures)
{
	sprite_.setPosition(position);
}

Button::Button(std::map<std::string, sf::Texture> const& textures, sf::Vector2f const position, float const leftMostPos, float const rightMostPos) : textures_(textures), m_leftMostPos(leftMostPos), m_rightMostPos(rightMostPos)
{
	sprite_.setPosition(position);
}


bool Button::CheckIsHovered(sf::Vector2f const mousePos) {
	if (sprite_.getGlobalBounds().contains(mousePos)) 
	{
		//also update the Sprite's Texture, if it has a "hovered" Texture
		if (textures_.find("hovered") != textures_.end()) 
		{
			sprite_.setTexture(textures_.at("hovered"));
		}
		return true;
	}
	else 
	{ 
		sprite_.setTexture(textures_.at("unhovered"));
		return false; 
	}
}