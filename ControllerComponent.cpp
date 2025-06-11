#include "ControllerComponent.h"

void ControllerComponent::Update(GameObject &obj, float const deltaTime, sf::Vector2f mousePos) {
    //perform specific behaviour depending on type of entity
    switch (obj.GetType())
    {
    case GameObject::EntityType::CHARACTER:
        obj.SetDirection(CalculateCharacterDirection(obj));
        obj.SetRotation(CalculateCharacterRotation(obj, mousePos));
        CalculateCharacterAnimation(obj);
        timeSinceLastShot_ += deltaTime;
        break;

    case GameObject::EntityType::ENEMY:
        UpdateEnemyState(obj, deltaTime);
        CalculateEnemyAnimation(obj);
        break;

    case GameObject::EntityType::BULLET:
        //the only thing bullets need to do is check if they collided with something solid
        if (obj.CheckHorizontalCollision()) //bullet hit a left/right wall
        {
            //reset the notification so this doesn't trigger constantly
            obj.NotifyHorizontalCollision(false);
            BulletSolidCollision(obj, true);
        }
        else if (obj.CheckVerticalCollision()) //bullet hit a top/bottom wall
        {
            obj.NotifyVerticalCollision(false);
            BulletSolidCollision(obj, false);
        }
        break;

    default:
        break;
    }
}

#pragma region Character-Specific Behaviour

//Update the character's behaviour ------------------------------------------------------------------------------------------------------------------------------------------------

//Update the character GameObject's direction_ variable based on any input received earlier (keys pressed)
sf::Vector2f ControllerComponent::CalculateCharacterDirection(GameObject &obj) {
    
    //start by setting the direction values to zero
    float newDirX = 0, newDirY = 0;
    
    //add and subtract values based on which keys are pressed, which allows users to hold multiple (even opposing) move keys
    if (moveUp_) newDirY--;
    if (moveDown_) newDirY++;
    if (moveLeft_) newDirX--;
    if (moveRight_) newDirX++;

    return { newDirX, newDirY };
}

//Update the character GameObject's rotation_ variable based on the mouse position and GameObject position
sf::Angle ControllerComponent::CalculateCharacterRotation(GameObject &obj, sf::Vector2f mousePos) {

    //calculate the vector between the player and mouse
    sf::Vector2f relativePos = { mousePos.x - obj.GetCenter().x, mousePos.y - (obj.GetCenter().y)};

    //convert the vector into an angle (radians)
    float lookAtMouse = atan2(relativePos.y, relativePos.x);

    //convert the angle from a float to a sf::Angle
    sf::Angle characterAngle = sf::radians(lookAtMouse);

    //update the GameObject's rotation_ variable
    return characterAngle;
}

//Update the character GameObject's animation stack based on input received earlier
void ControllerComponent::CalculateCharacterAnimation(GameObject& obj, std::string anim) {

    //shooting animation is called by the Shoot method passing in "shoot" as the anim string
    //if not already playing the shoot animation, play it
    if (anim == "shoot" && obj.GetCurrentAnimation() != "shoot")
    {
        obj.AddAnimationToStack("shoot");
    }

    //reloading animation is called by the Reload method passing in "reload" as the anim string
    //if not already playing the reload animation, play it
    if (anim == "reload" && obj.GetCurrentAnimation() != "reload")
    {
        obj.AddAnimationToStack("reload");
    }

    //idling and walking animations are controlled by the GameObject's direction
    if (obj.GetDirection().lengthSquared() == 0) //if there's no direction (no input), character is idling
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
}

//Make the character shoot
void ControllerComponent::Shoot(GameObject& obj) {

    CalculateCharacterAnimation(obj, "shoot");

    //play a sound
    obj.NotifySoundEvent(GameObject::SoundEvent::CHARACTER_SHOOT);

    //shoot
    timeSinceLastShot_ = 0;
    characterCurrentAmmo_--;
}

//Make the character reload
void ControllerComponent::Reload(GameObject& obj) {

    //switch aniation
    CalculateCharacterAnimation(obj, "reload");

    //play a sound
    obj.NotifySoundEvent(GameObject::SoundEvent::CHARACTER_RELOAD);

    //reload
    characterCurrentAmmo_ = characterMaxAmmo_;
}


//Commands called by user input ---------------------------------------------------------------------------------------------------------------------------------------------------

//Command called from the Level instance -- check if the character is able to shoot
int ControllerComponent::ValidateShootCommand(GameObject &obj) {
    //prevent bullet spam
    if (timeSinceLastShot_ < timeBetweenShots_) return characterCurrentAmmo_;

    //prevent shooting when out of bullets (add UI message here)
    if (characterCurrentAmmo_ <= 0) return characterCurrentAmmo_;

    //prevent shooting while reloading
    if (obj.GetCurrentAnimation() == "reload") return characterCurrentAmmo_;


    //passed conditions: can shoot 
    //perform character shoot behaviour (sound and animation)
    Shoot(obj);

    //return the character's new ammo value
    return characterCurrentAmmo_;
}

