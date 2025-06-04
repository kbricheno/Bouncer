#pragma once

#include <map>
#include <SFML/Graphics.hpp>

#include "GameObject.h"

class PhysicsComponent {
public:
	PhysicsComponent(GameObject::EntityType type, float const speed = 0.f) : type_(type), speed_(speed)
	{
		//create a collider and/or hitbox for each type of physics-affected entity
		switch (type_)
		{
		case GameObject::EntityType::PLAYER:
			solid_ = false; //determines whether this object's collider prevents other colliders from moving into it
			collider_ = sf::FloatRect({ 0,0 }, { 40,40 }); //presence of a collider means this object interacts with other colliders
			hitbox_ = sf::FloatRect({ 0,0 }, { 45,45 }); //presence of a hitbox means this object interacts with other hitboxes
			break;

		case GameObject::EntityType::ENEMY:
			solid_ = false;
			collider_ = sf::FloatRect({ 0,0 }, { 50,50 });
			hitbox_ = sf::FloatRect({ 0,0 }, { 75,75 });
			break;

		case GameObject::EntityType::ENEMY_VISION:
			solid_ = false;
			collider_ = sf::FloatRect();
			hitbox_ = sf::FloatRect({ 0,0 }, { 205,205 });
			break;

		case GameObject::EntityType::BULLET:
			solid_ = false;
			collider_ = sf::FloatRect({ 0,0 }, { 3,3 });
			hitbox_ = sf::FloatRect({ 0,0 }, { 5,5 });
			break;

		case GameObject::EntityType::DOOR:
			solid_ = true;
			collider_ = sf::FloatRect({ 0,0 }, { 100,100 });
			hitbox_ = sf::FloatRect({ 0,0 }, { 100,100 });
			break;

		case GameObject::EntityType::WALL:
			solid_ = true;
			collider_ = sf::FloatRect({ 0,0 }, { 100,100 });
			hitbox_ = sf::FloatRect();
			break;

		default:
			solid_ = false;
			collider_ = sf::FloatRect();
			hitbox_ = sf::FloatRect();
			break;
		}
	}
	~PhysicsComponent() {}

	void Update(GameObject &obj, float const deltaTime, std::map<int, PhysicsComponent> const &allPhysicsComponents);

	//movement & collision checking
	void Move(GameObject& obj, float const deltaTime, std::map<int, PhysicsComponent> const& allPhysicsComponents);
	void ResolveCollisions(GameObject &obj, bool xAxis, std::map<int, PhysicsComponent> const& allPhysicsComponents);
	void ResolveInteraction(GameObject& obj, std::map<int, PhysicsComponent> const& allPhysicsComponents);

private:
	GameObject::EntityType type_; //keep a copy of the entity type
	bool solid_;
	sf::FloatRect collider_;
	sf::FloatRect hitbox_;
	float speed_;
};