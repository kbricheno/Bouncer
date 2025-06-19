#include "PhysicsComponent.h"

PhysicsComponent::PhysicsComponent(GameObject::EntityType const inEntityType, float const inMoveSpeed) : m_speed(inMoveSpeed) 
{
	//create a collider and/or hitbox for each type of physics-affected entity
	switch (inEntityType)
	{
	case GameObject::EntityType::HERO:
		m_collider = sf::FloatRect({ 0,0 }, { 40,40 }); //presence of a collider means this object interacts with other colliders
		m_hitbox = sf::FloatRect({ 0,0 }, { 45,45 }); //presence of a hitbox means this object interacts with other hitboxes
		break;

	case GameObject::EntityType::ENEMY:
		m_collider = sf::FloatRect({ 0,0 }, { 50,50 });
		m_hitbox = sf::FloatRect({ 0,0 }, { 200,200 });
		m_secondaryHitbox = sf::FloatRect({ 0,0 }, { 60,60 }); //the secondary hitbox should always be the smaller one so we only need to check for secondary hitbox interactions when the primary hitbox is already intersecting with something
		break;

	case GameObject::EntityType::BULLET:
		m_collider = sf::FloatRect({ 0,0 }, { 3,3 });
		m_hitbox = sf::FloatRect({ 0,0 }, { 5,5 });
		break;

	case GameObject::EntityType::DOOR:
		m_collider = sf::FloatRect({ 0,0 }, { 100,100 });
		m_isSolid = true; //solid prevents other colliders from moving into this one
		m_hitbox = sf::FloatRect({ 0,0 }, { 100,100 });
		break;

	case GameObject::EntityType::WALL:
		m_collider = sf::FloatRect({ 0,0 }, { 100,100 });
		m_isSolid = true;
		break;
	}
}

void PhysicsComponent::Update(GameObject& obj, float const deltaTime, std::map<int, GameObject>& allObjsRef, std::map<int, PhysicsComponent>& allPCompsRef) {

	//set the entity's collider position
	m_collider.position = obj.GetColliderPosition();

	//set the entity's hitbox position(s) based on the center position on the collider
	m_hitbox.position = obj.GetColliderPosition() + (m_collider.size - m_hitbox.size) / 2.f;
	m_secondaryHitbox.position = obj.GetColliderPosition() + (m_collider.size - m_secondaryHitbox.size) / 2.f;

	//set the entity's center position (used to draw the Sprite if this entity has a VisualComponent)
	obj.SetCenter(m_collider.position + (m_collider.size / 2.f));

	//some entities move and/or interact with other entities; perform different behaviour depending on the entity's type
	switch (obj.GetType())
	{
	case GameObject::EntityType::HERO:
		Move(obj, deltaTime, allPCompsRef);
		ResolveInteractions(obj, allObjsRef, allPCompsRef);
		break;

	case GameObject::EntityType::ENEMY:
		Move(obj, deltaTime, allPCompsRef);
		ResolveInteractions(obj, allObjsRef, allPCompsRef);
		break;

	case GameObject::EntityType::BULLET:
		Move(obj, deltaTime, allPCompsRef);
		ResolveInteractions(obj, allObjsRef, allPCompsRef);
		break;

	case GameObject::EntityType::DOOR:
		ResolveInteractions(obj, allObjsRef, allPCompsRef);
		break;
	}
}

void PhysicsComponent::Move(GameObject& obj, float const deltaTime, std::map<int, PhysicsComponent>& allPCompsRef) {
	//don't run move code if direction is empty
	if (obj.GetDirection() == sf::Vector2f{ 0, 0 }) return;

	//calculate the entity's new/proposed position based on its current position, direction, and speed
	//normalization used to prevent the object moving faster diagonally
	//deltaTime used to maintain the object's speed regardless of framerate
	float newPosX = obj.GetColliderPosition().x + (obj.GetDirection().normalized().x * m_speed * deltaTime);
	float newPosY = obj.GetColliderPosition().y + (obj.GetDirection().normalized().y * m_speed * deltaTime);

	if (m_collider != sf::FloatRect()) //if the entity has a collider (aka collider does not equal a FloatRect of (0,0),(0,0))
	{
		//move this collider using proposed positions, then immediately check collisions to validate position
		//check collisions on each axis independently to avoid incorrectly overwriting a position on the other axis
		m_collider.position = { newPosX, obj.GetColliderPosition().y };
		ResolveCollisions(obj, true, allPCompsRef);
		m_collider.position = { obj.GetColliderPosition().x, newPosY };
		ResolveCollisions(obj, false, allPCompsRef);
	}
	else //if the entity doesn't have a collider, just move it to the proposed position
	{
		obj.SetColliderPosition({ newPosX, newPosY });
	}
}

