#include "Level.h"
#include <SFML/Window.hpp>
#include <iostream>

#pragma region Level Control

//Create every GameObject and Component the level starts with
void Level::GenerateLevel(const int tileSize, const std::vector<char>& levelPlan, const sf::Vector2i levelSize) {

	/*
	First, spawn a background -- every level has one(and only one), and its Sprite and Textures are determined by the this level's levelId.
	*/

	//create a GameObject, pass in the type of entity, its position, and the size of its Sprite
	GameObject backgroundObj(GameObject::EntityType::BACKGROUND, { 0,0 }, sf::Sprite(allAnimations_.at("background").at(std::to_string(levelId_))[0]).getLocalBounds().size, std::to_string(levelId_));

	//create a VisualComponent, pass in a pointer to the window, a reference to its Textures (animations), and (in this case) the name of the first animation that should play
	VisualComponent vComp(window_, allAnimations_.at("background"), std::to_string(levelId_));

	//store the GameObject and VisualComponent in maps as values, using the currentObjectId as their keys
	//this allows Components to update the correct GameObject and vice versa
	gameObjects_.insert({ currentObjectId, backgroundObj });
	visualComponents_.insert({ currentObjectId, vComp });

	//finally, increase the currentObjectId, ensuring that it will be unique for each GameObject/Components set
	currentObjectId++;


	/*
	Now, we'll iterate over the level plan (a string of characters) for each entity, in the order they should be drawn in.
	This ensures that, e.g., the player is drawn on top of walls which are drawn on top of the background, etc.
	Some entities are grouped together because the order in which they are drawn does not matter (they will never overlap).
	*/

	//store ints used to position each entity in the game world
	int currentX = 0, currentY = 0;

	//spawn environment -- walls, doors
	for (int i = 0; i <= levelPlan.size() - 1; i++)
	{
		sf::Vector2f position(currentX * tileSize, currentY * tileSize);

		//spawn walls
		if (levelPlan[i] == 'W')
		{
			//create GameObject
			GameObject wallObj(GameObject::EntityType::WALL, position);

			//create VisualComponent
			//create PhysicsComponent, pass in the GameObject's type (this affects the collider and/or hitbox it will have)
			VisualComponent vComp(window_, allAnimations_.at("wall"));
			PhysicsComponent pComp(wallObj.GetType());

			//store everything
			gameObjects_.insert({ currentObjectId, wallObj });
			visualComponents_.insert({ currentObjectId, vComp });
			physicsComponents_.insert({ currentObjectId, pComp });

			//increase the id
			currentObjectId++;
		}

		//spawn doors
		else if (levelPlan[i] == 'H' || levelPlan[i] == 'V')
		{
			//create GameObject
			GameObject doorObj(GameObject::EntityType::DOOR, position);

			//create VisualComponent and PhysicsComponent
			//create AudioComponent, pass in a reference to its SoundBuffers (sound effects)
			VisualComponent vComp(window_, levelPlan[i] == 'H' ? allAnimations_.at("doorHor") : allAnimations_.at("doorVer"));
			PhysicsComponent pComp(doorObj.GetType());
			AudioComponent aComp(allSoundEffects_.at("door"));

			//store everything
			gameObjects_.insert({ currentObjectId, doorObj });
			visualComponents_.insert({ currentObjectId, vComp });
			physicsComponents_.insert({ currentObjectId, pComp });
			audioComponents_.insert({ currentObjectId, aComp });

			//increase the id
			currentObjectId++;
		}

		if (currentX >= levelSize.x - 1)
		{
			currentY++;
			currentX = 0;
		}
		else currentX++;
	}


	//reset the position ints to start the loop again
	currentX = 0;
	currentY = 0;

	//spawn enemies
	for (int i = 0; i <= levelPlan.size() - 1; i++)
	{
		sf::Vector2f position(currentX * tileSize, currentY * tileSize);

		//spawn enemies
		if (levelPlan[i] == 'E')
		{
			//create GameObject
			GameObject enemyObj(GameObject::EntityType::ENEMY, position);

			//create VisualComponent, PhysicsComponent, AudioComponent
			//create an enemy-specific ControllerComponent, passing in the currentObjectId used in srand to determine directions the enemy will move in
			ControllerComponent cComp(currentObjectId);
			VisualComponent vComp(window_, allAnimations_.at("enemy"));
			PhysicsComponent pComp(enemyObj.GetType(), 250.f);
			AudioComponent aComp(allSoundEffects_.at("enemy"));

			//store everything
			gameObjects_.insert({ currentObjectId, enemyObj });
			controllerComponents_.insert({ currentObjectId, cComp });
			visualComponents_.insert({ currentObjectId, vComp });
			physicsComponents_.insert({ currentObjectId, pComp });
			audioComponents_.insert({ currentObjectId, aComp });

			//record the number of enemies in the level
			startEnemies_++;

			//increase the id
			currentObjectId++;
		}

		if (currentX >= levelSize.x - 1)
		{
			currentY++;
			currentX = 0;
		}
		else currentX++;
	}


	//reset the position ints to start the loop again
	currentX = 0;
	currentY = 0;

	//spawn character
	for (int i = 0; i <= levelPlan.size() - 1; i++)
	{
		sf::Vector2f position(currentX * tileSize, currentY * tileSize);

		if (levelPlan[i] == 'P')
		{
			//create GameObject
			GameObject characterObj(GameObject::EntityType::CHARACTER, position);

			//create Components
			ControllerComponent cComp;
			VisualComponent vComp(window_, allAnimations_.at("character"));
			PhysicsComponent pComp(characterObj.GetType(), 500.f);
			AudioComponent aComp(allSoundEffects_.at("character"));

			//store everything
			gameObjects_.insert({ currentObjectId, characterObj });
			controllerComponents_.insert({ currentObjectId, cComp });
			visualComponents_.insert({ currentObjectId, vComp });
			physicsComponents_.insert({ currentObjectId, pComp });
			audioComponents_.insert({ currentObjectId, aComp });

			//record the character's id so user input can be passed to it easily
			characterId = currentObjectId;

			//call a reload purely to inform the level of the character's starting ammo
			ammoCount_ = cComp.ValidateReloadCommand(characterObj);

			//increase the id
			currentObjectId++;

			//there's only ever one character, so don't bother finishing the loop
			break;
		}

		if (currentX >= levelSize.x - 1)
		{
			currentY++;
			currentX = 0;
		}
		else currentX++;
	}
	levelView.setSize({ (float)tileSize * 16, (float)tileSize * 9 }); //display 16 by 9 tiles
}

