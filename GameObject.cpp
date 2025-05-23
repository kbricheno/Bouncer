#include "GameObject.h"

void GameObject::SetComponents(//const ControllerComponent* controllerComponent,
							   //const PhysicsComponent* physicsComponent,
							   VisualComponent* visualComponent
							   /*const AudioComponent* audioComponent*/){
	visualComponent_ = visualComponent;
}