void PhysicsComponent::ResolveCollisions(GameObject& obj, bool inIsValidatingXAxis, std::map<int, PhysicsComponent>& allPCompsRef) {
	//use a bool to keep track of whether or not a collision was detected
	bool collisionDetected = false;

	//loop through all PhysicsComponents
	for (auto const& [id, otherPhysComp] : allPCompsRef)
	{		
		//conditions: only check for collisions with PhysicsComponents that have solid colliders, and don't check for collisions with yourself
		if (otherPhysComp.m_isSolid && otherPhysComp.m_collider != m_collider)
		{
			//check for overlap with the other collider
			if (m_collider.findIntersection(otherPhysComp.m_collider))
			{
				collisionDetected = true;

				//create floats ready to push this collider out of the other collider
				float pushedOutX = m_collider.position.x, pushedOutY = m_collider.position.y;

				if (inIsValidatingXAxis) //check collisions on each axis independently to avoid incorrectly overwriting a position on the other axis
				{
					if (obj.GetDirection().x > 0) // moving right
					{
						pushedOutX = otherPhysComp.m_collider.position.x - m_collider.size.x;
					}
					else if (obj.GetDirection().x < 0) // moving left
					{
						pushedOutX = otherPhysComp.m_collider.position.x + otherPhysComp.m_collider.size.x;
					}

					//some entities have additional behaviour when they collide with a solid object
					if (obj.GetType() == GameObject::EntityType::BULLET || obj.GetType() == GameObject::EntityType::ENEMY)
					{
						obj.NotifyHorizontalCollisionEvent(true);
					}
				}

				else
				{
					if (obj.GetDirection().y > 0) // moving down
					{
						pushedOutY = otherPhysComp.m_collider.position.y - m_collider.size.y;
					}
					else if (obj.GetDirection().y < 0) // moving up
					{
						pushedOutY = otherPhysComp.m_collider.position.y + otherPhysComp.m_collider.size.y;
					}

					//some entities have additional behaviour when they collide with a solid object
					if (obj.GetType() == GameObject::EntityType::BULLET || obj.GetType() == GameObject::EntityType::ENEMY)
					{
						obj.NotifyVerticalCollisionEvent(true);
					}
				}
				//return the corrected position coordinates
				obj.SetColliderPosition({ pushedOutX, pushedOutY });
			}
		}
	}

	//if there was no collision, return this collider's position
	if (!collisionDetected) obj.SetColliderPosition(m_collider.position);
}

void PhysicsComponent::ResolveInteractions(GameObject& obj, std::map<int, GameObject>& allObjsRef, std::map<int, PhysicsComponent>& allPCompsRef) {
	
	//iterate through all the PhysicsComponents
	for (auto& [id, pComp] : allPCompsRef)
	{
		//don't check for interactions with yourself
		if (pComp.m_hitbox != m_hitbox)
		{
			if (m_hitbox.findIntersection(pComp.m_hitbox))
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
					switch (allObjsRef.at(id).GetType())
					{
					case GameObject::EntityType::HERO: //2) the other entity's type is HERO
						if (obj.GetBulletBounceCount() > 0)	//3) check the condition: the bullet must have bounced at least once (prevents this from triggering on bullet spawn)
						{
							//4) destroy this entity (the bullet)
							obj.TagForDestruction();

							//4) notify the other entity (hero) it was hit by a bullet
							allObjsRef.at(id).NotifyHitByBulletEvent(true);
						}					
						break;

					//a bullet is intersecting with an enemy
					case GameObject::EntityType::ENEMY:
						//check the enemy is alive (dead enemies and bullets do not interact at all)
						if (allObjsRef.at(id).IsEntityAlive())
						{
							//an enemy's primary hitbox is its vision detection radius -- bullets do not interact with this vision hitbox
							//so only report interactions between a bullet and an enemy if the bullet has also intersected with the secondary hitbox (the enemy's actual body)
							if (m_hitbox.findIntersection(pComp.m_secondaryHitbox))
							{
								//destroy the bullet
								obj.TagForDestruction();
							
								//notify the enemy it was hit by a bullet
								allObjsRef.at(id).NotifyHitByBulletEvent(true);
							}
						}
						break;

					//a bullet is intersecting with a door
					case GameObject::EntityType::DOOR:
						//check if the door is solid (if the door is already broken, there is no interaction)
						if (pComp.m_isSolid) 
						{
							//destroy the bullet
							obj.TagForDestruction();

							//tell the door to play a sound, switch its animation, and make its collider non-solid
							allObjsRef.at(id).NotifySoundEvent(GameObject::SoundEvent::BULLET_COLLISION);
							allObjsRef.at(id).AddAnimationToStack("break", 0);
							pComp.m_isSolid = false;
						}
						break;
					}
					break;

				case GameObject::EntityType::ENEMY:
					switch (allObjsRef.at(id).GetType())
					{
					//an enemy is intersecting with another enemy
					case GameObject::EntityType::ENEMY:
						//only handle interactions between enemies if this enemy is dead and its body (secondary hitbox) intersects with another enemy's vision radius (primary hitbox)
						if (!obj.IsEntityAlive()) 
						{
							if (m_secondaryHitbox.findIntersection(pComp.m_hitbox)) 
							{
								//a dead body has been detected
								obj.SetBodyDetected();
							}
						}
						break;

					//an enemy is intersecting with the hero
					case GameObject::EntityType::HERO:
						//if this enemy is alive, kill the hero
						if (obj.IsEntityAlive()) 
						{
							allObjsRef.at(id).SetEntityDead();
						}
						break;
					}
					break;
				}
			}
		}
	}
}