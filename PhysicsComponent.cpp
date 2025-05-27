#include "PhysicsComponent.h"
#include <iostream>

void PhysicsComponent::Update(float deltaTime, std::vector<PhysicsComponent>& const allColliders) {
	//perform specific behaviour depending on assigned type
	switch (type_)
	{
	case ColliderType::PLAYER:
		Move(deltaTime, allColliders);
		break;
	case ColliderType::ENEMY:
		Move(deltaTime, allColliders);
		break;
	case ColliderType::BULLET:
		Move(deltaTime, allColliders);
		break;
	case ColliderType::DOOR:
		ResolveInteraction(allColliders);
		break;
	default:
		break;
	}

	collider_.position = obj_->GetPosition();
	hitbox_.position = obj_->GetPosition();
}

void PhysicsComponent::Move(float deltaTime, std::vector<PhysicsComponent>& const allColliders) {
	//don't run move code if move input (direction) is empty
	if (obj_->GetDirection() == sf::Vector2f(0, 0)) return;

	//for loop speed, set obj position to obj position + direction and perform collision detection
	for (int i = 0; i < speed_; i++)
	{
		//normalization used to prevent the object moving faster diagonally
		//deltaTime used to maintain the object's speed regardless of framerate
		float newPosX = obj_->GetPosition().x + (obj_->GetDirection().normalized().x * deltaTime);
		float newPosY = obj_->GetPosition().y + (obj_->GetDirection().normalized().y * deltaTime);

		//move collider using proposed positions, then immediately check collisions to validate position
		//check collisions on each axis independently to avoid incorrectly overwriting a position on the other axis
		collider_.position = { newPosX, obj_->GetPosition().y};
		ResolveCollisions(true, allColliders);
		collider_.position = { obj_->GetPosition().x, newPosY };
		ResolveCollisions(false, allColliders);

		ResolveInteraction(allColliders);
	}
}

void PhysicsComponent::ResolveCollisions(bool xAxis, std::vector<PhysicsComponent>& const allColliders) {

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
					if (obj_->GetDirection().x > 0) // moving right
					{
						//TODO: update this when we swap to using rects' centers as their origins/positions
						pushedOutX = allColliders[i].GetCollider().position.x - collider_.size.x;
					}
					else if (obj_->GetDirection().x < 0) // moving left
					{
						pushedOutX = allColliders[i].GetCollider().size.x;
					}
				}
			
				else 
				{
					if (obj_->GetDirection().y > 0) // moving down
					{
						pushedOutY = allColliders[i].GetCollider().position.y - collider_.size.y;
					}
					else if (obj_->GetDirection().y < 0) // moving up
					{
						pushedOutY = allColliders[i].GetCollider().size.y;
					}
				}

				//set the corrected position coordinates on this Component's GameObject
				obj_->SetPosition({ pushedOutX, pushedOutY });

				//bullets have special additional behaviour
				if (type_ == ColliderType::BULLET)
				{
					//swap direction of movement if bullet hasn't already bounced twice
					//if hit left/right side, flip x direction
					//if hit top/bottom side, flip y direction
				}
			}
		}
	}
	//if there was no collision, update the position coordinates on this Component's GameObject with this collider's position
	if (!collisionDetected) obj_->SetPosition({ collider_.position.x, collider_.position.y });
}

void PhysicsComponent::ResolveInteraction(std::vector<PhysicsComponent>& const allColliders) {
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