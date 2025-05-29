#pragma once

#include <SFML/Graphics.hpp>
#include "GameObject.h"

class PhysicsComponent{
public:
	enum class ColliderType {
		NONE,
		PLAYER,
		ENEMY,
		BULLET,
		DOOR,
		WALL
	};

	PhysicsComponent(int const objIndex,
		ColliderType const type,
		sf::FloatRect const collider,
		sf::FloatRect const hitbox = sf::FloatRect({ 0,0 }, { 0, 0 }),
		float const speed = 0.f)
		:
		objIndex_(objIndex), 
		type_(type), 
		collider_(collider),
		hitbox_(hitbox),
		speed_(speed)
	{}
	~PhysicsComponent() {}

	int GetGameObjectIndex() const { return objIndex_; }

	void Update(GameObject &obj, float const deltaTime, std::vector<PhysicsComponent> const &allColliders);

	//movement & collision checking
	void Move(GameObject& obj, float const deltaTime, std::vector<PhysicsComponent> const &allColliders);
	sf::Vector2f ResolveCollisions(sf::Vector2f direction, bool xAxis, std::vector<PhysicsComponent> const &allColliders);
	void ResolveInteraction(std::vector<PhysicsComponent> const &allColliders);

	//getters
	ColliderType const GetType() const { return type_; }
	sf::FloatRect GetCollider() const { return collider_; }
	sf::FloatRect GetHitbox() const { return hitbox_; }

private:
	int const objIndex_;

	ColliderType const type_;
	sf::FloatRect collider_;
	sf::FloatRect hitbox_;
	int const speed_;
};