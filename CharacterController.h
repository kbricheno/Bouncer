#pragma once
#include <SFML/System.hpp>
#include <SFML/Window.hpp>

#include "ControllerComponent.h"

class CharacterController : public ControllerComponent {
public:
	CharacterController(int const objIndex, sf::RenderWindow* const window) : objIndex_(objIndex), window_(window) 
	{
		window_->setKeyRepeatEnabled(false);
		moveUp_ = false;
		moveDown_ = false;
		moveLeft_ = false;
		moveRight_ = false;
	}
	~CharacterController() {}

	int GetGameObjectIndex() const { return objIndex_; }

	void Update(GameObject &obj);

	void HandleInput(GameObject &obj);
	void UpdateDirection(GameObject &objt);
	void ShootCommand();
	void ReloadCommand();
	void ButtonClickCommand();

private:
	int const objIndex_;

	sf::RenderWindow* window_;

	bool moveUp_, moveDown_, moveLeft_, moveRight_;
};