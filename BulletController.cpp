#include "BulletController.h"

bool BulletController::HandleInput(GameObject& obj, float const deltaTime) {

	if (obj.CheckHorizontalCollision()) //bullet hit a left/right wall
	{
		//reset the notification so this doesn't trigger constantly
		obj.NotifyHorizontalCollision(false);

		obj.SetDirection({ obj.GetDirection().x * -1.f, obj.GetDirection().y });

		obj.NotifySoundEvent(GameObject::SoundEvent::SOLID_COLLISION);

		obj.SetBulletBounceCount(obj.GetBulletBounceCount() + 1);
		if (obj.GetBulletBounceCount() > maxBounces_) obj.Kill();
	}
	else if (obj.CheckVerticalCollision()) //bullet hit a top/bottom wall
	{
		obj.NotifyVerticalCollision(false);

		obj.SetDirection({ obj.GetDirection().x, obj.GetDirection().y * -1.f });

		obj.NotifySoundEvent(GameObject::SoundEvent::SOLID_COLLISION);

		obj.SetBulletBounceCount(obj.GetBulletBounceCount() + 1);		
		if (obj.GetBulletBounceCount() > maxBounces_) obj.Kill();
	}

	return false;
}
