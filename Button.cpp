#include "Button.h"

Button::Button(std::map<std::string, sf::Texture> const& inTextures, sf::Vector2f const inPos) : m_textures(inTextures)
{
	m_sprite.setPosition(inPos);
}

Button::Button(std::map<std::string, sf::Texture> const& inTextures, sf::Vector2f const inPos, float const inMinXPos, float const inMaxXPos) : m_textures(inTextures), m_minXPos(inMinXPos), m_maxXPos(inMaxXPos)
{
	m_sprite.setPosition(inPos);
}


bool Button::CheckIsHovered(sf::Vector2f const inMousePos) {
	if (m_sprite.getGlobalBounds().contains(inMousePos)) 
	{
		//also update the Sprite's Texture, if it has a "hovered" Texture
		if (m_textures.find("hovered") != m_textures.end()) 
		{
			m_sprite.setTexture(m_textures.at("hovered"));
		}
		return true;
	}
	else 
	{ 
		m_sprite.setTexture(m_textures.at("unhovered"));
		return false; 
	}
}