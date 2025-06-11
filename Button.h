#pragma once
#include <map>
#include <SFML/Graphics.hpp>

class Button {
public:
	Button(std::map<std::string, sf::Texture> const& textures, sf::Vector2f position = sf::Vector2f()) : textures_(textures) 
	{
		sprite_[0].setPosition(position);
	}
	~Button() {}

	sf::Sprite GetSprite() { return sprite_[0]; }

	bool CheckIsHovered(sf::Vector2f const mousePos);

private:
	std::map<std::string, sf::Texture> const& textures_;
	std::array<sf::Sprite, 1> sprite_ = { sf::Sprite(textures_.at("unhovered"))};
};