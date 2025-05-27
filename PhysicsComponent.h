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
	void SetGameObject(GameObject* const obj) { obj_ = obj; }
	GameObject* GetGameObject() const { return obj_; }

	void Update(float deltaTime, std::vector<PhysicsComponent>& const allColliders);

	//movement & collision checking
	void Move(float deltaTime, std::vector<PhysicsComponent>& const allColliders);
	void ResolveCollisions(bool xAxis, std::vector<PhysicsComponent>& const allColliders);
	void ResolveInteraction(std::vector<PhysicsComponent>& const allColliders);

	//getters
	ColliderType const GetType() const { return type_; }
	sf::FloatRect GetCollider() const { return collider_; }
	sf::FloatRect GetHitbox() const { return hitbox_; }

private:
	GameObject* obj_ = nullptr;

	int const objIndex_;
	ColliderType const type_;
	sf::FloatRect collider_;
	sf::FloatRect hitbox_;
	int const speed_;
};