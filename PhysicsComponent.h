#pragma once
#include <map>
#include <SFML/Graphics.hpp>

#include "GameObject.h"

class PhysicsComponent {
public:
	PhysicsComponent(GameObject::EntityType type, float const speed = 0.f) : speed_(speed)
	{
		//create a collider and/or hitbox for each type of physics-affected entity
		switch (type)
		{
		case GameObject::EntityType::CHARACTER:
			collider_ = sf::FloatRect({ 0,0 }, { 40,40 }); //presence of a collider means this object interacts with other colliders
			hitbox_ = sf::FloatRect({ 0,0 }, { 45,45 }); //presence of a hitbox means this object interacts with other hitboxes
			break;

		case GameObject::EntityType::ENEMY:
			collider_ = sf::FloatRect({ 0,0 }, { 50,50 });
			hitbox_ = sf::FloatRect({ 0,0 }, { 200,200 }); 
			secondaryHitbox_ = sf::FloatRect({ 0,0 }, { 60,60 }); //the secondary hitbox should always be the smaller one so we only need to check for secondary hitbox interactions when the primary hitbox is already intersecting with something
			break;

		case GameObject::EntityType::BULLET:
			collider_ = sf::FloatRect({ 0,0 }, { 3,3 });
			hitbox_ = sf::FloatRect({ 0,0 }, { 5,5 });
			break;

		case GameObject::EntityType::DOOR:
			collider_ = sf::FloatRect({ 0,0 }, { 100,100 });
			solid_ = true;
			hitbox_ = sf::FloatRect({ 0,0 }, { 100,100 });
			break;

		case GameObject::EntityType::WALL:
			collider_ = sf::FloatRect({ 0,0 }, { 100,100 });
			solid_ = true;
			break;

		default:
			break;
		}
	}
	~PhysicsComponent() {}

	void Update(GameObject &obj, float const deltaTime, std::map<int, GameObject>& allObjs, std::map<int, PhysicsComponent>& allPComps);

	//movement & collision checking
	void Move(GameObject& obj, float const deltaTime, std::map<int, PhysicsComponent>& allPComps);
	void ResolveCollisions(GameObject &obj, bool xAxis, std::map<int, PhysicsComponent>& allPComps);
	void ResolveInteractions(GameObject& obj, std::map<int, GameObject>& allObjs, std::map<int, PhysicsComponent>& allPComps);

private:	
	float speed_;

	sf::FloatRect collider_ = sf::FloatRect(); //colliders are used to handle physical (movement-altering) collisions
	bool solid_ = false; //solid determines whether this entity's collider prevents other colliders from moving into it
	sf::FloatRect hitbox_ = sf::FloatRect(); //hitboxes are used to handle interaction between entities
	sf::FloatRect secondaryHitbox_ = sf::FloatRect(); //some entities have a secondary hitbox to handle separate interactions (enemy vision radiuses)
};