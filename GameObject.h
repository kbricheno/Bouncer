#pragma once
#include <stack>
#include <SFML/Graphics.hpp>

class GameObject {
public:
	//Enum for all of the types of entity in the game
	enum class EntityType {
		HERO,
		BULLET,
		ENEMY,
		WALL,
		DOOR,
		BACKGROUND
	};

	//Enum for each event that requires the AudioComponent associated with this GameObject to play a sound effect
	enum class SoundEvent {
		NONE,
		HERO_SHOOT,
		HERO_RELOAD,
		BULLET_COLLISION,
		SOLID_COLLISION,
		GAME_OVER,
		VICTORY
	};

	//Constructor for a standard GameObject
	GameObject(EntityType const inObjectType,
		sf::Vector2f const inStartPos,
		sf::Vector2f const inStartDir = sf::Vector2f(), //only necessary if the entity needs to start moving in a specific direction
		std::string const inStartAnim = "idle") //only necessary if the entity needs to use a special starting animation
		:
		m_entityType(inObjectType),
		m_colliderPos(inStartPos),
		m_direction(inStartDir)
	{
		AddAnimationToStack(inStartAnim, 0); //add the initial animation to the stack, tell it to loop
	}

	//Constructor for a GameObject that doesn't utilise a PhysicsComponent
	GameObject(EntityType const inObjectType,
		sf::Vector2f const inStartPos,
		sf::FloatRect const inSpriteBounds, //only necessary if the entity doesn't have a PhysicsComponent
		std::string const inStartAnim = "idle") //only necessary if the entity needs to use a special starting animation
		:
		m_entityType(inObjectType),
		m_colliderPos(inStartPos)
	{
		AddAnimationToStack(inStartAnim, 0); //add the initial animation to the stack, tell it to loop
		m_centerPos = m_colliderPos + inSpriteBounds.size / 2.f; //since this GameObject doesn't use a PhysicsComponent, set the center position (used to render its Sprite) once here
	}

	~GameObject() {}

	/*
	GameObjects exist purely to hold state that is modified and read by Components.

	Multiple different types of Components need to alter the behaviour of, update the position of, check the collisions of, 
	draw the Sprite of, and play Sounds from ONE specific entity. Each Component refers to a singular GameObject to determine 
	which entity it is managing, allowing Components to interact without being coupled together. 
	*/


	//CORE ENTITY STATE - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
	
	//Get the entity's type (e.g. enemy, wall, bullet) -- used to manage entity-specific behaviour
	EntityType const GetType() const { return m_entityType; }

	//Check if the entity needs to be destroyed this frame (all of its parts must be removed from the Level vectors)
	bool IsTaggedForDestruction() const { return m_isTaggedForDestruction; }
	//Set the entity as to-be-destroyed (all of its parts will be removed from the Level vectors next frame)
	void TagForDestruction() { m_isTaggedForDestruction = true; }


	//TRANSFORM STATE - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
	
	//Get the top-left position of the entity's collider
	sf::Vector2f GetColliderPosition() const { return m_colliderPos; }
	//Set the top-left position of the entity's collider
	void SetColliderPosition(sf::Vector2f inColliderPos) { m_colliderPos = inColliderPos; }

	//Get the center position of the entity's collider (Sprites are drawn at this position)
	sf::Vector2f GetCenter() const { return m_centerPos; }
	//Set the center position of the entity's collider (Sprites are drawn at this position)
	void SetCenter(sf::Vector2f inCenterPos) { m_centerPos = inCenterPos; }

	//Get the entity's current direction
	sf::Vector2f GetDirection() const { return m_direction; }
	//Set the entity's current direction
	void SetDirection(sf::Vector2f inDir) { m_direction = inDir; }

	//Get the entity's current rotation
	sf::Angle GetRotation() const { return m_rotation; }
	//Set the entity's current rotation
	void SetRotation(sf::Angle inRot) { m_rotation = inRot; }


	//ANIMATION STATE - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
	
	/*
	Getters and setters for the animation stack. I'm using a last-in-first-out system for animations, allowing new animations 
	to "overwrite" older ones for a time before popping them out to resume the old animation underneath. Almost every entity 
	in the game has a base "idle" animation that it will always need to return to.
	*/

	//Get the entity's currently-playing animation; the top value of the animation stack
	std::string GetCurrentAnimation() const { return m_animationStack.top(); }
	//Get the frame number that the entity's currently-playing animation will loop from
	int GetAnimationLoopFrame() const { return m_loopFrameStack.top(); }

	//Add an animation to the entity's animation stack: name, loop frame (-1 for no loop)
	void AddAnimationToStack(std::string inNewAnim, int inFrameToLoopFrom = -1) { m_animationStack.push(inNewAnim); m_loopFrameStack.push(inFrameToLoopFrom); }
	//Pop the currently-playing animation out of the animation stack
	void RemoveAnimationFromStack() { m_animationStack.pop(); m_loopFrameStack.pop(); }


