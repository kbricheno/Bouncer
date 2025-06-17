#include "VisualComponent.h"

void VisualComponent::Update(GameObject &obj, float const deltaTime) {
	
	//if a new animation has been set, play it from frame 0
	if (obj.GetCurrentAnimation() != m_previousAnimation) 
	{
		m_currentFrame = 0;
		m_previousAnimation = obj.GetCurrentAnimation();
	}
	else 
	{
		if (m_timeElapsedSinceLastFrame >= 1.f / m_animationFramerate) //1/animationFrameRate gives seconds per frame
		{
			//increment current frame by 1 until the entire animation has played through
			if (m_currentFrame < m_animationsRef.at(obj.GetCurrentAnimation()).size() - 1)
			{
				m_currentFrame++;
			}
			//if the animation loops, reset currentFrame to the desired loop frame
			else if (obj.GetAnimationLoopFrame() != -1)
			{
				m_currentFrame = obj.GetAnimationLoopFrame();
			}
			//if the animation does not loop, pop it out of the stack and update the GameObject about which animation is currently playing
			else 
			{
				obj.RemoveAnimationFromStack();
				m_currentFrame = 0;
			}
			
			//reset the timer
			m_timeElapsedSinceLastFrame = 0;
		}
	}

	//increase the frame timer
	m_timeElapsedSinceLastFrame += deltaTime;

	//set the sprite to use the correct frame from the correct animation
	m_sprite.setTexture(m_animationsRef.at(obj.GetCurrentAnimation())[m_currentFrame]);
			
	//move and draw the Sprite
	m_sprite.setPosition(obj.GetCenter());
	m_sprite.setRotation(obj.GetRotation());

	m_windowRef.draw(m_sprite);
}