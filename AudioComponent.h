#pragma once
#include <SFML/Audio.hpp>
#include <map>
#include "GameObject.h"

class AudioComponent {
public:
	//Create an AudioComponent, passing in a reference to the map of SoundBuffers generated in GameManager
	AudioComponent(std::map<std::string, sf::SoundBuffer>& const inSoundBuffers) : m_soundBuffersRef(inSoundBuffers) {}
	~AudioComponent() {}

	//Update is called by the Level instance once per frame
	void Update(GameObject& obj);

	static float m_globalVolume; //determines the volume for the entire game

private:
	//Switches out m_sound's SoundBuffer and plays the audio
	void PlaySound(std::string inSoundName);

	std::map<std::string, sf::SoundBuffer>& const m_soundBuffersRef; //reference to all SoundBuffers
	sf::Sound m_sound = sf::Sound(m_soundBuffersRef.begin()->second); //sf::Sound doesn't have a default constructor so just pass in the first SoundBuffer for now
};