#pragma once
#include <SFML/Graphics.hpp>

class ControllerComponent;
class PhysicsComponent;
class VisualComponent;
class AudioComponent;

class GameObject {
public:
	GameObject(const sf::Vector2f position, 
		int const controllerComponentIndex = -1,
		int const physicsComponentIndex = -1,
		int const visualComponentIndex = -1,
		int const audioComponentIndex = -1)
		: 
		position_(position), 
		controllerComponentIndex_(controllerComponentIndex),
		physicsComponentIndex_(physicsComponentIndex),
		visualComponentIndex_(visualComponentIndex),
		audioComponentIndex_(audioComponentIndex)
	{}
	~GameObject() {}

	int GetControllerIndex() const { return controllerComponentIndex_; }
	int GetPhysicsIndex() const { return physicsComponentIndex_; }
	int GetVisualIndex() const { return visualComponentIndex_; }
	int GetAudioIndex() const { return audioComponentIndex_; }

	void SetComponents(ControllerComponent* const controllerComponent = nullptr,
					   PhysicsComponent* const physicsComponent = nullptr,
					   VisualComponent* const visualComponent = nullptr,
					   AudioComponent* const audioComponent = nullptr);

	sf::Vector2f GetPosition() const { return position_; }
	void SetPosition(sf::Vector2f position) { position_ = position; }

	sf::Vector2f GetDirection() const { return direction_; }
	void SetDirection(sf::Vector2f direction) { direction_ = direction; }

private:
	bool dead_ = false;

	sf::Vector2f position_;
	sf::Vector2f direction_ = { 0,0 };
	int const controllerComponentIndex_;
	int const physicsComponentIndex_;
	int const visualComponentIndex_;
	int const audioComponentIndex_;

	ControllerComponent* controllerComponent_ = nullptr;
	PhysicsComponent* physicsComponent_ = nullptr;
	VisualComponent* visualComponent_ = nullptr;
	AudioComponent* audioComponent_ = nullptr;

};

