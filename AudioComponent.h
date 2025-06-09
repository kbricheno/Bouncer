#pragma once
#include <SFML/Audio.hpp>
#include <map>

#include "GameObject.h"

class AudioComponent {
public:
	AudioComponent(std::map<std::string, sf::SoundBuffer> const &soundEffects) : soundEffects_(soundEffects) {}
	~AudioComponent() {}

	void Update(GameObject& obj);

private:
	std::map<std::string, sf::SoundBuffer> soundEffects_;
	std::array<sf::Sound, 1> sounds_ = {sf::Sound(soundEffects_.begin()->second)}; //there's only one sound per entity but have to use an array because you can't make an empty container for a single object
};