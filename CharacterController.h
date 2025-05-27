#pragma once
#include <SFML/System.hpp>
#include <SFML/Window.hpp>

#include "ControllerComponent.h"

class CharacterController : public ControllerComponent {
public:
	enum class PlayerInput {
		W,
		A,
		S,
		D,
		LMB,
		R
	};

	CharacterController(int const objIndex, sf::RenderWindow* const window) : objIndex_(objIndex), window_(window) {}
	~CharacterController() {}

	int GetGameObjectIndex() const { return objIndex_; }
	void SetGameObject(GameObject* const obj) { obj_ = obj; }
	GameObject* GetGameObject() const { return obj_; }

	void Update();

	void DecodeInput();
	void StartMoveCommand(PlayerInput input);
	void StopMoveCommand(PlayerInput input);
	void ShootInput();
	void ReloadInput();
	void ButtonClickInput();

private:
	GameObject* obj_ = nullptr;

	int const objIndex_;
	sf::RenderWindow* window_;
};