#include "Level.h"

#pragma region Level Control

void Level::GenerateLevel(const int tileSize, const std::vector<char>& inLevelPlan, const sf::Vector2i inLevelSize) {

	/*
	First, spawn a background -- every level has one (and only one), and its Sprite and Textures are determined by the this level's levelId.
	*/

	//create a GameObject, pass in the type of entity, its position, and the size of its Sprite
	GameObject backgroundObj(GameObject::EntityType::BACKGROUND, { 0,0 }, sf::Sprite(m_allAnimationsRef.at("background").at(std::to_string(m_levelId))[0]).getLocalBounds(), std::to_string(m_levelId));

	//create a VisualComponent, pass in a reference to the window, a reference to its Textures (animations), and the name of the first animation that should play
	VisualComponent vComp(m_windowRef, m_allAnimationsRef.at("background"), std::to_string(m_levelId));

	//store the GameObject and VisualComponent in maps as values, using the currentObjectId as their keys
	//this allows Components to read from and write to the correct GameObject when they are updated
	m_gameObjects.insert({ m_currentObjectId, backgroundObj });
	m_visualComponents.insert({ m_currentObjectId, vComp });

	//finally, increase the currentObjectId, ensuring that it will be unique for each GameObject/Components set
	m_currentObjectId++;


	/*
	Now, iterate over the level plan (a string of characters) for each entity, in the order they should be drawn in.
	This ensures that, e.g., the player is drawn on top of walls which are drawn on top of the background, etc.
	Some entities are grouped together because the order in which they are drawn does not matter (they will never overlap).
	*/

	//create ints used as the spawn position for each entity in the game world
	int currentX = 0, currentY = 0;

	//spawn environment -- walls, doors
	for (int i = 0; i <= inLevelPlan.size() - 1; i++)
	{
		sf::Vector2f position(currentX * tileSize, currentY * tileSize);

		//spawn walls
		if (inLevelPlan[i] == 'W')
		{
			//create GameObject
			GameObject wallObj(GameObject::EntityType::WALL, position);

			//create VisualComponent
			VisualComponent vComp(m_windowRef, m_allAnimationsRef.at("wall"));
			//create PhysicsComponent, pass in the GameObject's type (this affects the collider and/or hitbox it will have)
			PhysicsComponent pComp(wallObj.GetType());

			//store everything
			m_gameObjects.insert({ m_currentObjectId, wallObj });
			m_visualComponents.insert({ m_currentObjectId, vComp });
			m_physicsComponents.insert({ m_currentObjectId, pComp });

			//increase the id
			m_currentObjectId++;
		}

		//spawn doors
		else if (inLevelPlan[i] == 'H' || inLevelPlan[i] == 'V')
		{
			//create GameObject
			GameObject doorObj(GameObject::EntityType::DOOR, position);

			//create VisualComponent and PhysicsComponent
			VisualComponent vComp(m_windowRef, inLevelPlan[i] == 'H' ? m_allAnimationsRef.at("doorHor") : m_allAnimationsRef.at("doorVer"));
			PhysicsComponent pComp(doorObj.GetType());
			//create AudioComponent, pass in a reference to its SoundBuffers (sound effects)
			AudioComponent aComp(m_allSoundBuffersRef.at("door"));

			//store everything
			m_gameObjects.insert({ m_currentObjectId, doorObj });
			m_visualComponents.insert({ m_currentObjectId, vComp });
			m_physicsComponents.insert({ m_currentObjectId, pComp });
			m_audioComponents.insert({ m_currentObjectId, aComp });

			//increase the id
			m_currentObjectId++;
		}

		//increment the spawn position
		if (currentX >= inLevelSize.x - 1)
		{
			currentY++;
			currentX = 0;
		}
		else currentX++;
	}


	//reset the spawn position to start the loop again
	currentX = 0;
	currentY = 0;

	//spawn enemies
	for (int i = 0; i <= inLevelPlan.size() - 1; i++)
	{
		sf::Vector2f position(currentX * tileSize, currentY * tileSize);

		//spawn enemies
		if (inLevelPlan[i] == 'E')
		{
			//create GameObject
			GameObject enemyObj(GameObject::EntityType::ENEMY, position);

			//create VisualComponent, PhysicsComponent, AudioComponent
			//create a ControllerComponent, passing in its object id that will seed rand to determine directions the enemy will move in
			VisualComponent vComp(m_windowRef, m_allAnimationsRef.at("enemy"));
			PhysicsComponent pComp(enemyObj.GetType(), 250.f);
			AudioComponent aComp(m_allSoundBuffersRef.at("enemy"));
			ControllerComponent cComp(m_currentObjectId);

			//store everything
			m_gameObjects.insert({ m_currentObjectId, enemyObj });
			m_visualComponents.insert({ m_currentObjectId, vComp });
			m_physicsComponents.insert({ m_currentObjectId, pComp });
			m_audioComponents.insert({ m_currentObjectId, aComp });
			m_controllerComponents.insert({ m_currentObjectId, cComp });

			//record the number of enemies in the level
			m_startEnemies++;

			//increase the id
			m_currentObjectId++;
		}

		if (currentX >= inLevelSize.x - 1)
		{
			currentY++;
			currentX = 0;
		}
		else currentX++;
	}


	//reset the position ints to start the loop again
	currentX = 0;
	currentY = 0;

	//spawn hero
	for (int i = 0; i <= inLevelPlan.size() - 1; i++)
	{
		sf::Vector2f position(currentX * tileSize, currentY * tileSize);

		if (inLevelPlan[i] == 'P')
		{
			//create GameObject
			GameObject heroObj(GameObject::EntityType::HERO, position);

			//create Components
			ControllerComponent cComp;
			VisualComponent vComp(m_windowRef, m_allAnimationsRef.at("hero"));
			PhysicsComponent pComp(heroObj.GetType(), 500.f);
			AudioComponent aComp(m_allSoundBuffersRef.at("hero"));

			//store everything
			m_gameObjects.insert({ m_currentObjectId, heroObj });
			m_controllerComponents.insert({ m_currentObjectId, cComp });
			m_visualComponents.insert({ m_currentObjectId, vComp });
			m_physicsComponents.insert({ m_currentObjectId, pComp });
			m_audioComponents.insert({ m_currentObjectId, aComp });

			//record the hero's id so input commands can be passed to it easily
			m_heroId = m_currentObjectId;

			//call a reload purely to inform the level of the hero's starting ammo
			m_ammoCount = cComp.ValidateReloadCommand(heroObj);

			//increase the id
			m_currentObjectId++;

			//there's only ever one hero, so don't bother finishing the loop
			break;
		}

		if (currentX >= inLevelSize.x - 1)
		{
			currentY++;
			currentX = 0;
		}
		else currentX++;
	}

	//set up the level view size
	m_levelView.setSize({ (float)tileSize * 16, (float)tileSize * 9 }); //display 16 by 9 tiles
}

