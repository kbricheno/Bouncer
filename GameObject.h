#pragma once
#include <SFML/Graphics.hpp>

#include "ControllerComponent.h"
#include "PhysicsComponent.h"
#include "VisualComponent.h"
#include "AudioComponent.h"

class GameObject {
public:
	GameObject(sf::Vector2i position, VisualComponent* visualComponent);
	GameObject(sf::Vector2i position,
		sf::Vector2f direction = sf::Vector2f(0, 0),
		ControllerComponent* controllerComponent = nullptr,
		PhysicsComponent* physicsComponent = nullptr,
		VisualComponent* visualComponent = nullptr,
		AudioComponent* audioComponent = nullptr);

private:
	bool dead_;

	sf::Vector2i position_;
	sf::Vector2f direction_;

	//std::vector<sf::Sprite> animations;
	//std::vector<sf::SoundBuffer> soundEffects;

	ControllerComponent* controllerComponent_;
	PhysicsComponent* physicsComponent_;
	VisualComponent* visualComponent_;
	AudioComponent* audioComponent_;

};