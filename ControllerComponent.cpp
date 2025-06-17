#include "ControllerComponent.h"

void ControllerComponent::Update(GameObject &obj, float const deltaTime, sf::Vector2f inMousePos) {
    //perform specific behaviour depending on type of entity
    switch (obj.GetType())
    {
    case GameObject::EntityType::HERO:
        obj.SetDirection(CalculateHeroDirection(obj));
        obj.SetRotation(CalculateHeroRotation(obj, inMousePos));
        CalculateHeroAnimation(obj);
        m_timeSinceHeroLastShot += deltaTime;
        break;

    case GameObject::EntityType::ENEMY:
        UpdateEnemyState(obj, deltaTime);
        CalculateEnemyAnimation(obj);
        break;

    case GameObject::EntityType::BULLET:
        //the only thing bullets need to do is check if they collided with something solid
        if (obj.IsEntityCollidingHorizontally()) //bullet hit a left/right wall
        {
            //reset the notification so this doesn't trigger constantly
            obj.NotifyHorizontalCollisionEvent(false);
            BulletSolidCollision(obj, true);
        }
        else if (obj.IsEntityCollidingVertically()) //bullet hit a top/bottom wall
        {
            obj.NotifyVerticalCollisionEvent(false);
            BulletSolidCollision(obj, false);
        }
        break;

    default:
        break;
    }
}

#pragma region Hero-Specific Behaviour

sf::Vector2f ControllerComponent::CalculateHeroDirection(GameObject &obj) {
    
    //start by setting the direction values to zero
    float newDirX = 0, newDirY = 0;
    
    //add and subtract values based on which keys are pressed, which allows users to hold multiple (even opposing) move keys
    if (m_isHeroMovingUp) newDirY--;
    if (m_isHeroMovingDown) newDirY++;
    if (m_isHeroMovingLeft) newDirX--;
    if (m_isHeroMovingRight) newDirX++;

    return { newDirX, newDirY };
}

sf::Angle ControllerComponent::CalculateHeroRotation(GameObject &obj, sf::Vector2f inMousePos) {

    //calculate the vector between the player and mouse
    sf::Vector2f relativePos = { inMousePos.x - obj.GetCenter().x, inMousePos.y - (obj.GetCenter().y)};

    //convert the vector into an angle (radians)
    float lookAtMouse = atan2(relativePos.y, relativePos.x);

    //convert the angle from a float to a sf::Angle
    sf::Angle heroAngle = sf::radians(lookAtMouse);

    //update the GameObject's rotation_ variable
    return heroAngle;
}

void ControllerComponent::CalculateHeroAnimation(GameObject& obj, std::string anim) {

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
    if (obj.GetDirection().lengthSquared() == 0) //if there's no direction (no input), hero is idling
    {
        //the idling animation is 0 which for every object is always at the bottom of the animation stack, looping forever
        //if the hero is currently walking, pop the walking animation out to enable the idling animation to play
        //idling should only ever interrupt walking, not shooting or reloading (shooting/reloading don't loop so they pop themselves out of the stack when finished)
        if (obj.GetCurrentAnimation() == "walk")
        {
            obj.RemoveAnimationFromStack();
        }
    }
    else //if there's direction (input), hero is walking
    {
        //only interrupt the idling animation, not shooting or reloading
        if (obj.GetCurrentAnimation() == "idle")
        {
            obj.AddAnimationToStack("walk", 0);
        }
    }
}

void ControllerComponent::HeroShoot(GameObject& obj) {

    CalculateHeroAnimation(obj, "shoot");

    //play a sound
    obj.NotifySoundEvent(GameObject::SoundEvent::HERO_SHOOT);

    //shoot
    m_timeSinceHeroLastShot = 0;
    m_heroCurrentAmmo--;
}

void ControllerComponent::HeroReload(GameObject& obj) {

    //switch aniation
    CalculateHeroAnimation(obj, "reload");

    //play a sound
    obj.NotifySoundEvent(GameObject::SoundEvent::HERO_RELOAD);

    //reload
    m_heroCurrentAmmo = m_heroMaxAmmo;
}


int ControllerComponent::ValidateShootCommand(GameObject &obj) {
    //prevent bullet spam
    if (m_timeSinceHeroLastShot < m_timeBetweenHeroShots) return m_heroCurrentAmmo;

    //prevent shooting when out of bullets (add UI message here)
    if (m_heroCurrentAmmo <= 0) return m_heroCurrentAmmo;

    //prevent shooting while reloading
    if (obj.GetCurrentAnimation() == "reload") return m_heroCurrentAmmo;


    //passed conditions: can shoot 
    //perform hero shoot behaviour (sound and animation)
    HeroShoot(obj);

    //return the hero's new ammo value
    return m_heroCurrentAmmo;
}

