#include "CharacterController.h"

void CharacterController::Update(GameObject &obj, float const deltaTime) {
    //handle events and key presses
    HandleInput(obj);
    timeSinceLastShot += deltaTime;

    //update the GameObject's direction_ variable based on the handled input (keys pressed)
    obj.SetDirection(CalculateDirection());
    //update the GameObject's rotation_ variable based on the handled input (mouse position, obj position)
    obj.SetRotation(CalculateRotation(obj.GetPosition()));
    //update the GameObject's currentAnimation_ variable based on the handled input (state) 
    obj.SetCurrentAnimation(CalculateAnimation());
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
        else if (const auto* mouseButtonPressed = event->getIf<sf::Event::MouseButtonPressed>())
        {
            if (mouseButtonPressed->button == sf::Mouse::Button::Left)
            {
                ShootCommand();
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
}

sf::Vector2f CharacterController::CalculateDirection() {
    //start by setting the direction values to zero
    float newDirX = 0, newDirY = 0;
    
    //add and subtract values based on which keys are pressed, which allows users to hold multiple (even opposing) move keys
    if (moveUp_) newDirY--;
    if (moveDown_) newDirY++;
    if (moveLeft_) newDirX--;
    if (moveRight_) newDirX++;

    if (sf::Vector2f{ newDirX, newDirY }.lengthSquared() == 0) //if there's no direction (no input), character is idling
    {
        state_ = CharacterState::IDLING;
    }
    else //if there's direction (input), character is walking
    {
        state_ = CharacterState::WALKING;
    }

    return { newDirX, newDirY };
}

int CharacterController::CalculateAnimation() {
    switch (state_)
    {
    case CharacterController::CharacterState::IDLING:
        return 0;
        break;
    case CharacterController::CharacterState::WALKING:
        return 1;
        break;
    case CharacterController::CharacterState::SHOOTING:
        return 2;
        break;
    case CharacterController::CharacterState::RELOADING:
        return 3;
        break;
    default:
        break;
    }
}

sf::Angle CharacterController::CalculateRotation(sf::Vector2f objPosition) {
    //get the mouse position in the window (in screen space)
    sf::Vector2i mousePos = sf::Mouse::getPosition(*window_);

    //calculate the vector between the player and mouse
    sf::Vector2f relativePos = { mousePos.x - objPosition.x, mousePos.y - (objPosition.y) };

    //convert the vector into an angle (radians)
    float lookAtMouse = atan2(relativePos.y, relativePos.x);

    //convert the angle from a float to a sf::Angle
    sf::Angle characterAngle = sf::radians(lookAtMouse);

    //update the GameObject's rotation_ variable
    return characterAngle;
}

void CharacterController::ShootCommand() {
    //prevent bullet spam
    if (timeSinceLastShot < timeBetweenShots) return;

    //prevent shooting when out of bullets (add UI message here)
    if (characterCurrentBullets <= 0) return;

    timeSinceLastShot = 0;
    //tell Level to spawn a bullet using obj direction & obj position -- how to do this without creating a circular dependency?
}

void CharacterController::ReloadCommand() {

}

void CharacterController::ButtonClickCommand() {

}