//Command called from the Level instance -- make the character reload
int ControllerComponent::ValidateReloadCommand(GameObject &obj) {

    //ignore reload command when character has full ammo
    if (characterCurrentAmmo_ == characterMaxAmmo_) return characterCurrentAmmo_;


    //passed condition: can reload
    //perform character reload behaviour (sound and animation)
    Reload(obj);

    //return the character's new ammo value
    return characterCurrentAmmo_;
}

#pragma endregion

#pragma region Enemy-Specific Behaviour

//Update the enemy's current behavioural state
void ControllerComponent::UpdateEnemyState(GameObject& obj, float const deltaTime) {
    
    //if the enemy is "dead", no further behaviour updates are needed
    if (state_ == EnemyState::DEAD) return;


    //check if enemy "died" this frame
    if (obj.CheckHitByBullet())
    {
        //perform death actions
        Die(obj);

        //notify the GameObject that this enemy is "dead"
        obj.SetEntityDead();

        //reset the notification so this doesn't trigger multiple times
        obj.NotifyHitByBullet(false);
    }


    //update movement
    if (state_ == EnemyState::WALKING)
    {
        randomTurnTimer_ -= deltaTime;

        //if enemy randomly decides to turn or collides with a wall
        if (randomTurnTimer_ <= 0 || obj.CheckHorizontalCollision() || obj.CheckVerticalCollision())
        {
            //stop moving
            Stop(obj);

            //reset the collision bools and turn timer so this doesn't trigger multiple times
            obj.NotifyHorizontalCollision(false);
            obj.NotifyVerticalCollision(false);
            randomTurnTimer_ = (rand() % 5) + 2;
        }
    }
    else if (state_ == EnemyState::PAUSED)
    {
        //pause for a few seconds before changing direction
        pauseTimer_ -= deltaTime;

        if (pauseTimer_ <= 0)
        {
            Turn(obj);
        }
    }
}

//"Kill" the enemy
void ControllerComponent::Die(GameObject& obj) {

    //call no further updates
    state_ = EnemyState::DEAD;

    //stop moving
    obj.SetDirection({ 0,0 });

    //tell the AudioComponent to play a sound
    obj.NotifySoundEvent(GameObject::SoundEvent::BULLET_COLLISION);
}

//Stop the enemy's movement
void ControllerComponent::Stop(GameObject& obj) {

    state_ = EnemyState::PAUSED;

    //start the pause timer before changing direction
    pauseTimer_ = (rand() % 3) + 1;

    //stop moving to prevent further collision detection
    obj.SetDirection({ 0,0 });
}

//Change the enemy's direction and re-start its movement
void ControllerComponent::Turn(GameObject& obj) {

    state_ = EnemyState::WALKING;

    //temporarily remove the current direction from the vector of directions to choose from
    for (int i = 0; i < directions_.size(); i++)
    {
        if (directions_[i] == currentDirection_)
        {
            directions_.erase(directions_.begin() + i);
            break;
        }
    }

    //pick a direction at random
    obj.SetDirection(directions_[rand() % 3]);

    //return the removed direction to the vector
    directions_.push_back(currentDirection_);

    //update the currentDirection_ variable and the GameObject's rotation
    currentDirection_ = obj.GetDirection();
    obj.SetRotation(currentDirection_.angle());
}

//Update the enemy GameObject's animation stack based on what state the enemy is in
void ControllerComponent::CalculateEnemyAnimation(GameObject& obj) {
    switch (state_)
    {
    case EnemyState::WALKING:
        if (obj.GetCurrentAnimation() != "walk")
        {
            obj.AddAnimationToStack("walk", 0);
        }
        break;

    case EnemyState::PAUSED:
        if (obj.GetCurrentAnimation() != "idle")
        {
            obj.RemoveAnimationFromStack();
        }
        break;

    case EnemyState::DEAD:
        if (obj.GetCurrentAnimation() != "die")
        {
            obj.AddAnimationToStack("die", 3);
        }
        break;

    default:
        break;
    }
}

#pragma endregion

#pragma region Bullet-Specific Behaviour

//Update the bullet's direction and bounce counter
void ControllerComponent::BulletSolidCollision(GameObject &obj, bool const horizontal) {

    //flip the appropriate direction axis
    if (horizontal) obj.SetDirection({ obj.GetDirection().x * -1.f, obj.GetDirection().y });
    else obj.SetDirection({ obj.GetDirection().x, obj.GetDirection().y * -1.f });

    //play a sound
    obj.NotifySoundEvent(GameObject::SoundEvent::SOLID_COLLISION);

    //increase the bullet counter; if it's higher than maxBounces_, destroy the bullet
    obj.SetBulletBounceCount(obj.GetBulletBounceCount() + 1);
    if (obj.GetBulletBounceCount() > maxBounces_) obj.Destroy();
}

#pragma endregion