//Create a new bullet
void Level::SpawnBullet(sf::Vector2f const inStartPos, sf::Vector2f const inStartDir) {

	//create GameObject
	GameObject bulletObj(GameObject::EntityType::BULLET, inStartPos, sf::Vector2f(), "idle", inStartDir);

	//create Components
	ControllerComponent cComp;
	VisualComponent vComp(window_, allAnimations_.at("bullet"));
	PhysicsComponent pComp(bulletObj.GetType(), 1000.f);
	AudioComponent aComp(allSoundEffects_.at("bullet"));

	//store everything
	gameObjects_.insert({ currentObjectId, bulletObj });
	controllerComponents_.insert({ currentObjectId, cComp });
	visualComponents_.insert({ currentObjectId, vComp });
	physicsComponents_.insert({ currentObjectId, pComp });
	audioComponents_.insert({ currentObjectId, aComp });

	//increase the id
	currentObjectId++;
}

//Remove "dead" GameObjects and their Components from storage maps
void Level::CleanUpDeadEntities() 
{
	//remove dead objects
	std::vector<int> indexesToBeDeleted;

	for (auto& [id, obj] : gameObjects_)
	{
		if (obj.IsTaggedForDestruction()) {

			indexesToBeDeleted.push_back(id);

			//loop through each Component, identify any Components belonging to the dead GameObject using their id and remove them from their maps
			if (controllerComponents_.find(id) != controllerComponents_.end())
			{
				controllerComponents_.erase(id);
			}

			if (physicsComponents_.find(id) != physicsComponents_.end())
			{
				physicsComponents_.erase(id);
			}

			if (visualComponents_.find(id) != visualComponents_.end())
			{
				visualComponents_.erase(id);
			}

			if (audioComponents_.find(id) != audioComponents_.end())
			{
				audioComponents_.erase(id);
			}
		}
	}

	//finally, remove the dead GameObjects from the GameObject map
	for (int i = 0; i < indexesToBeDeleted.size(); i++)
	{
		gameObjects_.erase(indexesToBeDeleted[i]);
	}
}

