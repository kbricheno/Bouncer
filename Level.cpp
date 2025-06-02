#include "Level.h"

#include <iostream>

Level::Level(sf::RenderWindow* const window, const int levelId, const int tileSize, const int levelWidth, const int levelHeight, const std::vector<char> &levelPlan) {
	levelId_ = levelId;
	window_ = window;

	GenerateTexturesAndSprites();
	GenerateLevel(tileSize, levelWidth, levelHeight, levelPlan);
}

//create all Textures & Sprites, store them in vector (here rather than on a GameObject or Component so we only create 1 set of each Texture/Sprite for the entire level)
void Level::GenerateTexturesAndSprites() {
	
	//player animations .....................................................................................................................................................
	//create vectors representing individual animations -- each will hold a number of frames (images) making up the animation
	std::vector<sf::Texture> characterIdleTextures_;
	std::vector<sf::Texture> characterWalkTextures_;
	std::vector<sf::Texture> characterShootTextures_;
	std::vector<sf::Texture> characterReloadTextures_;

	//create a string we can use to generate each frame's file path (frames are saved as numbers in a folder)
	std::string fileName = "";

	//loop through the number of frames in the first animation, idle
	for (int i = 1; i <= 10; i++)
	{
		//create the frame's file path using the iterator to determine the current frame
		fileName = "Assets/Player/idle/" + std::to_string(i) + ".png";

		//create a Texture using the generated file path and immediately store it in the relevant animation vector
		characterIdleTextures_.push_back(sf::Texture(fileName));
	}

	//store the animation in the Level's vector of player animations
	characterAnimations_.push_back(characterIdleTextures_);

	//repeat the process for the walking animation
	for (int i = 1; i <= 5; i++)
	{
		fileName = "Assets/Player/walk/" + std::to_string(i) + ".png";
		characterWalkTextures_.push_back(sf::Texture(fileName));
	}
	characterAnimations_.push_back(characterWalkTextures_);

	//shooting
	for (int i = 1; i <= 3; i++)
	{
		fileName = "Assets/Player/shoot/" + std::to_string(i) + ".png";
		characterShootTextures_.push_back(sf::Texture(fileName));
	}
	characterAnimations_.push_back(characterShootTextures_);

	//reloading
	for (int i = 1; i <= 4; i++)
	{
		fileName = "Assets/Player/reload/" + std::to_string(i) + ".png";
		characterReloadTextures_.push_back(sf::Texture(fileName));
	}
	characterAnimations_.push_back(characterReloadTextures_);


	//enemy animations .....................................................................................................................................................
	std::vector<sf::Texture> enemyWalkTextures_;
	std::vector<sf::Texture> enemyDeadTextures_;

	//walking
	for (int i = 1; i <= 10; i++)
	{
		fileName = "Assets/Guard/walk/" + std::to_string(i) + ".png";
		enemyWalkTextures_.push_back(sf::Texture(fileName));
	}
	enemyAnimations_.push_back(enemyWalkTextures_);

	//dead
	for (int i = 1; i <= 4; i++)
	{
		fileName = "Assets/Guard/death/" + std::to_string(i) + ".png";
		enemyDeadTextures_.push_back(sf::Texture(fileName));
	}
	enemyAnimations_.push_back(enemyDeadTextures_);


	//bullet Texture .....................................................................................................................................................
	//bullets only have one animation containing one Texture, but must use the same template as other entities for the contextless VisualComponent to work
	std::vector<sf::Texture> bulletIdleTextures_;
	bulletIdleTextures_.push_back(sf::Texture("Assets/Player/bullet.png"));
	bulletAnimations_.push_back(bulletIdleTextures_);

	//door animations .....................................................................................................................................................


	//wall Texture .....................................................................................................................................................
	std::vector<sf::Texture> wallIdleTextures_;
	wallIdleTextures_.push_back(sf::Texture("Assets/Environment/wall.png"));
	wallAnimations_.push_back(wallIdleTextures_);
}

