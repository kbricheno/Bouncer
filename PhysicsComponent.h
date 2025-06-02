#pragma once

#include <SFML/Graphics.hpp>
#include "GameObject.h"

class PhysicsComponent {
public:
	enum class ColliderType {
		NONE,
		PLAYER,
		ENEMY,
		ENEMY_VISION,
		BULLET,
		DOOR,
		WALL
	};

	PhysicsComponent(int const objId, ColliderType const type, float const speed = 0.f) : objId_(objId), type_(type), speed_(speed)
	{
		//create a collider and/or hitbox for each type of physics-affected object
		switch (type_)
		{
		case ColliderType::PLAYER:
			solid_ = false; //determines whether this object's collider prevents other colliders from moving into it
			collider_ = sf::FloatRect({ 0,0 }, { 40,40 }); //presence of a collider means this object interacts with other colliders
			hitbox_ = sf::FloatRect({ 0,0 }, { 45,45 }); //presence of a hitbox means this object interacts with other hitboxes
			break;

		case ColliderType::ENEMY:
			solid_ = false;
			collider_ = sf::FloatRect({ 0,0 }, { 50,50 });
			hitbox_ = sf::FloatRect({ 0,0 }, { 75,75 });
			break;

		case ColliderType::ENEMY_VISION:
			solid_ = false;
			collider_ = sf::FloatRect();
			hitbox_ = sf::FloatRect({ 0,0 }, { 205,205 });
			break;

		case ColliderType::BULLET:
			solid_ = false;
			collider_ = sf::FloatRect({ 0,0 }, { 3,3 });
			hitbox_ = sf::FloatRect({ 0,0 }, { 5,5 });
			break;

		case ColliderType::DOOR:
			solid_ = true;
			collider_ = sf::FloatRect({ 0,0 }, { 100,100 });
			hitbox_ = sf::FloatRect({ 0,0 }, { 100,100 });
			break;

		case ColliderType::WALL:
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

	int const GetGameObjectId() const { return objId_; }
	ColliderType const GetType() const { return type_; }

	void Update(GameObject &obj, float const deltaTime, std::vector<PhysicsComponent> const &allPhysicsComponents);

	//movement & collision checking
	void Move(GameObject& obj, float const deltaTime, std::vector<PhysicsComponent> const &allPhysicsComponents);
	void ResolveCollisions(GameObject &obj, bool xAxis, std::vector<PhysicsComponent> const &allPhysicsComponents);
	//void ResolveInteraction(std::vector<PhysicsComponent> const &allColliders, sf::FloatRect hitbox);

	//getters

private:
	int objId_;

	ColliderType type_;
	bool solid_;
	sf::FloatRect collider_;
	sf::FloatRect hitbox_;
	float speed_;
};