void Level::SpawnBullet(sf::Vector2f const inStartPos, sf::Vector2f const inStartDir) {

	//create GameObject
	GameObject bulletObj(GameObject::EntityType::BULLET, inStartPos, inStartDir);

	//create Components
	ControllerComponent cComp;
	VisualComponent vComp(m_windowRef, m_allAnimationsRef.at("bullet"));
	PhysicsComponent pComp(bulletObj.GetType(), 1000.f);
	AudioComponent aComp(m_allSoundBuffersRef.at("bullet"));

	//store everything
	m_gameObjects.insert({ m_currentObjectId, bulletObj });
	m_controllerComponents.insert({ m_currentObjectId, cComp });
	m_visualComponents.insert({ m_currentObjectId, vComp });
	m_physicsComponents.insert({ m_currentObjectId, pComp });
	m_audioComponents.insert({ m_currentObjectId, aComp });

	//increase the id
	m_currentObjectId++;
}

void Level::CleanUpDeadEntities() 
{
	//keep a vector of ids of all the dead entities
	std::vector<int> deletionIds;

	//loop through every GameObject
	for (auto& [id, obj] : m_gameObjects)
	{
		if (obj.IsTaggedForDestruction()) {

			deletionIds.push_back(id);

			//loop through each Component, identify any Components belonging to the dead GameObject using their id and remove them from their maps
			if (m_controllerComponents.find(id) != m_controllerComponents.end())
			{
				m_controllerComponents.erase(id);
			}

			if (m_physicsComponents.find(id) != m_physicsComponents.end())
			{
				m_physicsComponents.erase(id);
			}

			if (m_visualComponents.find(id) != m_visualComponents.end())
			{
				m_visualComponents.erase(id);
			}

			if (m_audioComponents.find(id) != m_audioComponents.end())
			{
				m_audioComponents.erase(id);
			}
		}
	}

	//finally, remove the dead GameObjects from the GameObject map
	for (int i = 0; i < deletionIds.size(); i++)
	{
		m_gameObjects.erase(deletionIds[i]);
	}
}

