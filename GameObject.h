#pragma once
#include <SFML/Graphics.hpp>

class ControllerComponent;
class PhysicsComponent;
class VisualComponent;
class AudioComponent;

class GameObject {
public:
	GameObject(const sf::Vector2f position, const sf::Vector2f direction = { 0, 0 }, int visualComponentIndex = -1) : 
		position_(position), 
		direction_(direction), 
		visualComponentIndex_(visualComponentIndex) 
	{ dead_ = false; }

	int GetVisualComponentIndex() const { return visualComponentIndex_; }

	void SetComponents(//const ControllerComponent* controllerComponent = nullptr,
					   //const PhysicsComponent* physicsComponent = nullptr,
					   VisualComponent* visualComponent = nullptr
					   /*const AudioComponent* audioComponent = nullptr*/);

	sf::Vector2f GetPosition() const { return position_; }

private:
	bool dead_;

	sf::Vector2f position_;
	sf::Vector2f direction_;
	int visualComponentIndex_;

	ControllerComponent* controllerComponent_ = nullptr;
	PhysicsComponent* physicsComponent_ = nullptr;
	VisualComponent* visualComponent_ = nullptr;
	AudioComponent* audioComponent_ = nullptr;

};

