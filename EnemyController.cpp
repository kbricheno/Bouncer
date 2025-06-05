#include "EnemyController.h"
#include <iostream>

void EnemyController::Update(GameObject &obj, float const deltaTime) {
	
	//if the enemy is "dead", no further behaviour updates are needed
	if (!active_) return;

	//check if enemy "died" this frame
	if (obj.CheckHitByBullet()) 
	{
		//perform death behaviours
		Die(obj);

		//reset the notification so this doesn't trigger multiple times
		obj.NotifyHitByBullet(false);

		//call no further updates
		active_ = false;
	}

	//movement/direction updating
	if (!turning_) //if enemy isn't already turning
	{
		randomTurnTimer_ -= deltaTime;

		//if enemy randomly decides to turn or collides with a wall
		if (randomTurnTimer_ <= 0 || obj.CheckHorizontalCollision() || obj.CheckVerticalCollision())
		{
			//start the stop timer before changing direction
			stopTimer_ = (rand() % 3) + 1;
			turning_ = true;

			//reset the collision bools and turn timer
			obj.NotifyHorizontalCollision(false);
			obj.NotifyVerticalCollision(false);
			randomTurnTimer_ = (rand() % 5) + 3;

			//stop moving to prevent further collision detection
			obj.SetDirection({ 0,0 });

			//remove the walk animation so the idle beneath it plays
			if (obj.GetCurrentAnimation() == "walk") 
			{
				obj.RemoveAnimationFromStack();
			}
		}
	}
	else
	{
		//stop moving to prevent further collision detection
		obj.SetDirection({ 0,0 });

		//pause for a few seconds before changing direction
		stopTimer_ -= deltaTime;

		if (stopTimer_ <= 0)
		{
			currentDirection_ = ChangeDirection();
			obj.SetDirection(currentDirection_);
			obj.SetRotation(currentDirection_.angle());

			//set animation to walk if it's not already playing
			if (obj.GetCurrentAnimation() != "walk") 
			{
				obj.AddAnimationToStack("walk", 0);
			}

			turning_ = false;
		}
	}

	//check if the enemy was hit by a bullet
	if (obj.CheckHitByBullet())
	{
		Die(obj);
	}
}

void EnemyController::Die(GameObject &obj) {
	active_ = false;

	//stop moving
	obj.SetDirection({ 0,0 });

	//play the dying animation, loop the final frame
	obj.AddAnimationToStack("die", 3);

	//tell the AudioComponent to play a sound
	obj.NotifySoundEvent(GameObject::SoundEvent::BULLET_COLLISION);
}

sf::Vector2f EnemyController::ChangeDirection() {
	
	sf::Vector2f nextDirection;

	//temporarily remove the current direction from the vector of directions to choose from
	for (int i = 0; i < directions_.size(); i++)
	{
		if (directions_[i] == currentDirection_)
		{
			directions_.erase(directions_.begin() + i);
			break;
		}
	}

	//pick a direction at random
	nextDirection = directions_[rand() % 3];

	//return the removed direction to the vector
	directions_.push_back(currentDirection_);	
	
	return nextDirection;
}