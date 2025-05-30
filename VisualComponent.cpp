#include "VisualComponent.h"
#include <iostream>

void VisualComponent::Update(GameObject &obj, float const deltaTime, sf::Sprite &sprite) {
	
	timeElapsedSinceLastFrame_ += deltaTime;
		
	//if a new animation just started, play it from frame 0
	if (lastAnimation_ != obj.GetCurrentAnimation()) 
	{
		currentFrame_ = 0;
		lastAnimation_ = obj.GetCurrentAnimation();
	}
	//otherwise, check if it's time to increment the current frame
	else 
	{
		if (timeElapsedSinceLastFrame_ >= (float)1/animationFrameRate) //1/animationFrameRate gives seconds per frame
		{
			//increment currentFrame by 1 modulo the number of frames in the currently playing animation
			currentFrame_ = (currentFrame_ + 1) % animations_[obj.GetCurrentAnimation()].size();
			timeElapsedSinceLastFrame_ = 0;
		}
	}
	sprite.setTexture(animations_[obj.GetCurrentAnimation()][currentFrame_]);
			
	//every sprite's origin is set to its center, so offset the true position by half of the sprite's size
	sf::Vector2f spritePos = obj.GetPosition() + sprite.getLocalBounds().size / 2.f;

	//move and draw the Sprite
	sprite.setPosition(spritePos);
	sprite.setRotation(obj.GetRotation());

	window_->draw(sprite);
}