void Level::GenerateLevel(const int tileSize, const int levelWidth, const int levelHeight, const std::vector<char>& levelPlan) {

	//create all the level GameObjects and Components
	int currentX = 0, currentY = 0;
	for (int i = 0; i < levelPlan.size() - 1; i++)
	{
		sf::Vector2f position(currentX * tileSize, currentY * tileSize);

		//spawn wall
		if (levelPlan[i] == 'W')
		{
			//create GameObject, pass in its id and position
			//the id will be used to locate its Components when needed
			GameObject wallObj(currentObjectId, position, (sf::Vector2f)wallAnimations_[0][0].getSize());

			//create VisualComponent, pass in the id needed to identify its GameObject, a pointer to the window, and a reference to its Textures
			//create PhysicsComponent, pass in its id and the type of collider it should have
			VisualComponent vComp(currentObjectId, window_, wallAnimations_);
			PhysicsComponent pComp(currentObjectId, PhysicsComponent::ColliderType::WALL);

			//store the GameObject and Components in this Level instance
			gameObjects_.push_back(wallObj);
			visualComponents_.push_back(vComp);
			physicsComponents_.push_back(pComp);
		}

		//spawn character
		else if (levelPlan[i] == 'P') {
			//create GameObject
			GameObject characterObj(currentObjectId, position, (sf::Vector2f)characterAnimations_[0][0].getSize());

			//create Components
			CharacterController cComp(currentObjectId, window_);
			VisualComponent vComp(currentObjectId, window_, characterAnimations_);
			PhysicsComponent pComp(currentObjectId, PhysicsComponent::ColliderType::PLAYER, 500.f);

			//store everything
			gameObjects_.push_back(characterObj);
			characterControllers_.push_back(cComp);
			visualComponents_.push_back(vComp);
			physicsComponents_.push_back(pComp);
		}

		//spawn enemy
		else if (levelPlan[i] == 'E') {
			//create GameObject
			GameObject enemyObj(currentObjectId, position, (sf::Vector2f)enemyAnimations_[0][0].getSize());

			//create Components
			EnemyController cComp(currentObjectId);
			VisualComponent vComp(currentObjectId, window_, enemyAnimations_);
			PhysicsComponent pComp(currentObjectId, PhysicsComponent::ColliderType::ENEMY, 250.f);

			//store everything
			gameObjects_.push_back(enemyObj);
			enemyControllers_.push_back(cComp);
			visualComponents_.push_back(vComp);
			physicsComponents_.push_back(pComp);
		}

		//spawn door


		//spawn background

		currentObjectId++;

		if (currentX >= levelWidth - 1) {
			currentY++;
			currentX = 0;
		}
		else currentX++;
	}
}

void Level::SpawnBullet(sf::Vector2f const startPos, sf::Vector2f const startDir) {
	//create GameObject
	GameObject bulletObj(currentObjectId, startPos, (sf::Vector2f)bulletAnimations_[0][0].getSize(), startDir);

	//create Components
	BulletController cComp(currentObjectId);
	VisualComponent vComp(currentObjectId, window_, bulletAnimations_);
	PhysicsComponent pComp(currentObjectId, PhysicsComponent::ColliderType::BULLET, 1000.f);

	//store everything
	gameObjects_.push_back(bulletObj);
	bulletControllers_.push_back(cComp);
	visualComponents_.push_back(vComp);
	physicsComponents_.push_back(pComp);

	currentObjectId++;
}

#pragma region Gameplay Loop