//Update the level view's position
void Level::UpdateView() {

	//obtain the player character's position
	sf::Vector2f centerPos = gameObjects_.at(characterId).GetCenter();

	//clamp that value between the size of the view and the size of the level
	if (centerPos.x < levelView.getSize().x / 2.f) centerPos.x = levelView.getSize().x / 2.f;
	if (centerPos.y < levelView.getSize().y / 2.f) centerPos.y = levelView.getSize().y / 2.f;
	if (centerPos.x > levelSizePixels_.x - levelView.getSize().x / 2.f) centerPos.x = levelSizePixels_.x - levelView.getSize().x / 2.f;
	if (centerPos.y > levelSizePixels_.y - levelView.getSize().y / 2.f) centerPos.y = levelSizePixels_.y - levelView.getSize().y / 2.f;
	
	//set the view's position to that clamped value
	levelView.setCenter(centerPos);
}

std::string Level::CurrentAmmoString() {

	return "Ammo: " + std::to_string(ammoCount_) + "/6";
}

std::string Level::EnemiesRemainingString() {
	return "Enemies remaining: " + std::to_string(enemiesAlive_);
}

#pragma endregion

#pragma region User Input Commands

//Ask the character if it's okay to shoot
void Level::CommandShoot() {
	
	//check that the character is able to shoot (if they are, the returned ammo value will be less than the ammo count stored here)
	int newAmmoCount = controllerComponents_.at(characterId).ValidateShootCommand(gameObjects_.at(characterId));
	if (newAmmoCount < ammoCount_)
	{
		sf::Angle startAngle = gameObjects_.at(characterId).GetRotation();
		sf::Vector2f startDir = { cos(startAngle.asRadians()), sin(startAngle.asRadians()) };

		SpawnBullet(gameObjects_.at(characterId).GetCenter(), startDir);
	}
	//update the Level's ammo count
	ammoCount_ = newAmmoCount;
}

//Ask the character to reload their weapon
void Level::CommandReload() {

	int newAmmoCount = controllerComponents_.at(characterId).ValidateReloadCommand(gameObjects_.at(characterId));
	ammoCount_ = newAmmoCount;
}

#pragma endregion

#pragma region Gameplay Loop

//Update every behaviour-altering Component in the level
void Level::Update(float const deltaTime) {

	//remove "dead" entities before updating anything
	CleanUpDeadEntities();

	
	//update game state altering variables (these determine game over and level clear)
	int deadEnemyCount = 0;
	for (auto& [id, obj] : gameObjects_)
	{
		//check if the player was detected or killed this frame
		if (id == characterId) 
		{
			if (!obj.CheckEntityAlive()) characterDetected_ = true;
			if (obj.CheckHitByBullet()) characterHitByBullet_ = true;
		}

		//check if this object is an enemy
		else if (obj.GetType() == GameObject::EntityType::ENEMY)
		{
			//check if this enemy is dead 
			if (!obj.CheckEntityAlive()) 
			{
				//increase the dead enemy counter
				deadEnemyCount++;
				//check if this dead enemy has been detected by another enemy's vision circle
				if (obj.CheckDeadEnemyDetected()) bodyDetected = true;
			}
		}
	}
	//update the number of enemies left alive
	enemiesAlive_ = startEnemies_ - deadEnemyCount;


	//update every ControllerComponent
	//get the mouse position in the window, convert it to world space relative to the level view
	sf::Vector2f mousePos = window_.mapPixelToCoords(sf::Mouse::getPosition(), levelView);
	for (auto& [id, cComp] : controllerComponents_)
	{
		//special treatment: only the character needs the mouse position to update its rotation, but may as well pass it to everyone
		cComp.Update(gameObjects_.at(id), deltaTime, mousePos);
		if (cComp.GetEnemyState() == ControllerComponent::EnemyState::DEAD) deadEnemyCount++;
	}


	//update all the PhysicsComponents
	for (auto& [id, pComp] : physicsComponents_)
	{
		pComp.Update(gameObjects_.at(id), deltaTime, gameObjects_, physicsComponents_);
	}
}

//Play sounds and draw every Sprite in the level
void Level::Draw(float const deltaTime) {

	//update all the AudioComponents
	for (auto& [id, aComp] : audioComponents_)
	{
		aComp.Update(gameObjects_.at(id));
	}


	//update the view, then set the window to use the updated level view
	UpdateView();
	window_.setView(levelView);

	//update every VisualComponent
	for (auto& [id, vComp] : visualComponents_)
	{
		vComp.Update(gameObjects_.at(id), deltaTime);
	}
}

#pragma endregion