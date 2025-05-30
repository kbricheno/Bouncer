#include "BulletController.h"

bool BulletController::HandleInput(GameObject& obj, float const deltaTime) {
	if (firstUpdate) 
	{
		obj.SetDirection(startDir_);
		firstUpdate = false;
	}
	return false;
}