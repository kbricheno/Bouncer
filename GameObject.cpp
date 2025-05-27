#include "GameObject.h"

void GameObject::SetComponents(ControllerComponent* const controllerComponent,
							   PhysicsComponent* const physicsComponent,
							   VisualComponent* const visualComponent,
							   AudioComponent* const audioComponent){
	controllerComponent_ = controllerComponent;
	physicsComponent_ = physicsComponent;
	visualComponent_ = visualComponent;
	audioComponent_ = audioComponent;
}