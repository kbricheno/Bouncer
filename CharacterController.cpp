#include <iostream>
#include "CharacterController.h"

bool CharacterController::HandleInput(GameObject &obj, float const deltaTime) {
    //handle events and key presses
    bool spawnBullet = ReceiveInput(obj);
    timeSinceLastShot += deltaTime;

    //update the GameObject's direction_ variable based on the handled input (keys pressed)
    obj.SetDirection(CalculateDirection(obj));
    //update the GameObject's rotation_ variable based on the handled input (mouse position, obj position)
    obj.SetRotation(CalculateRotation(obj));

    return spawnBullet;
}

bool CharacterController::ReceiveInput(GameObject &obj) {
    bool spawnBullet = false;
    // handle input from the event queue
    while (const std::optional event = window_->pollEvent())
    {
        // closing window
        if (event->is<sf::Event::Closed>())
        {
            window_->close();
        }
        //key press events
        else if (const auto* keyPressed = event->getIf<sf::Event::KeyPressed>())
        {
            // quit game through escape for now
            if (keyPressed->scancode == sf::Keyboard::Scancode::Escape)
            {
                window_->close();
            }            
            //reloading
            if (keyPressed->scancode == sf::Keyboard::Scancode::R)
            {
                ReloadCommand(obj);
            }
        }
        else if (const auto* mouseButtonPressed = event->getIf<sf::Event::MouseButtonPressed>())
        {
            if (mouseButtonPressed->button == sf::Mouse::Button::Left)
            {
                spawnBullet = ShootCommand(obj);
            }
        }
    }

    //avoid using Event::KeyPressed for movement because when a key is held, its events are generated with a significant delay
    //^ I actually switched Event key repeat off completely so it can be used for single key presses exclusively
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W))
    {
        moveUp_ = true;
    }
    else
    {
        moveUp_ = false;
    }

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A))
    {
        moveLeft_ = true;
    }
    else
    {
        moveLeft_ = false;
    }

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S))
    {
        moveDown_ = true;
    }
    else
    {
        moveDown_ = false;
    }

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D))
    {
        moveRight_ = true;
    }
    else
    {
        moveRight_ = false;
    }

    return spawnBullet;
}

sf::Vector2f CharacterController::CalculateDirection(GameObject &obj) {
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
        if (obj.GetCurrentAnimation() == 1)
        {
            obj.RemoveAnimationFromStack();
        }
    }
    else //if there's direction (input), character is walking
    {
        //only interrupt the idling animation, not shooting or reloading
        if (obj.GetCurrentAnimation() == 0) 
        {
            obj.AddAnimationToStack(1, 0);
        }
    }

    return { newDirX, newDirY };
}

sf::Angle CharacterController::CalculateRotation(GameObject &obj) {
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

bool CharacterController::ShootCommand(GameObject &obj) {
    //prevent bullet spam
    if (timeSinceLastShot < timeBetweenShots) return false;

    //prevent shooting when out of bullets (add UI message here)
    if (characterCurrentBullets <= 0) return false;

    //prevent shooting while reloading
    if (obj.GetCurrentAnimation() == 3) return false;

    //change animation
    obj.AddAnimationToStack(2);

    //play a sound
    obj.NotifySoundEvent(GameObject::SoundEvent::CHARACTER_SHOOT);

    //shoot
    timeSinceLastShot = 0;
    characterCurrentBullets--;

    //tell Level to spawn a bullet using obj direction & obj position -- how to do this without creating a circular dependency?
    return true;
}

void CharacterController::ReloadCommand(GameObject &obj) {
    //change animation
    obj.AddAnimationToStack(3);

    //play a sound
    obj.NotifySoundEvent(GameObject::SoundEvent::CHARACTER_RELOAD);

    //reload
    characterCurrentBullets = characterMaxBullets;
}

void CharacterController::ButtonClickCommand() {

}