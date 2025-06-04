#pragma once
#include <SFML/Audio.hpp>
#include <map>

#include "GameObject.h"

class AudioComponent {
public:
	AudioComponent(int const objId, 
		std::map<std::string, sf::SoundBuffer> const &soundEffects) 
		: 
		objId_(objId), 
		soundEffects_(soundEffects) 
	{
		sounds_.push_back(sf::Sound(soundEffects_.begin()->second));
	}
	~AudioComponent() {}

	int GetGameObjectId() const { return objId_; }

	void Update(GameObject& obj);

private:
	int objId_;

	std::map<std::string, sf::SoundBuffer> soundEffects_;
	std::vector<sf::Sound> sounds_; //there's only one sound per entity but have to use an array because you can't make an empty container for a single object

};