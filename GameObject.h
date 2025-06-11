#pragma once
#include <stack>
#include <SFML/Graphics.hpp>

class GameObject {
public:
	enum class EntityType {
		CHARACTER,
		BULLET,
		ENEMY,
		WALL,
		DOOR,
		BACKGROUND
	};

	enum class SoundEvent {
		NONE,
		CHARACTER_SHOOT,
		CHARACTER_RELOAD,
		BULLET_COLLISION,
		SOLID_COLLISION,
		GAME_OVER,
		VICTORY
	};

	GameObject(EntityType type,
		sf::Vector2f const position,
		sf::Vector2f const spriteSize = sf::Vector2f(), //only necessary if the entity doesn't have a PhysicsComponent
		std::string startAnim = "idle", //only necessary if the entity needs to use a special starting animation
		sf::Vector2f const direction = sf::Vector2f()) //only necessary if the entity needs to start moving in a specific direction
		:
		type_(type),
		colliderPosition_(position),
		direction_(direction)
	{
		//set up some starting values that are identical across all GameObjects
		centerPosition_ = colliderPosition_ + spriteSize / 2.f;
		AddAnimationToStack(startAnim, 0);
	}
	~GameObject() {}

	//getters and setters for variables used by Components
	EntityType const GetType() const { return type_; }

	sf::Vector2f GetColliderPosition() const { return colliderPosition_; }
	void SetColliderPosition(sf::Vector2f colliderPosition) { colliderPosition_ = colliderPosition; }

	sf::Vector2f GetCenter() const { return centerPosition_; }
	void SetCenter(sf::Vector2f centerPosition) { centerPosition_ = centerPosition; }

	sf::Vector2f GetDirection() const { return direction_; }
	void SetDirection(sf::Vector2f direction) { direction_ = direction; }

	sf::Angle GetRotation() const { return rotation_; }
	void SetRotation(sf::Angle rotation) { rotation_ = rotation; }

	std::string GetCurrentAnimation() const { return animationStack_.top(); }
	std::stack<std::string> GetStack() const { return animationStack_; }
	int GetAnimationLoopFrame() const { return loopFrameStack_.top(); }
	void AddAnimationToStack(std::string newAnim, int frameToLoopFrom = -1) { animationStack_.push(newAnim); loopFrameStack_.push(frameToLoopFrom); }
	void RemoveAnimationFromStack() { animationStack_.pop(); loopFrameStack_.pop(); }

	bool CheckTaggedForDestruction() const { return dead_; }
	void Destroy() { dead_ = true; }

	SoundEvent CheckSoundEvent() const { return currentSound_; }
	void NotifySoundEvent(SoundEvent currentSound) { currentSound_ = currentSound; }


	//entity-specific

	bool CheckHorizontalCollision() const { return collidedHorizontally_; }
	void NotifyHorizontalCollision(bool collidedHor) { collidedHorizontally_ = collidedHor; }
	bool CheckVerticalCollision() const { return collidedVertically_; }
	void NotifyVerticalCollision(bool collidedVer) { collidedVertically_ = collidedVer; }

	bool CheckHitByBullet() const { return hitByBullet_; }
	void NotifyHitByBullet(bool hitByBullet) { hitByBullet_ = hitByBullet; }

	int GetBulletBounceCount() const { return bulletBounceCount_; }
	void SetBulletBounceCount(int bulletBounceCount) { bulletBounceCount_ = bulletBounceCount; }

	bool CheckEntityAlive() { return entityAlive_; }
	void SetEntityDead() { entityAlive_ = false; }

	bool CheckDeadEnemyDetected() { return deadEnemyDetected_; }
	void SetBodyDetected() { deadEnemyDetected_ = true; }

private:
	EntityType type_;
	bool dead_ = false;

	//colliderPosition_ is updated by a PhysicsComponent based on direction_
	sf::Vector2f colliderPosition_;

	//direction_ is updated by a ControllerComponent based on player input or some notification
	sf::Vector2f direction_;

	//centerPosition_ is updated by a PhysicsComponent (center of the collider) and used by a VisualComponent to draw a sprite at its centered origin
	//it's set once on GameObject construction to the starting position + 1/2 the sprite size, which is used by entities that do not have a PhysicsComponent
	sf::Vector2f centerPosition_;

	//currentAnimation_ and animationLoopFrame_ are updated by a ControllerComponent and used by a VisualComponent
	std::stack<std::string> animationStack_;
	std::stack<int> loopFrameStack_;

	//rotation_ is 0 for everything except the character and bullets -- it's updated by a ControllerComponent and used by a VisualComponent
	sf::Angle rotation_ = sf::degrees(0);

	SoundEvent currentSound_ = SoundEvent::NONE;

	//entity-specific state
	bool collidedHorizontally_ = false;
	bool collidedVertically_ = false;
	bool hitByBullet_ = false;
	int bulletBounceCount_ = 0;
	bool entityAlive_ = true;
	bool deadEnemyDetected_ = false;
};