void Level::HandleInput(float const deltaTime) {
	//update character

	for (int o = 0; o < gameObjects_.size(); o++)
	{
		if (gameObjects_[o].GetGameObjectId() == characterControllers_[0].GetGameObjectId())
		{
			if (characterControllers_[0].HandleInput(gameObjects_[o], deltaTime)) //this is really bad, need to think of a better way to spawn bullets
			{
				sf::Angle startAngle = gameObjects_[o].GetRotation();
				sf::Vector2f startDir = { cos(startAngle.asRadians()), sin(startAngle.asRadians()) };

				SpawnBullet(gameObjects_[o].GetCenter(), startDir);
			}			
			break;
		}
	}



	//update enemies
	for (int e = 0; e < enemyControllers_.size(); e++)
	{
		for (int o = 0; o < gameObjects_.size(); o++)
		{
			if (gameObjects_[o].GetGameObjectId() == enemyControllers_[e].GetGameObjectId())
			{
				enemyControllers_[e].HandleInput(gameObjects_[o], deltaTime);
			}
		}
	}

	//update bullets
	for (int b = 0; b < bulletControllers_.size(); b++)
	{
		for (int o = 0; o < gameObjects_.size(); o++)
		{
			if (gameObjects_[o].GetGameObjectId() == bulletControllers_[b].GetGameObjectId())
			{
				bulletControllers_[b].HandleInput(gameObjects_[o], deltaTime);
			}
		}
	}
}


void Level::Update(float const deltaTime) {

	//remove dead objects
	std::vector<int> indexesToBeDeleted;

	for (int o = 0; o < gameObjects_.size(); o++)
	{
		if (gameObjects_[o].CheckDead()) {

			indexesToBeDeleted.push_back(o);
			int correctId = gameObjects_[o].GetGameObjectId();

			//loop through each Component, identify any Components belonging to the dead GameObject using their id and remove them from their vectors
			for (int c = 0; c < characterControllers_.size(); c++)
			{
				if (characterControllers_[c].GetGameObjectId() == correctId) 
				{
					characterControllers_.erase(characterControllers_.begin() + c);
					break;
				}
			}
			for (int b = 0; b < bulletControllers_.size(); b++)
			{
				if (bulletControllers_[b].GetGameObjectId() == correctId)
				{
					bulletControllers_.erase(bulletControllers_.begin() + b);
					break;
				}
			}

			for (int p = 0; p < physicsComponents_.size(); p++)
			{
				if (physicsComponents_[p].GetGameObjectId() == correctId)
				{
					physicsComponents_.erase(physicsComponents_.begin() + p);
					break;
				}
			}

			for (int v = 0; v < visualComponents_.size(); v++)
			{
				if (visualComponents_[v].GetGameObjectId() == correctId)
				{
					visualComponents_.erase(visualComponents_.begin() + v);
					break;
				}
			}

			for (int a = 0; a < audioComponents_.size(); a++)
			{
				if (audioComponents_[a].GetGameObjectId() == correctId)
				{
					audioComponents_.erase(audioComponents_.begin() + a);
					break;
				}
			}
		}
	}

	//finally, remove the dead GameObjects from the GameObject vector
	for (int i = 0; i < indexesToBeDeleted.size(); i++)
	{
		gameObjects_.erase(gameObjects_.begin() + indexesToBeDeleted[i]);
	}


	//update all the PhysicsComponents
	for (int p = 0; p < physicsComponents_.size(); p++) //loop every PhysicsComponent
	{
		for (int o = 0; o < gameObjects_.size(); o++) //loop every GameObject
		{
			if (gameObjects_[o].GetGameObjectId() == physicsComponents_[p].GetGameObjectId()) //find the PhysicsComponent's GameObject by comparing their ids
			{
				physicsComponents_[p].Update(gameObjects_[o], deltaTime, physicsComponents_); //update the PhysicsComponent, passing it a reference to its GameObject
			}
		}
	}

	//update all the AudioComponents
	//for (int a = 0; a < audioComponents_.size(); a++)
	//{
	//	audioComponents_[a].Update();
	//}
}

void Level::Draw(float const deltaTime) {

	for (int v = 0; v < visualComponents_.size(); v++)
	{
		for (int o = 0; o < gameObjects_.size(); o++)
		{
			if (gameObjects_[o].GetGameObjectId() == visualComponents_[v].GetGameObjectId())
			{
				visualComponents_[v].Update(gameObjects_[o], deltaTime);
			}
		}
	}
}

#pragma endregion