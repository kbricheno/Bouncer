#include "PhysicsComponent.h"
#include <iostream>

void PhysicsComponent::Update(GameObject &obj, float const deltaTime, std::vector<PhysicsComponent> const &allPhysicsComponents) {

	//perform specific behaviour depending on assigned type
	switch (type_)
	{
	case ColliderType::PLAYER:
		Move(obj, deltaTime, allPhysicsComponents);
		break;
	case ColliderType::ENEMY:
		Move(obj, deltaTime, allPhysicsComponents);
		break;
	case ColliderType::BULLET:
		Move(obj, deltaTime, allPhysicsComponents);
		break;
	case ColliderType::DOOR:
		//ResolveInteraction(allColliders, hitbox);
		break;
	default:
		break;
	}

	//set the collider position
	collider_.position = obj.GetColliderPosition();

	//set the hitbox position (hitboxes extend slightly further than colliders to ensure objects can interact)
	hitbox_.position = collider_.position + (collider_.size - hitbox_.size) / 2.f;
	
	//set the GameObject's sprite position to be utilised by the VisualComponent (sprites' origins are their centers, so offset by half the collider's size)
	obj.SetSpritePosition(obj.GetColliderPosition() + (collider_.size / 2.f));
}

void PhysicsComponent::Move(GameObject& obj, float const deltaTime, std::vector<PhysicsComponent> const &allPhysicsComponents) {
	//don't run move code if move input (direction) is empty
	if (obj.GetDirection() == sf::Vector2f{ 0, 0 }) return;

	//normalization used to prevent the object moving faster diagonally
	//deltaTime used to maintain the object's speed regardless of frameratef
	float newPosX = obj.GetColliderPosition().x + (obj.GetDirection().normalized().x * speed_ * deltaTime);
	float newPosY = obj.GetColliderPosition().y + (obj.GetDirection().normalized().y * speed_ * deltaTime);

	if (collider_ != sf::FloatRect()) //if this object has a collider -- collider does not equal a FloatRect of (0,0),(0,0)
	{
		//move this collider using proposed positions, then immediately check collisions to validate position
		//check collisions on each axis independently to avoid incorrectly overwriting a position on the other axis
		collider_.position = { newPosX, obj.GetColliderPosition().y };
		ResolveCollisions(obj, true, allPhysicsComponents);
		collider_.position = { obj.GetColliderPosition().x, newPosY };
		ResolveCollisions(obj, false, allPhysicsComponents);
	}
	else 
	{
		obj.SetColliderPosition({ newPosX, newPosY });
	}

	//ResolveInteraction(allColliders, hitbox);
}

void PhysicsComponent::ResolveCollisions(GameObject &obj, bool xAxis, std::vector<PhysicsComponent> const &allPhysicsComponents) {

	bool collisionDetected = false;

	//loop through all PhysicsComponents
	for (int i = 0; i < allPhysicsComponents.size(); i++)
	{
		//conditions: only check for collisions with PhysicsComponents that have solid colliders, and don't check for collisions with yourself
		if (allPhysicsComponents[i].solid_ && allPhysicsComponents[i].collider_ != collider_)
		{
			sf::FloatRect otherCollider = allPhysicsComponents[i].collider_;

			//check for overlap with the other collider
			if (collider_.findIntersection(otherCollider))
			{
				collisionDetected = true;

				//create floats ready to push this collider out of the other collider
				float pushedOutX = collider_.position.x, pushedOutY = collider_.position.y;

				if (xAxis) //check collisions on each axis independently to avoid incorrectly overwriting a position on the other axis
				{
					if (obj.GetDirection().x > 0) // moving right
					{
						//TODO: update this when we swap to using rects' centers as their origins/positions
						pushedOutX = otherCollider.position.x - collider_.size.x;
					}
					else if (obj.GetDirection().x < 0) // moving left
					{
						pushedOutX = otherCollider.position.x + otherCollider.size.x;
					}

					//some entities have additional behaviour when they collide with a solid object
					if (type_ == ColliderType::BULLET || type_ == ColliderType::ENEMY)
					{
						obj.NotifyHorizontalCollision(true);
					}
				}

				else
				{
					if (obj.GetDirection().y > 0) // moving down
					{
						pushedOutY = otherCollider.position.y - collider_.size.y;
					}
					else if (obj.GetDirection().y < 0) // moving up
					{
						pushedOutY = otherCollider.position.y + otherCollider.size.y;
					}

					//some entities have additional behaviour when they collide with a solid object
					if (type_ == ColliderType::BULLET || type_ == ColliderType::ENEMY)
					{
						obj.NotifyVerticalCollision(true);
					}
				}
				//return the corrected position coordinates
				obj.SetColliderPosition({ pushedOutX, pushedOutY });
			}
		}
	}

	//if there was no collision, return this collider's position
	if (!collisionDetected) obj.SetColliderPosition(collider_.position);
}

//void PhysicsComponent::ResolveInteraction(std::vector<PhysicsComponent> const &allColliders, sf::FloatRect hitbox) {
//	for (int i = 0; i < allColliders.size(); i++)
//	{
//		//can't interact with any collider without a hitbox
//		if (allColliders[i].GetHitbox() == sf::FloatRect({ 0,0 }, { 0, 0 })) return;
//
//		if (hitbox_.findIntersection(allColliders[i].GetHitbox())) {
//			ColliderType otherType = allColliders[i].type_;
//
//			//better way to write a collision matrix?
//			switch (type_)
//			{
//			case ColliderType::PLAYER:
//				switch (otherType)
//				{
//				case ColliderType::ENEMY:
//					//kill self
//					break;
//				case ColliderType::BULLET:
//					//kill self
//					break;
//				default:
//					break;
//				}
//				break;
//			case ColliderType::BULLET:
//				switch (otherType)
//				{
//				case ColliderType::PLAYER:
//					//kill self
//					break;
//				case ColliderType::ENEMY:
//					//kill self
//					break;
//				case ColliderType::DOOR:
//					//kill self
//					break;
//				default:
//					break;
//				}
//				break;
//			case ColliderType::ENEMY:
//				switch (otherType)
//				{
//				case ColliderType::ENEMY:
//					//check if enemy is dead -> game over
//					break;
//				case ColliderType::BULLET:
//					//kill self
//					break;
//				default:
//					break;
//				}
//				break;
//			case ColliderType::DOOR:
//				switch (otherType)
//				{
//				case ColliderType::BULLET:
//					//kill self
//					break;
//				default:
//					break;
//				}
//				break;
//			default:
//				break;
//			}
//		}
//	}
//}