	//SOUND STATE - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	//Get the entity's current sound event
	SoundEvent GetSoundEvent() const { return m_soundEvent; }
	//Notify the GameObject that an event requiring a sound effect has happened (will be set back to NULL as soon as the event is handled)
	void NotifySoundEvent(SoundEvent inSoundEvent) { m_soundEvent = inSoundEvent; }


	//UNIQUE STATE  - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	/*
	State utilised by only one or a handful of specific entities. In a more sophisticated system these would be stored in the 
	relevant derivatives of an abstract ControllerComponent class (e.g. m_bulletBounceCount would be stored in a BulletController
	subclass) and a messaging/event system would be used to notify the Controller of changes to this state.
	*/

	//(Enemies, bullets) Check if the entity has collided horizontally with another solid entity
	bool IsEntityCollidingHorizontally() const { return m_isCollidingHor; }
	//(Enemies, bullets) Notify the GameObject that the entity collided horizontally (will be set back to false as soon as the event is handled)
	void NotifyHorizontalCollisionEvent(bool inCollidedHor) { m_isCollidingHor = inCollidedHor; }
	//(Enemies, bullets) Check if the entity has collided vertically with another solid entity
	bool IsEntityCollidingVertically() const { return m_isCollidingVer; }
	//(Enemies, bullets) Notify the GameObject that the entity collided vertically (will be set back to false as soon as the event is handled)
	void NotifyVerticalCollisionEvent(bool inCollidedVer) { m_isCollidingVer = inCollidedVer; }

	//(Enemies, hero) Check if the entity has interacted with a bullet entity
	bool IsEntityHitByBullet() const { return m_isHitByBullet; }
	//(Enemies, hero) Notify the GameObject that the entity has interacted with a bullet entity (will be set back to false as soon as the event is handled)
	void NotifyHitByBulletEvent(bool inIsHitByBullet) { m_isHitByBullet = inIsHitByBullet; }

	//(Enemies, hero) Check if the entity is "alive" (these entities can be killed without being destroyed)
	bool IsEntityAlive() const { return m_isEntityAlive; }
	//(Enemies, hero) "Kill" the entity (does NOT destroy the entity!)
	void SetEntityDead() { m_isEntityAlive = false; }

	//(Enemies) Check if the entity is dead and has been detected by another enemy
	bool IsDeadEnemyDetected() const { return m_isDeadEnemyDetected; }
	//(Enemies) Inform the dead entity that it has been detected by another enemy
	void SetBodyDetected() { m_isDeadEnemyDetected = true; }

	//(Bullets) Get the number of times the entity has bounced
	int GetBulletBounceCount() const { return m_bulletBounceCount; }
	//(Bullets) Set the number of times the entity has bounced
	void SetBulletBounceCount(int inBulletBounceCount) { m_bulletBounceCount = inBulletBounceCount; }


private:

	//CORE ENTITY STATE - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
	EntityType const m_entityType; //e.g. enemy, wall, bullet -- used to manage entity-specific behaviour
	bool m_isTaggedForDestruction = false; //once an entity is tagged for destruction, its GameObject and Components will be removed from the Levels vector
	
	//TRANSFORM STATE - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
	sf::Vector2f m_colliderPos;	//updated by a PhysicsComponent based on m_direction
	sf::Vector2f m_centerPos; //updated by a PhysicsComponent (center of the collider) and used by a VisualComponent to draw a Sprite at its centered origin
	sf::Vector2f m_direction; //updated by a ControllerComponent based on player input or a collision-related event, used by a PhysicsComponent for movement
	sf::Angle m_rotation = sf::degrees(0); //0 for everything except the hero and enemies, updated by a ControllerComponent and used by a VisualComponent to rotate the Sprite

	//ANIMATION STATE - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
	std::stack<std::string> m_animationStack; //names of animations, updated by a ControllerComponent and used by a VisualComponent
	std::stack<int> m_loopFrameStack; //number of frame to loop animation from (-1 for no loop), updated by a ControllerComponent and used by a VisualComponent

	//SOUND STATE - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
	SoundEvent m_soundEvent = SoundEvent::NONE; //updated by a Controller or Physics Component based on input or an interaction, used by an AudioComponent to play Sounds

	//UNIQUE STATE  - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
	bool m_isCollidingHor = false; //updated by a PhysicsComponent, used by a ControllerComponent to define some behaviour when a collision happens
	bool m_isCollidingVer = false; //updated by a PhysicsComponent, used by a ControllerComponent to define some behaviour when a collision happens
	bool m_isHitByBullet = false; //updated by a PhysicsComponent, used by a ControllerComponent to define some behaviour when this entity is hit by a bullet
	int m_bulletBounceCount = 0; //updated by a ControllerComponent that determines whether the bullet should be destroyed
	bool m_isEntityAlive = true; //updated by a ControllerComponent that determines which behaviour the entity should have or whether the game is over
	bool m_isDeadEnemyDetected = false; //updated by a PhysicsComponent, used to determine game over
};

