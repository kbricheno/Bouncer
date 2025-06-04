#include "PhysicsComponent.h"
#include <iostream>

void PhysicsComponent::Update(GameObject &obj, float const deltaTime, std::map<int, PhysicsComponent> const& allPhysicsComponents) {

	//perform specific behaviour depending on assigned type
	switch (obj.GetType())
	{
	case GameObject::EntityType::PLAYER:
		Move(obj, deltaTime, allPhysicsComponents);
		ResolveInteraction(obj, allPhysicsComponents);
		break;
	case GameObject::EntityType::ENEMY:
		Move(obj, deltaTime, allPhysicsComponents);
		ResolveInteraction(obj, allPhysicsComponents);
		break;
	case GameObject::EntityType::BULLET:
		Move(obj, deltaTime, allPhysicsComponents);
		ResolveInteraction(obj, allPhysicsComponents);
		break;
	case GameObject::EntityType::DOOR:
		ResolveInteraction(obj, allPhysicsComponents);
		break;
	default:
		break;
	}

	//set the collider position
	collider_.position = obj.GetColliderPosition();

	//set the hitbox position (hitboxes extend slightly further than colliders to ensure objects can interact)
	hitbox_.position = collider_.position + (collider_.size - hitbox_.size) / 2.f;	

	//set the center position used to draw the sprite (assuming this object has a VisualComponent)
	obj.SetCenter(collider_.position + (collider_.size / 2.f));
}