int ControllerComponent::ValidateReloadCommand(GameObject &obj) {

    //ignore reload command when hero has full ammo
    if (m_heroCurrentAmmo == m_heroMaxAmmo) return m_heroCurrentAmmo;


    //passed condition: can reload
    //perform hero reload behaviour (sound and animation)
    HeroReload(obj);

    //return the hero's new ammo value
    return m_heroCurrentAmmo;
}

#pragma endregion

#pragma region Enemy-Specific Behaviour

void ControllerComponent::UpdateEnemyState(GameObject& obj, float const deltaTime) {
    
    //if the enemy is "dead", no further behaviour updates are needed
    if (m_currentEnemyState == EnemyState::DEAD) return;


    //check if enemy "died" this frame
    if (obj.IsEntityHitByBullet())
    {
        //perform death actions
        EnemyDie(obj);

        //notify the GameObject that this enemy is "dead"
        obj.SetEntityDead();

        //reset the notification so this doesn't trigger multiple times
        obj.NotifyHitByBulletEvent(false);
    }


    //update movement
    if (m_currentEnemyState == EnemyState::WALKING)
    {
        m_enemyRandomTurnTimer -= deltaTime;

        //if enemy randomly decides to turn or collides with a wall
        if (m_enemyRandomTurnTimer <= 0 || obj.IsEntityCollidingHorizontally() || obj.IsEntityCollidingVertically())
        {
            //stop moving
            EnemyStop(obj);

            //reset the collision bools and turn timer so this doesn't trigger multiple times
            obj.NotifyHorizontalCollisionEvent(false);
            obj.NotifyVerticalCollisionEvent(false);
            m_enemyRandomTurnTimer = (rand() % 5) + 2;
        }
    }
    else if (m_currentEnemyState == EnemyState::PAUSED)
    {
        //pause for a few seconds before changing direction
        m_enemyPauseTimer -= deltaTime;

        if (m_enemyPauseTimer <= 0)
        {
            EnemyTurn(obj);
        }
    }
}

void ControllerComponent::EnemyDie(GameObject& obj) {

    //call no further updates
    m_currentEnemyState = EnemyState::DEAD;

    //stop moving
    obj.SetDirection({ 0,0 });

    //tell the AudioComponent to play a sound
    obj.NotifySoundEvent(GameObject::SoundEvent::BULLET_COLLISION);
}

void ControllerComponent::EnemyStop(GameObject& obj) {

    m_currentEnemyState = EnemyState::PAUSED;

    //start the pause timer before changing direction
    m_enemyPauseTimer = (rand() % 3) + 1;

    //stop moving to prevent further collision detection
    obj.SetDirection({ 0,0 });
}

void ControllerComponent::EnemyTurn(GameObject& obj) {

    m_currentEnemyState = EnemyState::WALKING;

    //temporarily remove the current direction from the vector of directions to choose from
    for (int i = 0; i < m_enemyDirections.size(); i++)
    {
        if (m_enemyDirections[i] == m_currentEnemyDirection)
        {
            m_enemyDirections.erase(m_enemyDirections.begin() + i);
            break;
        }
    }

    //pick a direction at random
    obj.SetDirection(m_enemyDirections[rand() % 3]);

    //return the removed direction to the vector
    m_enemyDirections.push_back(m_currentEnemyDirection);

    //update the currentDirection_ variable and the GameObject's rotation
    m_currentEnemyDirection = obj.GetDirection();
    obj.SetRotation(m_currentEnemyDirection.angle());
}

void ControllerComponent::CalculateEnemyAnimation(GameObject& obj) {
    switch (m_currentEnemyState)
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

void ControllerComponent::BulletSolidCollision(GameObject &obj, bool const inIsCollisionHorizontal) {

    //flip the appropriate direction axis
    if (inIsCollisionHorizontal) obj.SetDirection({ obj.GetDirection().x * -1.f, obj.GetDirection().y });
    else obj.SetDirection({ obj.GetDirection().x, obj.GetDirection().y * -1.f });

    //play a sound
    obj.NotifySoundEvent(GameObject::SoundEvent::SOLID_COLLISION);

    //increase the bullet counter; if it's higher than maxBounces_, destroy the bullet
    obj.SetBulletBounceCount(obj.GetBulletBounceCount() + 1);
    if (obj.GetBulletBounceCount() > m_maxBulletBounces) obj.TagForDestruction();
}

#pragma endregion