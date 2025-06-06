#include "AudioComponent.h"

void AudioComponent::Update(GameObject &obj) {
	
	//check if there's a sound to be played this frame
	if (obj.CheckSoundEvent() != GameObject::SoundEvent::NONE) 
	{
		GameObject::SoundEvent event = obj.CheckSoundEvent();
		switch (event)
		{
		case GameObject::SoundEvent::CHARACTER_SHOOT:
			sounds_[0].setBuffer(soundEffects_["shoot"]);
			sounds_[0].play();
			break;

		case GameObject::SoundEvent::CHARACTER_RELOAD:
			sounds_[0].setBuffer(soundEffects_["reload"]);
			sounds_[0].play();
			break;

		case GameObject::SoundEvent::BULLET_COLLISION:
			//this is the only sound event that could have come from more than one type of entity, so check which entity this AudioComponent belongs to
			switch (obj.GetType())
			{
			case GameObject::EntityType::ENEMY:
				sounds_[0].setBuffer(soundEffects_["die"]);
				sounds_[0].play();
				break;

			case GameObject::EntityType::DOOR:
				sounds_[0].setBuffer(soundEffects_["break"]);
				sounds_[0].play();
				break;

			default:
				break;
			}
			break;

		case GameObject::SoundEvent::SOLID_COLLISION:
			sounds_[0].setBuffer(soundEffects_["bounce"]);
			sounds_[0].play();
			break;

		default:
			break;
		}

		//reset the sound event
		obj.NotifySoundEvent(GameObject::SoundEvent::NONE);
	}
}