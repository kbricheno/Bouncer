#include "VisualComponent.h"

VisualComponent::VisualComponent(std::vector<sf::Sprite> sprites) {
	sprites_ = sprites;
}

sf::Sprite VisualComponent::Update() {
	return sprites_[0];
}