#pragma once
#include <map>
#include <SFML/Graphics.hpp>

#include "GameObject.h"

class PhysicsComponent {
public:
	PhysicsComponent(GameObject::EntityType type, float const speed = 0.f);
	~PhysicsComponent() {}

	void Update(GameObject &obj, float const deltaTime, std::map<int, GameObject>& allObjs, std::map<int, PhysicsComponent>& allPComps);

	//movement & collision checking
	void Move(GameObject& obj, float const deltaTime, std::map<int, PhysicsComponent>& allPComps);
	void ResolveCollisions(GameObject &obj, bool xAxis, std::map<int, PhysicsComponent>& allPComps);
	void ResolveInteractions(GameObject& obj, std::map<int, GameObject>& allObjs, std::map<int, PhysicsComponent>& allPComps);

private:	
	float speed_;

	sf::FloatRect collider_ = sf::FloatRect(); //colliders are used to handle physical (movement-altering) collisions
	bool m_isSolid = false; //solid determines whether this entity's collider prevents other colliders from moving into it
	sf::FloatRect hitbox_ = sf::FloatRect(); //hitboxes are used to handle interaction between entities
	sf::FloatRect secondaryHitbox_ = sf::FloatRect(); //some entities have a secondary hitbox to handle separate interactions (enemy vision radiuses)
};