void PhysicsComponent::Move(GameObject& obj, float const deltaTime, std::map<int, PhysicsComponent> const& allPhysicsComponents) {
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

void PhysicsComponent::ResolveCollisions(GameObject &obj, bool xAxis, std::map<int, PhysicsComponent> const& allPhysicsComponents) {

	bool collisionDetected = false;

	//loop through all PhysicsComponents
	for (auto const& [id, otherPhysComp] : allPhysicsComponents)
	{		
		//conditions: only check for collisions with PhysicsComponents that have solid colliders, and don't check for collisions with yourself
		if (otherPhysComp.solid_ && otherPhysComp.collider_ != collider_)
		{
			//check for overlap with the other collider
			if (collider_.findIntersection(otherPhysComp.collider_))
			{
				collisionDetected = true;

				//create floats ready to push this collider out of the other collider
				float pushedOutX = collider_.position.x, pushedOutY = collider_.position.y;

				if (xAxis) //check collisions on each axis independently to avoid incorrectly overwriting a position on the other axis
				{
					if (obj.GetDirection().x > 0) // moving right
					{
						//TODO: update this when we swap to using rects' centers as their origins/positions
						pushedOutX = otherPhysComp.collider_.position.x - collider_.size.x;
					}
					else if (obj.GetDirection().x < 0) // moving left
					{
						pushedOutX = otherPhysComp.collider_.position.x + otherPhysComp.collider_.size.x;
					}

					//some entities have additional behaviour when they collide with a solid object
					if (obj.GetType() == GameObject::EntityType::BULLET || obj.GetType() == GameObject::EntityType::ENEMY)
					{
						obj.NotifyHorizontalCollision(true);
					}
				}

				else
				{
					if (obj.GetDirection().y > 0) // moving down
					{
						pushedOutY = otherPhysComp.collider_.position.y - collider_.size.y;
					}
					else if (obj.GetDirection().y < 0) // moving up
					{
						pushedOutY = otherPhysComp.collider_.position.y + otherPhysComp.collider_.size.y;
					}

					//some entities have additional behaviour when they collide with a solid object
					if (obj.GetType() == GameObject::EntityType::BULLET || obj.GetType() == GameObject::EntityType::ENEMY)
					{
						obj.NotifyVerticalCollision(true);
					}
				}
				//return the corrected position coordinates
				obj.SetColliderPosition({ pushedOutX, pushedOutY });
			}
		}
	}

	////loop through all PhysicsComponents
	//for (int i = 0; i < allPhysicsComponents.size(); i++)
	//{
	//	//conditions: only check for collisions with PhysicsComponents that have solid colliders, and don't check for collisions with yourself
	//	if (allPhysicsComponents[i].solid_ && allPhysicsComponents[i].collider_ != collider_)
	//	{
	//		sf::FloatRect otherCollider = allPhysicsComponents[i].collider_;

	//		//check for overlap with the other collider
	//		if (collider_.findIntersection(otherCollider))
	//		{
	//			collisionDetected = true;

	//			//create floats ready to push this collider out of the other collider
	//			float pushedOutX = collider_.position.x, pushedOutY = collider_.position.y;

	//			if (xAxis) //check collisions on each axis independently to avoid incorrectly overwriting a position on the other axis
	//			{
	//				if (obj.GetDirection().x > 0) // moving right
	//				{
	//					//TODO: update this when we swap to using rects' centers as their origins/positions
	//					pushedOutX = otherCollider.position.x - collider_.size.x;
	//				}
	//				else if (obj.GetDirection().x < 0) // moving left
	//				{
	//					pushedOutX = otherCollider.position.x + otherCollider.size.x;
	//				}

	//				//some entities have additional behaviour when they collide with a solid object
	//				if (obj.GetType() == GameObject::EntityType::BULLET || obj.GetType() == GameObject::EntityType::ENEMY)
	//				{
	//					obj.NotifyHorizontalCollision(true);
	//				}
	//			}

	//			else
	//			{
	//				if (obj.GetDirection().y > 0) // moving down
	//				{
	//					pushedOutY = otherCollider.position.y - collider_.size.y;
	//				}
	//				else if (obj.GetDirection().y < 0) // moving up
	//				{
	//					pushedOutY = otherCollider.position.y + otherCollider.size.y;
	//				}

	//				//some entities have additional behaviour when they collide with a solid object
	//				if (obj.GetType() == GameObject::EntityType::BULLET || obj.GetType() == GameObject::EntityType::ENEMY)
	//				{
	//					obj.NotifyVerticalCollision(true);
	//				}
	//			}
	//			//return the corrected position coordinates
	//			obj.SetColliderPosition({ pushedOutX, pushedOutY });
	//		}
	//	}
	//}

	//if there was no collision, return this collider's position
	if (!collisionDetected) obj.SetColliderPosition(collider_.position);
}

void PhysicsComponent::ResolveInteraction(GameObject &obj, std::map<int, PhysicsComponent> const& allPhysicsComponents) {
	for (auto const& [id, pComp] : allPhysicsComponents)
	{
		//condition: don't check for interactions with yourself
		if (pComp.hitbox_ != hitbox_)
		{
			if (hitbox_.findIntersection(pComp.hitbox_))
			{
				//better way to write a collision matrix?
				//identify this Component's entity type, identify the other Component's entity type, then resolve interaction
				switch (obj.GetType())
				{
				case GameObject::EntityType::BULLET:
					switch (pComp.type_)
					{
					case GameObject::EntityType::PLAYER:
						//if the bullet has bounced at least once (prevents triggering immediately on bullet spawn), destroy it & call game over
						if (obj.GetBulletBounceCount() > 0)
						{
							obj.Kill();
							//trigger game over
						}
						break;

					case GameObject::EntityType::ENEMY:
						obj.Kill();
						break;

					case GameObject::EntityType::DOOR:
						obj.Kill();
						break;

					default:
						break;
					}
					break;

				case GameObject::EntityType::ENEMY:
					switch (pComp.type_)
					{
					case GameObject::EntityType::PLAYER:
						//if the enemy is alive, the player is detected -> game over
						if (!obj.CheckHitByBullet())
						{
							//game over
						}
						break;

					case GameObject::EntityType::ENEMY:
						//if the other enemy is dead, this enemy detects the player's presence -> game over
						if (obj.CheckHitByBullet())
						{
							//game over
						}
						break;

					case GameObject::EntityType::BULLET:
						//"kill" the enemy without destroying it
						if (!obj.CheckHitByBullet())
						{
							obj.NotifyHitByBullet(true);
						}
						break;

					default:
						break;
					}
					break;

				case GameObject::EntityType::DOOR:
					switch (pComp.type_)
					{
					case GameObject::EntityType::BULLET:
						//switch animation, make collider non-solid so player can pass through
						obj.NotifySoundEvent(GameObject::SoundEvent::BULLET_COLLISION);
						obj.AddAnimationToStack(1, 0);
						solid_ = false;
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
	
	//for (int i = 0; i < allPhysicsComponents.size(); i++)
	//{
	//	//condition: don't check for interactions with yourself
	//	if (allPhysicsComponents[i].hitbox_ != hitbox_)
	//	{
	//		sf::FloatRect otherHitbox = allPhysicsComponents[i].hitbox_;

	//		if (hitbox_.findIntersection(otherHitbox)) 
	//		{
	//			//obtain the other hitbox's entity type
	//			GameObject::EntityType otherType = allPhysicsComponents[i].type_;

	//			//better way to write a collision matrix?
	//			//identify this Component's entity type, identify the other Component's entity type, then resolve interaction
	//			switch (obj.GetType())
	//			{
	//			case GameObject::EntityType::BULLET:
	//				switch (otherType)
	//				{
	//				case GameObject::EntityType::PLAYER:
	//					//if the bullet has bounced at least once (prevents triggering immediately on bullet spawn), destroy it & call game over
	//					if (obj.GetBulletBounceCount() > 0) 
	//					{
	//						obj.Kill();
	//						//trigger game over
	//					}
	//					break;

	//				case GameObject::EntityType::ENEMY:
	//					obj.Kill();
	//					break;

	//				case GameObject::EntityType::DOOR:
	//					obj.Kill();
	//					break;

	//				default:
	//					break;
	//				}
	//				break;

	//			case GameObject::EntityType::ENEMY:
	//				switch (otherType)
	//				{
	//				case GameObject::EntityType::PLAYER:
	//					//if the enemy is alive, the player is detected -> game over
	//					if (!obj.CheckHitByBullet())
	//					{
	//						//game over
	//					}
	//					break;

	//				case GameObject::EntityType::ENEMY:
	//					//if the other enemy is dead, this enemy detects the player's presence -> game over
	//					if (obj.CheckHitByBullet()) 
	//					{
	//						//game over
	//					}
	//					break;

	//				case GameObject::EntityType::BULLET:
	//					//"kill" the enemy without destroying it
	//					if (!obj.CheckHitByBullet()) 
	//					{
	//						obj.NotifyHitByBullet(true);
	//					}
	//					break;

	//				default:
	//					break;
	//				}
	//				break;

	//			case GameObject::EntityType::DOOR:
	//				switch (otherType)
	//				{
	//				case GameObject::EntityType::BULLET:
	//					//switch animation, make collider non-solid so player can pass through
	//					obj.NotifySoundEvent(GameObject::SoundEvent::BULLET_COLLISION);
	//					obj.AddAnimationToStack(1, 0);
	//					solid_ = false;
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
}