#pragma once
#include <map>
#include "GameObject.h"

class VisualComponent {
public:
	//Create a VisualComponent, passing in a reference to the window and the animations for the relevant entity, plus its starting animation
	VisualComponent(sf::RenderWindow& const inWindowRef,
		std::map<std::string, std::vector<sf::Texture>> const& inAnimationsRef,
		std::string inStartAnim = "idle")
		:
		m_windowRef(inWindowRef),
		m_animationsRef(inAnimationsRef),
		m_previousAnimation(inStartAnim)
	{
		m_sprite.setOrigin(m_sprite.getLocalBounds().size / 2.f); //set the Sprite's origin to its center (for Sprite rotation)
	}
	~VisualComponent() {}

	//Update is called by the Level instance once per frame
	void Update(GameObject &obj, float const deltaTime);

private:
	static int const m_animationFramerate = 10; //animation framerate across the entire game

	sf::RenderWindow& const m_windowRef; //reference to the window for drawing Sprites
	std::map<std::string, std::vector<sf::Texture>> const m_animationsRef; //reference to the animation Textures for the relevant entity
	
	std::string m_previousAnimation; //used to keep track of when the animation changes so the current frame can be reset
	sf::Sprite m_sprite = sf::Sprite(m_animationsRef.at(m_previousAnimation).front()); //create a Sprite using the starting animation at frame 0
	
	int m_currentFrame = 0; //which frame of the animation is currently being drawn to the window
	float m_timeElapsedSinceLastFrame = 0.f; //used to keep track of how long a frame has been playing for
};