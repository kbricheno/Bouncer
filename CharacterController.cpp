#include <iostream>
#include "CharacterController.h"

void CharacterController::Update(GameObject &obj, float const deltaTime) {
    //perform specific behaviour depending on type of entity
    switch (obj.GetType())
    {
    case GameObject::EntityType::CHARACTER:
        obj.SetDirection(CalculateCharacterDirection(obj));
        obj.SetRotation(CalculateCharacterRotation(obj));
        break;

    case GameObject::EntityType::ENEMY:
        break;

    case GameObject::EntityType::BULLET:
        break;

    default:
        break;
    }
    timeSinceLastShot += deltaTime;
}

#pragma region Character-Specific Behaviour

//Update the character GameObject's direction_ variable based on any input received earlier (keys pressed)
sf::Vector2f CharacterController::CalculateCharacterDirection(GameObject &obj) {
    //start by setting the direction values to zero
    float newDirX = 0, newDirY = 0;
    
    //add and subtract values based on which keys are pressed, which allows users to hold multiple (even opposing) move keys
    if (moveUp_) newDirY--;
    if (moveDown_) newDirY++;
    if (moveLeft_) newDirX--;
    if (moveRight_) newDirX++;

    //change animation
    if (sf::Vector2f{ newDirX, newDirY }.lengthSquared() == 0) //if there's no direction (no input), character is idling
    {
        //the idling animation is 0 which for every object is always at the bottom of the animation stack, looping forever
        //if the character is currently walking, pop the walking animation out to enable the idling animation to play
        //idling should only ever interrupt walking, not shooting or reloading (shooting/reloading don't loop so they pop themselves out of the stack when finished)
        if (obj.GetCurrentAnimation() == "walk")
        {
            obj.RemoveAnimationFromStack();
        }
    }
    else //if there's direction (input), character is walking
    {
        //only interrupt the idling animation, not shooting or reloading
        if (obj.GetCurrentAnimation() == "idle")
        {
            obj.AddAnimationToStack("walk", 0);
        }
    }

    return { newDirX, newDirY };
}

//Update the character GameObject's rotation_ variable based on the mouse position and GameObject position
sf::Angle CharacterController::CalculateCharacterRotation(GameObject &obj) {
    //get the mouse position in the window (in screen space)
    sf::Vector2i mousePos = sf::Mouse::getPosition(*window_);

    //calculate the vector between the player and mouse
    sf::Vector2f relativePos = { mousePos.x - obj.GetCenter().x, mousePos.y - (obj.GetCenter().y)};

    //convert the vector into an angle (radians)
    float lookAtMouse = atan2(relativePos.y, relativePos.x);

    //convert the angle from a float to a sf::Angle
    sf::Angle characterAngle = sf::radians(lookAtMouse);

    //update the GameObject's rotation_ variable
    return characterAngle;
}

//Command called from the Level instance -- check if the character is able to shoot
bool CharacterController::ValidateShootCommand(GameObject &obj) {
    //prevent bullet spam
    if (timeSinceLastShot < timeBetweenShots) return false;

    //prevent shooting when out of bullets (add UI message here)
    if (characterCurrentBullets <= 0) return false;

    //prevent shooting while reloading
    if (obj.GetCurrentAnimation() == "reload") return false;


    //passed conditions: can shoot 
    //perform character shoot behaviour (sound and animation)
    Shoot(obj);

    //inform Level it's okay to spawn a bullet
    return true;
}

//Make the character shoot
void CharacterController::Shoot(GameObject &obj) {
    
    //change animation
    if (obj.GetCurrentAnimation() != "shoot")
    {
        obj.AddAnimationToStack("shoot");
    }

    //play a sound
    obj.NotifySoundEvent(GameObject::SoundEvent::CHARACTER_SHOOT);

    //shoot
    timeSinceLastShot = 0;
    characterCurrentBullets--;
}

//Command called from the Level instance -- make the character reload
void CharacterController::Reload(GameObject &obj) {

    //ignore reload command when character has full ammo
    if (characterCurrentBullets == characterMaxBullets) return;

    //change animation
    if (obj.GetCurrentAnimation() != "reload") 
    {
        obj.AddAnimationToStack("reload");
    }

    //play a sound
    obj.NotifySoundEvent(GameObject::SoundEvent::CHARACTER_RELOAD);

    //reload
    characterCurrentBullets = characterMaxBullets;
}

#pragma endregion

#pragma region Enemy-Specific Behaviour



#pragma endregion

#pragma region Bullet-Specific Behaviour



#pragma endregion