#include "VisualComponent.h"

void VisualComponent::Update(GameObject &obj, float const deltaTime) {
	
	//if a new animation has been set, play it from frame 0
	if (obj.GetCurrentAnimation() != previousAnimation_) 
	{
		currentFrame_ = 0;
		previousAnimation_ = obj.GetCurrentAnimation();
	}
	else 
	{
		if (timeElapsedSinceLastFrame_ >= 1.f / animationFrameRate_) //1/animationFrameRate gives seconds per frame
		{
			//increment current frame by 1 until the entire animation has played through
			if (currentFrame_ < animations_.at(obj.GetCurrentAnimation()).size() - 1)
			{
				currentFrame_++;
			}
			//if the animation loops, reset currentFrame to the desired loop frame
			else if (obj.GetAnimationLoopFrame() != -1)
			{
				currentFrame_ = obj.GetAnimationLoopFrame();
			}
			//if the animation does not loop, pop it out of the stack and update the GameObject about which animation is currently playing
			else 
			{
				obj.RemoveAnimationFromStack();
				currentFrame_ = 0;
			}
			
			//reset the timer
			timeElapsedSinceLastFrame_ = 0;
		}
	}

	//increase the frame timer
	timeElapsedSinceLastFrame_ += deltaTime;

	//set the sprite to use the correct frame from the correct animation
	sprites_[0].setTexture(animations_.at(obj.GetCurrentAnimation())[currentFrame_]);
			
	//move and draw the Sprite
	sprites_[0].setPosition(obj.GetCenter());
	sprites_[0].setRotation(obj.GetRotation());

	window_.draw(sprites_[0]);
}