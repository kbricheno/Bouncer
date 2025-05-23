#pragma once
#include <vector>

#include <SFML/Graphics.hpp>

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

	PhysicsComponent(ColliderType type, sf::FloatRect hitbox) : type_(type), hitbox_(hitbox) {};
	~PhysicsComponent() {}

	void Update();

	sf::FloatRect GetRect() const { return hitbox_; }
	ColliderType GetType() const { return type_; }
	//ColliderType ResolveCollisions(std::vector<PhysicsComponent*> otherColliders);

private:
	const ColliderType type_;
	const sf::FloatRect hitbox_;
};