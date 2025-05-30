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
		int const colliderIndex,
		int const hitboxIndex = -1,
		float const speed = 0.f)
		:
		objIndex_(objIndex), 
		type_(type), 
		colliderIndex_(colliderIndex),
		hitboxIndex_(hitboxIndex),
		speed_(speed)
	{}
	~PhysicsComponent() {}

	int const GetGameObjectIndex() const { return objIndex_; }
	int const GetColliderIndex() const { return colliderIndex_; }
	int const GetHitboxIndex() const { return hitboxIndex_; }
	ColliderType const GetType() const { return type_; }

	void Update(GameObject &obj, float const deltaTime, std::vector<sf::FloatRect> &allColliders);

	//movement & collision checking
	void Move(GameObject& obj, float const deltaTime, std::vector<sf::FloatRect> &allColliders);
	sf::Vector2f ResolveCollisions(sf::Vector2f direction, bool xAxis, std::vector<sf::FloatRect> &allColliders);
	//void ResolveInteraction(std::vector<PhysicsComponent> const &allColliders, sf::FloatRect hitbox);

	//getters

private:
	int const objIndex_;

	ColliderType const type_;
	int const colliderIndex_;
	int const hitboxIndex_;
	int const speed_;
};