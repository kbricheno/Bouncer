#include "AudioComponent.h"

//Define the static float that determines the volume across the entire game
float AudioComponent::m_globalVolume = 1.f;

void AudioComponent::Update(GameObject &obj) {
	
	//check if there's a sound to be played this frame
	if (obj.GetSoundEvent() != GameObject::SoundEvent::NONE) 
	{
		switch (obj.GetSoundEvent())
		{
		case GameObject::SoundEvent::HERO_SHOOT:
			PlaySound("shoot");
			break;

		case GameObject::SoundEvent::HERO_RELOAD:
			PlaySound("reload");
			break;

		case GameObject::SoundEvent::BULLET_COLLISION:
			//this is the only sound event that could have come from more than one type of entity, so check which entity this AudioComponent belongs to
			switch (obj.GetType())
			{
			case GameObject::EntityType::ENEMY:
				PlaySound("die");
				break;

			case GameObject::EntityType::DOOR:
				PlaySound("break");
				break;
			}
			break;

		case GameObject::SoundEvent::SOLID_COLLISION:
			PlaySound("bounce");
			break;
		}

		//reset the sound event
		obj.NotifySoundEvent(GameObject::SoundEvent::NONE);
	}
}

void AudioComponent::PlaySound(std::string inSoundName) {
	//set the sound's buffer based on the provided name, set the sound's volume, and play the sound
	m_sound.setBuffer(m_soundBuffersRef.at(inSoundName));
	m_sound.setVolume(m_globalVolume * 100.f);
	m_sound.play();
}
