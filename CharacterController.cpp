#include "CharacterController.h"

void CharacterController::Update() {
    DecodeInput();
}

void CharacterController::DecodeInput() {
    // handle input from the event queue
    while (const std::optional event = window_->pollEvent())
    {
        // closing window
        if (event->is<sf::Event::Closed>())
        {
            window_->close();
        }
        //maybe use a stack for move commands?
        //key releases
        else if (const auto* keyReleased = event->getIf<sf::Event::KeyReleased>())
        {
            if (keyReleased->scancode == sf::Keyboard::Scancode::W)
            {
                StopMoveCommand(CharacterController::PlayerInput::W);
            }
            if (keyReleased->scancode == sf::Keyboard::Scancode::A)
            {
                StopMoveCommand(CharacterController::PlayerInput::A);
            }
            if (keyReleased->scancode == sf::Keyboard::Scancode::S)
            {
                StopMoveCommand(CharacterController::PlayerInput::S);
            }
            if (keyReleased->scancode == sf::Keyboard::Scancode::D)
            {
                StopMoveCommand(CharacterController::PlayerInput::D);
            }
        }
        //key presses
        else if (const auto* keyPressed = event->getIf<sf::Event::KeyPressed>())
        {
            // quit game through escape for now
            if (keyPressed->scancode == sf::Keyboard::Scancode::Escape)
            {
                window_->close();
            }
            if (keyPressed->scancode == sf::Keyboard::Scancode::W)
            {
                StartMoveCommand(CharacterController::PlayerInput::W);
            }
            if (keyPressed->scancode == sf::Keyboard::Scancode::A)
            {
                StartMoveCommand(CharacterController::PlayerInput::A);
            }
            if (keyPressed->scancode == sf::Keyboard::Scancode::S)
            {
                StartMoveCommand(CharacterController::PlayerInput::S);
            }
            if (keyPressed->scancode == sf::Keyboard::Scancode::D)
            {
                StartMoveCommand(CharacterController::PlayerInput::D);
            }
            if (keyPressed->scancode == sf::Keyboard::Scancode::R)
            {
                ReloadInput();
            }
        }
        //mouse click input here
    }
}

void CharacterController::StartMoveCommand(PlayerInput input) {
    float newDirX = obj_->GetDirection().x, newDirY = obj_->GetDirection().y;
    switch (input)
    {
    case CharacterController::PlayerInput::A:
        newDirX = -1;
        break;
    case CharacterController::PlayerInput::D:
        newDirX = 1;
        break;
    case CharacterController::PlayerInput::W:
        newDirY = -1;
        break;
    case CharacterController::PlayerInput::S:
        newDirY = 1;
        break;
    }
    obj_->SetDirection({ newDirX, newDirY });
}

void CharacterController::StopMoveCommand(PlayerInput input) {
    float newDirX = obj_->GetDirection().x, newDirY = obj_->GetDirection().y;
    switch (input)
    {
    case CharacterController::PlayerInput::A:
        newDirX = 0;
        break;
    case CharacterController::PlayerInput::D:
        newDirX = 0;
        break;
    case CharacterController::PlayerInput::W:
        newDirY = 0;
        break;
    case CharacterController::PlayerInput::S:
        newDirY = 0;
        break;
    }
    obj_->SetDirection({ newDirX, newDirY });
}

void CharacterController::ShootInput() {

}

void CharacterController::ReloadInput() {

}

void CharacterController::ButtonClickInput() {

}