#pragma once
#include <map>
#include "GameObject.h"

class PhysicsComponent {
public:
	//Create a PhysicsComponent, passing in its entity type to generate an appropriate collider and/or hitbox(es)
	PhysicsComponent(GameObject::EntityType const inEntityType, float const inMoveSpeed = 0.f);
	~PhysicsComponent() {}

	//Update is called by the Level instance once per frame
	void Update(GameObject& obj, float const deltaTime, std::map<int, GameObject>& allObjsRef, std::map<int, PhysicsComponent>& allPCompsRef);

private:	
	//Use the entity's direction and speed to propose its new position, then call ResolveCollisions to validate this position
	void Move(GameObject& obj, float const deltaTime, std::map<int, PhysicsComponent>& allPCompsRef);

	//Check for intersections between this entity's collider and other solid colliders, pushing this entity out of the other if a collision is identified
	void ResolveCollisions(GameObject& obj, bool inIsValidatingXAxis, std::map<int, PhysicsComponent>& allPCompsRef);

	//Detect intersections between this entity's hitbox and other hitboxes, perform interaction checks, then resolve the interaction
	void ResolveInteractions(GameObject& obj, std::map<int, GameObject>& allObjsRef, std::map<int, PhysicsComponent>& allPCompsRef);

	float const m_speed; //speed that the entity should move at
	sf::FloatRect m_collider = sf::FloatRect(); //colliders are used to handle physical (movement-altering) collisions
	bool m_isSolid = false; //solid determines whether this entity's collider prevents other colliders from moving into it
	sf::FloatRect m_hitbox = sf::FloatRect(); //hitboxes are used to handle interaction between entities
	sf::FloatRect m_secondaryHitbox = sf::FloatRect(); //some entities have a secondary hitbox to handle separate interactions (enemy vision circles)
};