#pragma once
#include <vector>

#include <SFML/Graphics.hpp>

class PhysicsComponent{
	enum class ColliderType {
		NONE,
		PLAYER,
		ENEMY,
		BULLET,
		DOOR,
		WALL
	};

public:
	PhysicsComponent(ColliderType type, sf::FloatRect hitbox) : type_(type), hitbox_(hitbox) {};
	~PhysicsComponent() {}

	void Update();

	const sf::FloatRect GetRect() { return hitbox_; }
	const ColliderType GetType() { return type_; }
	ColliderType ResolveCollisions(std::vector<PhysicsComponent*> otherColliders);

private:
	const ColliderType type_;
	const sf::FloatRect hitbox_;
};