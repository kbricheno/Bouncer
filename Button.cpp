#include "Button.h"

bool Button::CheckIsHovered(sf::Vector2f const mousePos) {
	if (sprite_[0].getGlobalBounds().contains(mousePos)) 
	{
		sprite_[0].setTexture(textures_.at("hovered"));
		return true;
	}
	else 
	{ 
		sprite_[0].setTexture(textures_.at("unhovered"));
		return false; 
	}
}