#include "VisualComponent.h"
#include <iostream>

void VisualComponent::Update(GameObject &obj, float const deltaTime) {
	
	//if a new animation has been set, push it onto the stack and play it from frame 0
	if (obj.GetCurrentAnimation() != animationStack.top()) 
	{
		animationStack.push(obj.GetCurrentAnimation());
		currentFrame_ = 0;
	}
	else 
	{
		if (timeElapsedSinceLastFrame_ >= 1.f / animationFrameRate) //1/animationFrameRate gives seconds per frame
		{
			//increment current frame by 1 until the entire animation has played through
			if (currentFrame_ < animations_[animationStack.top()].size() - 1)
			{
				currentFrame_++;
			}
			//if the animation loops, reset currentFrame to the desired loop frame
			else if (obj.GetAnimationLoopFrame() != -1)
			{
				currentFrame_ = obj.GetAnimationLoopFrame();
			}
			//if the animation does not loop, pop it out of the stack
			else 
			{
				animationStack.pop();
			}
			
			//reset the timer
			timeElapsedSinceLastFrame_ = 0;
		}
	}

	//increase the frame timer
	timeElapsedSinceLastFrame_ += deltaTime;

	//set the sprite to use the correct frame from the correct animation
	sprites_[0].setTexture(animations_[animationStack.top()][currentFrame_]);
			
	//every sprite's origin is set to its center, so offset the true (collider's) position by half of the sprite's size (used for rotating sprites)
	obj.SetCenter(obj.GetColliderPosition() + sprites_[0].getLocalBounds().size / 2.f);

	//move and draw the Sprite
	sprites_[0].setPosition(obj.GetSpritePosition());
	sprites_[0].setRotation(obj.GetRotation());

	std::cout << sprites_[0].getPosition().x << ", " << sprites_[0].getPosition().y << "\n";

	window_->draw(sprites_[0]);
}