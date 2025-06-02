#include "EnemyController.h"
#include <iostream>

bool EnemyController::HandleInput(GameObject &obj, float const deltaTime) {
	obj.SetDirection(currentDirection_);
	obj.SetRotation(currentDirection_.angle());

	randomTurnTimer_ -= deltaTime;
	stopTimer_ -= deltaTime;

	if (!turning_) //if enemy isn't already turning
	{
		//if enemy randomly decides to turn or collides with a wall
		if (randomTurnTimer_ <= 0 || obj.CheckHorizontalCollision() || obj.CheckVerticalCollision())
		{
			//start the stop timer before changing direction
			stopTimer_ = (rand() % 2) + 1;
			turning_ = true;

			//reset the collision bools and turn timer
			obj.NotifyHorizontalCollision(false);
			obj.NotifyVerticalCollision(false);
			randomTurnTimer_ = (rand() % 5) + 3 + stopTimer_;
		}
	}
	else
	{
		if (stopTimer_ <= 0)
		{
			currentDirection_ = ChangeDirection();
			turning_ = false;
		}
	}

	return false;
}

sf::Vector2f EnemyController::ChangeDirection() {
	
	sf::Vector2f nextDirection;

	for (int i = 0; i < directions_.size(); i++)
	{
		if (directions_[i] == currentDirection_)
		{
			directions_.erase(directions_.begin() + i);
			break;
		}
	}

	nextDirection = directions_[rand() % 3];
	directions_.push_back(currentDirection_);	
	
	return nextDirection;
}