void Level::UpdateView() {

	//use the hero's position as the center of the view
	sf::Vector2f centerPos = m_gameObjects.at(m_heroId).GetCenter();

	//clamp that value between the size of the view and the size of the level
	if (centerPos.x < m_levelView.getSize().x / 2.f) centerPos.x = m_levelView.getSize().x / 2.f;
	if (centerPos.y < m_levelView.getSize().y / 2.f) centerPos.y = m_levelView.getSize().y / 2.f;
	if (centerPos.x > m_levelSizePixels.x - m_levelView.getSize().x / 2.f) centerPos.x = m_levelSizePixels.x - m_levelView.getSize().x / 2.f;
	if (centerPos.y > m_levelSizePixels.y - m_levelView.getSize().y / 2.f) centerPos.y = m_levelSizePixels.y - m_levelView.getSize().y / 2.f;
	
	//set the view's position to that clamped value
	m_levelView.setCenter(centerPos);
}

#pragma endregion

#pragma region User Input Commands

void Level::CommandShoot() {
	
	//check that the hero is able to shoot (if they are, the returned ammo value will be less than the ammo count stored here)
	int newAmmoCount = m_controllerComponents.at(m_heroId).ValidateShootCommand(m_gameObjects.at(m_heroId));
	if (newAmmoCount < m_ammoCount)
	{
		//determine the starting direction for the resulting bullet
		sf::Angle startAngle = m_gameObjects.at(m_heroId).GetRotation();
		sf::Vector2f startDir = { cos(startAngle.asRadians()), sin(startAngle.asRadians()) };

		//spawn a bullet
		SpawnBullet(m_gameObjects.at(m_heroId).GetCenter(), startDir);
	}
	//update the Level instance's ammo count
	m_ammoCount = newAmmoCount;
}

void Level::CommandReload() {
	//tell the hero to reload and store the returned value as the ammo count
	m_ammoCount = m_controllerComponents.at(m_heroId).ValidateReloadCommand(m_gameObjects.at(m_heroId));
}

#pragma endregion

#pragma region Gameplay Loop

void Level::Update(float const deltaTime) {

	//remove "dead" entities before updating anything
	CleanUpDeadEntities();

	
	//update game-state altering variables (these determine game over and level clear)
	int deadEnemyCount = 0;
	for (auto& [id, obj] : m_gameObjects)
	{
		//check if the player was detected or killed this frame
		if (id == m_heroId) 
		{
			if (!obj.IsEntityAlive()) m_isHeroDetected = true; //game over
			if (obj.IsEntityHitByBullet()) m_isHeroHitByBullet = true; //game over
		}

		//check if this object is an enemy
		else if (obj.GetType() == GameObject::EntityType::ENEMY)
		{
			//check if this enemy is dead 
			if (!obj.IsEntityAlive()) 
			{
				//check if this dead enemy has been detected by another enemy's vision circle
				if (obj.IsDeadEnemyDetected()) m_isBodyDetected = true; //game over

				//increase the dead enemy counter
				deadEnemyCount++;
			}
		}
	}
	//update the number of enemies left alive
	m_enemiesAlive = m_startEnemies - deadEnemyCount;


	//update every ControllerComponent
	//get the mouse position in the window, convert it to world space relative to the level view
	sf::Vector2f mousePos = m_windowRef.mapPixelToCoords(sf::Mouse::getPosition(), m_levelView);
	for (auto& [id, cComp] : m_controllerComponents)
	{
		//special treatment: only the hero needs the mouse position to update its rotation, but may as well pass it to everyone
		cComp.Update(m_gameObjects.at(id), deltaTime, mousePos);
	}


	//update all the PhysicsComponents
	for (auto& [id, pComp] : m_physicsComponents)
	{
		pComp.Update(m_gameObjects.at(id), deltaTime, m_gameObjects, m_physicsComponents);
	}
}

void Level::Draw(float const deltaTime) {

	//update all the AudioComponents
	for (auto& [id, aComp] : m_audioComponents)
	{
		aComp.Update(m_gameObjects.at(id));
	}


	//update the view, then set the window to use the updated level view
	UpdateView();
	m_windowRef.setView(m_levelView);

	//update every VisualComponent
	for (auto& [id, vComp] : m_visualComponents)
	{
		vComp.Update(m_gameObjects.at(id), deltaTime);
	}
}

#pragma endregion