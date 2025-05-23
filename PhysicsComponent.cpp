#include "PhysicsComponent.h"

void PhysicsComponent::Update() {

}

//PhysicsComponent::ColliderType PhysicsComponent::ResolveCollisions(std::vector<PhysicsComponent*> allColliders) {
//	for (int i = 0; i < allColliders.size(); i++)
//	{
//		if (hitbox_.findIntersection(allColliders[i]->GetRect())) {
//			return allColliders[i]->GetType();
//		}
//	}
//
//	return ColliderType::NONE;
//}