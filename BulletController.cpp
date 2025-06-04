#include "BulletController.h"

bool BulletController::HandleInput(GameObject& obj, float const deltaTime) {

	if (obj.CheckHorizontalCollision()) //bullet hit a left/right wall
	{
		obj.NotifyHorizontalCollision(false);

		obj.SetDirection({ obj.GetDirection().x * -1.f, obj.GetDirection().y });

		obj.SetBulletBounceCount(obj.GetBulletBounceCount() + 1);
		if (obj.GetBulletBounceCount() > maxBounces_) obj.Kill();
	}
	else if (obj.CheckVerticalCollision()) //bullet hit a top/bottom wall
	{
		obj.NotifyVerticalCollision(false);

		obj.SetDirection({ obj.GetDirection().x, obj.GetDirection().y * -1.f });

		obj.SetBulletBounceCount(obj.GetBulletBounceCount() + 1);		
		if (obj.GetBulletBounceCount() > maxBounces_) obj.Kill();
	}

	return false;
}
