#include "PhysicsComponent.h"
#include <iostream>

void PhysicsComponent::Update(GameObject &obj, float const deltaTime, std::vector<PhysicsComponent> const &allColliders) {
	//perform specific behaviour depending on assigned type
	switch (type_)
	{
	case ColliderType::PLAYER:
		Move(obj, deltaTime, allColliders);
		break;
	case ColliderType::ENEMY:
		Move(obj, deltaTime, allColliders);
		break;
	case ColliderType::BULLET:
		Move(obj, deltaTime, allColliders);
		break;
	case ColliderType::DOOR:
		ResolveInteraction(allColliders);
		break;
	default:
		break;
	}

	collider_.position = obj.GetPosition();
	hitbox_.position = obj.GetPosition();
}

void PhysicsComponent::Move(GameObject& obj, float const deltaTime, std::vector<PhysicsComponent> const &allColliders) {
	//don't run move code if move input (direction) is empty
	if (obj.GetDirection() == sf::Vector2f(0, 0)) return;

	//for loop speed, set obj position to obj position + direction and perform collision detection
	for (int i = 0; i < speed_; i++)
	{
		//normalization used to prevent the object moving faster diagonally
		//deltaTime used to maintain the object's speed regardless of framerate
		float newPosX = obj.GetPosition().x + (obj.GetDirection().normalized().x * deltaTime);
		float newPosY = obj.GetPosition().y + (obj.GetDirection().normalized().y * deltaTime);

		//move collider using proposed positions, then immediately check collisions to validate position
		//check collisions on each axis independently to avoid incorrectly overwriting a position on the other axis
		collider_.position = { newPosX, obj.GetPosition().y};
		obj.SetPosition(ResolveCollisions(obj.GetDirection(), true, allColliders));
		collider_.position = { obj.GetPosition().x, newPosY };
		obj.SetPosition(ResolveCollisions(obj.GetDirection(), false, allColliders));

		ResolveInteraction(allColliders);
	}
}

sf::Vector2f PhysicsComponent::ResolveCollisions(sf::Vector2f direction, bool xAxis, std::vector<PhysicsComponent> const &allColliders) {

	bool collisionDetected = false;

	//loop through all colliders
	for (int i = 0; i < allColliders.size(); i++)
	{
		//check for overlap with this collider
		if (collider_.findIntersection(allColliders[i].GetCollider()))
		{
			//don't collide with yourself!
			if (allColliders[i].GetCollider() != collider_) 
			{
				collisionDetected = true;

				//create floats ready to push this collider out of the other collider
				float pushedOutX = collider_.position.x, pushedOutY = collider_.position.y;

				if (xAxis) //check collisions on each axis independently to avoid incorrectly overwriting a position on the other axis
				{
					if (direction.x > 0) // moving right
					{
						//TODO: update this when we swap to using rects' centers as their origins/positions
						pushedOutX = allColliders[i].GetCollider().position.x - collider_.size.x;
					}
					else if (direction.x < 0) // moving left
					{
						pushedOutX = allColliders[i].GetCollider().size.x;
					}
				}
			
				else 
				{
					if (direction.y > 0) // moving down
					{
						pushedOutY = allColliders[i].GetCollider().position.y - collider_.size.y;
					}
					else if (direction.y < 0) // moving up
					{
						pushedOutY = allColliders[i].GetCollider().size.y;
					}
				}

				//bullets have special additional behaviour
				if (type_ == ColliderType::BULLET)
				{
					//swap direction of movement if bullet hasn't already bounced twice
					//if hit left/right side, flip x direction
					//if hit top/bottom side, flip y direction
				}

				//return the corrected position coordinates
				return sf::Vector2f(pushedOutX, pushedOutY);
			}
		}
	}
	//if there was no collision, return this collider's position
	return collider_.position;
}

void PhysicsComponent::ResolveInteraction(std::vector<PhysicsComponent> const &allColliders) {
	for (int i = 0; i < allColliders.size(); i++)
	{
		//can't interact with any collider without a hitbox
		if (allColliders[i].GetHitbox() == sf::FloatRect({ 0,0 }, { 0, 0 })) return;

		if (hitbox_.findIntersection(allColliders[i].GetHitbox())) {
			ColliderType otherType = allColliders[i].type_;

			//better way to write a collision matrix?
			switch (type_)
			{
			case ColliderType::PLAYER:
				switch (otherType)
				{
				case ColliderType::ENEMY:
					//kill self
					break;
				case ColliderType::BULLET:
					//kill self
					break;
				default:
					break;
				}
				break;
			case ColliderType::BULLET:
				switch (otherType)
				{
				case ColliderType::PLAYER:
					//kill self
					break;
				case ColliderType::ENEMY:
					//kill self
					break;
				case ColliderType::DOOR:
					//kill self
					break;
				default:
					break;
				}
				break;
			case ColliderType::ENEMY:
				switch (otherType)
				{
				case ColliderType::ENEMY:
					//check if enemy is dead -> game over
					break;
				case ColliderType::BULLET:
					//kill self
					break;
				default:
					break;
				}
				break;
			case ColliderType::DOOR:
				switch (otherType)
				{
				case ColliderType::BULLET:
					//kill self
					break;
				default:
					break;
				}
				break;
			default:
				break;
			}
		}
	}
}