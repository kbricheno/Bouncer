#include "VisualComponent.h"
#include <iostream>

void VisualComponent::Update() {
	if (obj_ != nullptr) 
	{
		//std::cout << "obj " << &obj_ << " pos: " << obj_->GetPosition().x << ", " << obj_->GetPosition().y << "\n";
		for (int i = 0; i < sprites_.size(); i++)
		{
			//std::cout << "sprite pos: " << sprites_[i].getPosition().x << ", " << sprites_[i].getPosition().y << "\n";
			//std::cout << "sprite tex: " << sprites_[i].getTexture().getSize().x << "\n";

			//properly assign the Sprite's Texture (why doesn't the Texture reference it was constructed with still exist?)
			sprites_[i].setTexture(textures_[i]);
			
			//move and draw the Sprite
			sprites_[i].setPosition(obj_->GetPosition());
			window_->draw(sprites_[i]);
		}
	}
}