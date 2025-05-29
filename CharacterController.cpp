#include "CharacterController.h"

void CharacterController::Update(GameObject &obj) {
    HandleInput(obj);
    UpdateDirection(obj);
}

void CharacterController::HandleInput(GameObject &obj) {
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
                ReloadCommand();
            }
        }
        //mouse click input here
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

    //get the mouse position in screen space
    sf::Vector2i mousePos = sf::Mouse::getPosition();
    //get the screen dimensions
    sf::Vector2u screenRes = sf::VideoMode::getDesktopMode().size;
    //calculate the vector between the player and mouse
    sf::Vector2f relativePos = { mousePos.x - (obj.GetPosition().x + screenRes.x / 2), mousePos.y - (obj.GetPosition().y - screenRes.y / 2) };
    //convert the vector into an angle (radians)
    float lookAtMouseRad = -atan2(relativePos.y, relativePos.x);
    //convert the angle into degrees
    float lookAtMouseDeg = lookAtMouseRad * 180 / 1.141f;
    sf::Angle characterAngle = sf::degrees(lookAtMouseDeg);
    obj.SetRotation(characterAngle);
}

void CharacterController::UpdateDirection(GameObject &obj) {
    //start by setting the direction values to zero
    float newDirX = 0, newDirY = 0;
    
    //add and subtract values based on which keys are pressed, which allows users to hold multiple (even opposing) move keys
    if (moveUp_) newDirY--;
    if (moveDown_) newDirY++;
    if (moveLeft_) newDirX--;
    if (moveRight_) newDirX++;

    //set the GameObject's direction variable
    obj.SetDirection({ newDirX, newDirY });
    
    //update the GameObject's current animation variable
    if (sf::Vector2f(newDirX, newDirY).lengthSquared() == 0) //if there's no direction (no input), set the animation to idle
    {
        obj.SetCurrentAnimation(0);
    }
    else //if there's direction (input), set the animation to walking
    { 
        obj.SetCurrentAnimation(1); 
    }
}

void CharacterController::ShootCommand() {

}

void CharacterController::ReloadCommand() {

}

void CharacterController::ButtonClickCommand() {

}