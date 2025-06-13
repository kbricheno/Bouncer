#include "PhysicsComponent.h"
#include <iostream>

PhysicsComponent::PhysicsComponent(GameObject::EntityType type, float const speed) : speed_(speed) 
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
		m_isSolid = true;
		hitbox_ = sf::FloatRect({ 0,0 }, { 100,100 });
		break;

	case GameObject::EntityType::WALL:
		collider_ = sf::FloatRect({ 0,0 }, { 100,100 });
		m_isSolid = true;
		break;

	default:
		break;
	}
}


void PhysicsComponent::Update(GameObject &obj, float const deltaTime, std::map<int, GameObject> & allObjs, std::map<int, PhysicsComponent> & allPComps) {

	//set every entity's collider position
	collider_.position = obj.GetColliderPosition();

	//set every entity's hitbox position (hitboxes extend slightly further than colliders to ensure objects can interact)
	hitbox_.position = obj.GetColliderPosition() + (collider_.size - hitbox_.size) / 2.f;
	secondaryHitbox_.position = obj.GetColliderPosition() + (collider_.size - secondaryHitbox_.size) / 2.f;

	//set every entity's center position (used to draw the sprite if this object has a VisualComponent)
	obj.SetCenter(collider_.position + (collider_.size / 2.f));

	//some entities move and/or interact with other entities
	switch (obj.GetType())
	{
	case GameObject::EntityType::CHARACTER:
		Move(obj, deltaTime, allPComps);
		ResolveInteractions(obj, allObjs, allPComps);
		break;

	case GameObject::EntityType::ENEMY:
		Move(obj, deltaTime, allPComps);
		ResolveInteractions(obj, allObjs, allPComps);
		break;

	case GameObject::EntityType::BULLET:
		Move(obj, deltaTime, allPComps);
		ResolveInteractions(obj, allObjs, allPComps);
		break;

	case GameObject::EntityType::DOOR:
		ResolveInteractions(obj, allObjs, allPComps);
		break;

	default:
		break;
	}
}

void PhysicsComponent::Move(GameObject& obj, float const deltaTime, std::map<int, PhysicsComponent>& allPComps) {
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
		ResolveCollisions(obj, true, allPComps);
		collider_.position = { obj.GetColliderPosition().x, newPosY };
		ResolveCollisions(obj, false, allPComps);
	}
	else 
	{
		obj.SetColliderPosition({ newPosX, newPosY });
	}
}

void PhysicsComponent::ResolveCollisions(GameObject &obj, bool xAxis, std::map<int, PhysicsComponent>& allPComps) {

	bool collisionDetected = false;

	//loop through all PhysicsComponents
	for (auto const& [id, otherPhysComp] : allPComps)
	{		
		//conditions: only check for collisions with PhysicsComponents that have solid colliders, and don't check for collisions with yourself
		if (otherPhysComp.m_isSolid && otherPhysComp.collider_ != collider_)
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

	//if there was no collision, return this collider's position
	if (!collisionDetected) obj.SetColliderPosition(collider_.position);
}

//Detect intersections between hitboxes, perform interaction checks, then resolve the interaction
void PhysicsComponent::ResolveInteractions(GameObject& obj, std::map<int, GameObject>& allObjs, std::map<int, PhysicsComponent>& allPComps) {
	
	//iterate through all the PhysicsComponents
	for (auto& [id, pComp] : allPComps)
	{
		//don't check for interactions with yourself
		if (pComp.hitbox_ != hitbox_)
		{
			if (hitbox_.findIntersection(pComp.hitbox_))
			{
				/*
				When an intersection between any two hitboxes is detected, perform 4 actions:

				1) Identify this entity's type (ignoring any types that do not interact with anything).
				2) Identify the other entity's type (ignoring any types that do not interact with this entity).
				3) Check for conditions that enable an interaction between these entities to happen (i.e. *dead* enemies do not interact with bullets).
				4) Resolve the interaction for BOTH entities, usually by updating some state on their GameObjects.
				*/

				switch (obj.GetType())
				{
				case GameObject::EntityType::BULLET: //1) this entity's type is BULLET
					switch (allObjs.at(id).GetType())
					{
					case GameObject::EntityType::CHARACTER: //2) the other entity's type is CHARACTER
						if (obj.GetBulletBounceCount() > 0)	//3) check the condition: the bullet must have bounced at least once (prevents this from triggering on bullet spawn)
						{
							//4) destroy this entity (the bullet)
							obj.Destroy();

							//4) notify the other entity (character) it was hit by a bullet
							allObjs.at(id).NotifyHitByBullet(true);
						}					
						break;

					//a bullet is intersecting with an enemy
					case GameObject::EntityType::ENEMY:
						//check the enemy is alive (dead enemies and bullets do not interact at all)
						if (allObjs.at(id).CheckEntityAlive())
						{
							//an enemy's primary hitbox is its vision detection radius -- bullets do not interact with this vision hitbox
							//so only report interactions between a bullet and an enemy if the bullet has also intersected with the secondary hitbox (the enemy's actual body)
							if (hitbox_.findIntersection(pComp.secondaryHitbox_))
							{
								//destroy the bullet
								obj.Destroy();
							
								//notify the enemy it was hit by a bullet
								allObjs.at(id).NotifyHitByBullet(true);
							}
						}
						break;

					//a bullet is intersecting with a door
					case GameObject::EntityType::DOOR:
						//check if the door is solid (if the door is already broken, there is no interaction)
						if (pComp.m_isSolid) 
						{
							//destroy the bullet
							obj.Destroy();

							//tell the door to play a sound, switch its animation, and make its collider non-solid
							allObjs.at(id).NotifySoundEvent(GameObject::SoundEvent::BULLET_COLLISION);
							allObjs.at(id).AddAnimationToStack("break", 0);
							pComp.m_isSolid = false;
						}
						break;
					}
					break;

				case GameObject::EntityType::ENEMY:
					switch (allObjs.at(id).GetType())
					{
					//an enemy is intersecting with another enemy
					case GameObject::EntityType::ENEMY:
						//only handle interactions between enemies if this enemy is dead and its body (secondary hitbox) intersects with another enemy's vision radius (primary hitbox)
						if (!obj.CheckEntityAlive()) 
						{
							if (secondaryHitbox_.findIntersection(pComp.hitbox_)) 
							{
								//a dead body has been detected
								obj.SetBodyDetected();
							}
						}
						break;

					//an enemy is intersecting with the character
					case GameObject::EntityType::CHARACTER:
						//if this enemy is alive, kill the character
						if (obj.CheckEntityAlive()) 
						{
							allObjs.at(id).SetEntityDead();
						}
						break;
					}
					break;
				}
			}
		}
	}
}