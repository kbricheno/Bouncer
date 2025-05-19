#include "GameObject.h"

GameObject::GameObject(sf::Vector2i position, VisualComponent* visualComponent) {
	position_ = position;
	visualComponent_ = visualComponent;

	dead_ = false;
}

GameObject::GameObject(sf::Vector2i position,
	sf::Vector2f direction,
	ControllerComponent* controllerComponent,
	PhysicsComponent* physicsComponent,
	VisualComponent* visualComponent,
	AudioComponent* audioComponent) 
{
	position_ = position;
	direction_ = direction;

	controllerComponent_ = controllerComponent;
	physicsComponent_ = physicsComponent;
	visualComponent_ = visualComponent;
	audioComponent_ = audioComponent;

	dead_ = false;
}