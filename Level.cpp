#include "Level.h"

#include <iostream>

//create all of this level's GameObjects and Components
void Level::GenerateLevel(const int tileSize, const int levelWidth, const int levelHeight, const std::vector<char>& levelPlan) {

	/*
	First, spawn a background -- every level has one(and only one), and its Sprite and Textures are determined by the this level's levelId.
	*/

	//create a GameObject, pass in the type of entity, its position, and the size of its Sprite
	GameObject backgroundObj(GameObject::EntityType::BACKGROUND, { 0,0 }, (sf::Vector2f)allAnimations_.at("background").at(std::to_string(levelId_))[0].getSize());

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
			GameObject wallObj(GameObject::EntityType::WALL, position, (sf::Vector2f)allAnimations_.at("wall").at("idle")[0].getSize());

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
			GameObject doorObj(GameObject::EntityType::DOOR, position, (sf::Vector2f)allAnimations_.at("doorHor").at("idle")[0].getSize());

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

		if (currentX >= levelWidth - 1) 
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
			GameObject enemyObj(GameObject::EntityType::ENEMY, position, (sf::Vector2f)allAnimations_.at("enemy").at("idle")[0].getSize());

			//create VisualComponent, PhysicsComponent, AudioComponent
			//create an enemy-specific ControllerComponent, passing in the currentObjectId used in srand to determine directions the enemy will move in
			EnemyController cComp(currentObjectId);
			VisualComponent vComp(window_, allAnimations_.at("enemy"));
			PhysicsComponent pComp(enemyObj.GetType(), 250.f);
			AudioComponent aComp(allSoundEffects_.at("enemy"));

			//store everything
			gameObjects_.insert({ currentObjectId, enemyObj });
			enemyControllers_.insert({ currentObjectId, cComp });
			visualComponents_.insert({ currentObjectId, vComp });
			physicsComponents_.insert({ currentObjectId, pComp });
			audioComponents_.insert({ currentObjectId, aComp });
			
			//increase the id
			currentObjectId++;
		}

		if (currentX >= levelWidth - 1) 
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
			GameObject characterObj(GameObject::EntityType::CHARACTER, position, (sf::Vector2f)allAnimations_.at("character").at("idle")[0].getSize());

			//create Components
			CharacterController cComp(window_);
			VisualComponent vComp(window_, allAnimations_.at("character"));
			PhysicsComponent pComp(characterObj.GetType(), 500.f);
			AudioComponent aComp(allSoundEffects_.at("character"));

			//store everything
			gameObjects_.insert({ currentObjectId, characterObj });
			characterControllers_.insert({ currentObjectId, cComp });
			visualComponents_.insert({ currentObjectId, vComp });
			physicsComponents_.insert({ currentObjectId, pComp });
			audioComponents_.insert({ currentObjectId, aComp });

			//increase the id
			currentObjectId++;

			//there's only ever one character, so don't bother finishing the loop
			break;
		}

		if (currentX >= levelWidth - 1)
		{
			currentY++;
			currentX = 0;
		}
		else currentX++;
	}
}

void Level::SpawnBullet(sf::Vector2f const startPos, sf::Vector2f const startDir) {
	
	//create GameObject
	GameObject bulletObj(GameObject::EntityType::BULLET, startPos, (sf::Vector2f)allAnimations_.at("bullet").at("idle")[0].getSize(), startDir);

	//create Components
	BulletController cComp;
	VisualComponent vComp(window_, allAnimations_.at("bullet"));
	PhysicsComponent pComp(bulletObj.GetType(), 1000.f);
	AudioComponent aComp(allSoundEffects_.at("bullet"));

	//store everything
	gameObjects_.insert({ currentObjectId, bulletObj });
	bulletControllers_.insert({ currentObjectId, cComp });
	visualComponents_.insert({ currentObjectId, vComp });
	physicsComponents_.insert({ currentObjectId, pComp });
	audioComponents_.insert({ currentObjectId, aComp });

	//increase the id
	currentObjectId++;
}

void Level::CleanUpDeadObjects() 
{
	//remove dead objects
	std::vector<int> indexesToBeDeleted;

	for (auto& [id, obj] : gameObjects_)
	{
		if (obj.CheckDead()) {

			indexesToBeDeleted.push_back(id);

			//loop through each Component, identify any Components belonging to the dead GameObject using their id and remove them from their maps
			if (characterControllers_.find(id) != characterControllers_.end()) 
			{
				characterControllers_.erase(id);
			}

			if (bulletControllers_.find(id) != bulletControllers_.end())
			{
				bulletControllers_.erase(id);
			}

			if (enemyControllers_.find(id) != enemyControllers_.end())
			{
				enemyControllers_.erase(id);
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

#pragma region Gameplay Loop

void Level::HandleInput(float const deltaTime) {
	
	//update character
	for (auto& [id, cComp] : characterControllers_)
	{
		if (cComp.HandleInput(gameObjects_.at(id), deltaTime)) //this is really bad, need to think of a better way to spawn bullets
		{
			sf::Angle startAngle = gameObjects_.at(id).GetRotation();
			sf::Vector2f startDir = { cos(startAngle.asRadians()), sin(startAngle.asRadians()) };

			SpawnBullet(gameObjects_.at(id).GetCenter(), startDir);
		}
	}

	//update enemies
	for (auto& [id, cComp] : enemyControllers_)
	{
		cComp.HandleInput(gameObjects_.at(id), deltaTime);
	}

	//update bullets
	for (auto& [id, cComp] : bulletControllers_)
	{	
		cComp.HandleInput(gameObjects_.at(id), deltaTime);
	}
}


void Level::Update(float const deltaTime) {

	CleanUpDeadObjects();

	//update all the PhysicsComponents
	for (auto& [id, pComp] : physicsComponents_)
	{
		pComp.Update(gameObjects_.at(id), deltaTime, physicsComponents_);
	}

	//update all the AudioComponents
	for (auto& [id, aComp] : audioComponents_)
	{
		aComp.Update(gameObjects_.at(id));
	}
}

void Level::Draw(float const deltaTime) {

	//draw every VisualComponent's sprite
	for (auto& [id, vComp] : visualComponents_)
	{
		vComp.Update(gameObjects_.at(id), deltaTime);
	}
}

#pragma endregion