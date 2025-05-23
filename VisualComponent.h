#pragma once

#include <SFML/Graphics.hpp>
#include "GameObject.h"

class VisualComponent {
public:
	VisualComponent(int objIndex, sf::RenderWindow* const window, std::vector<sf::Texture> const& textures, std::vector<sf::Sprite> const& sprites) :
		objIndex_(objIndex),
		window_(window),
		textures_(textures), 
		sprites_(sprites) 
	{}
	~VisualComponent() {}

	int GetGameObjectIndex() const { return objIndex_; }
	void SetGameObject(GameObject* obj) { obj_ = obj; }
	GameObject* GetGameObject() { return obj_; }

	void Update();

private:
	GameObject* obj_ = nullptr;

	int objIndex_;
	sf::RenderWindow* const window_;
	std::vector<sf::Texture> textures_;
	std::vector<sf::Sprite> sprites_;
};