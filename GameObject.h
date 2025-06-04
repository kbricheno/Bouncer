#pragma once
#include <SFML/Graphics.hpp>

class ControllerComponent;
class PhysicsComponent;
class VisualComponent;
class AudioComponent;

class GameObject {
public:
	GameObject(int const gameObjectId,
		sf::Vector2f const position,
		sf::Vector2f const spriteSize,
		sf::Vector2f const direction = sf::Vector2f())
		: 
		gameObjectId_(gameObjectId),
		colliderPosition_(position),
		spritePosition_(spriteSize/2.f),
		direction_(direction)
	{}
	~GameObject() {}

	int const GetGameObjectId() const { return gameObjectId_; }

	//getters and setters for variables used by Components
	sf::Vector2f GetColliderPosition() const { return colliderPosition_; }
	void SetColliderPosition(sf::Vector2f colliderPosition) { colliderPosition_ = colliderPosition; }

	sf::Vector2f GetCenter() const { return centerPosition_; }
	void SetCenter(sf::Vector2f centerPosition) { centerPosition_ = centerPosition; }

	sf::Vector2f GetDirection() const { return direction_; }
	void SetDirection(sf::Vector2f direction) { direction_ = direction; }

	sf::Angle GetRotation() const { return rotation_; }
	void SetRotation(sf::Angle rotation) { rotation_ = rotation; }

	int GetCurrentAnimation() const { return currentAnimation_; }
	int GetAnimationLoopFrame() const { return animationLoopFrame_; }
	void SetCurrentAnimation(int currentAnimation, int loopAnimationFromFrame = -1) { currentAnimation_ = currentAnimation; animationLoopFrame_ = loopAnimationFromFrame; }

	void SetSpritePosition(sf::Vector2f spritePosition) { spritePosition_ = spritePosition; }
	sf::Vector2f GetSpritePosition() const { return spritePosition_; }

	bool CheckDead() const { return dead_; }
	void Kill() { dead_ = true; }

	bool CheckHorizontalCollision() const { return collidedHorizontally_; }
	void NotifyHorizontalCollision(bool collidedHor) { collidedHorizontally_ = collidedHor; }
	bool CheckVerticalCollision() const { return collidedVertically_; }
	void NotifyVerticalCollision(bool collidedVer) { collidedVertically_ = collidedVer; }

	bool CheckHitByBullet() const { return hitByBullet_; }
	void NotifyHitByBullet(bool hitByBullet) { hitByBullet_ = hitByBullet; }

	int GetBulletBounceCount() const { return bulletBounceCount_; }
	void SetBulletBounceCount(int bulletBounceCount) { bulletBounceCount_ = bulletBounceCount; }

private:
	int gameObjectId_;

	bool dead_ = false;

	sf::Vector2f colliderPosition_;
	sf::Vector2f direction_;
	sf::Vector2f centerPosition_ = { 0,0 };
	int currentAnimation_ = 0;
	int animationLoopFrame_ = 0;
	sf::Angle rotation_ = sf::degrees(0);
	sf::Vector2f spritePosition_;

	//entity-specific state
	bool collidedHorizontally_ = false;
	bool collidedVertically_ = false;
	bool hitByBullet_ = false;
	int bulletBounceCount_ = 0;
};

