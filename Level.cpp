#include "Level.h"

#include <iostream>

Level::Level(sf::RenderWindow* const window, const int levelId, const int tileSize, const int levelWidth, const int levelHeight, const std::vector<char> &levelPlan) {
	levelId_ = levelId;
	window_ = window;

	GenerateTextures();
	if (!GenerateSoundEffects()) std::cout << "ERROR: Could not find all sound effect file paths!\n";
	
	GenerateLevel(tileSize, levelWidth, levelHeight, levelPlan);
}

//create all Textures, store them in vectors (here rather than on a GameObject or Component so we only create 1 set of each Texture for the entire level)
//TODO: move this to the GameManager class; we only need 1 set of these for the entire *game*, no need to re-create them each level
//TODO: error handling for if a file is not found/loaded
void Level::GenerateTextures() {
	
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
	std::vector<sf::Texture> enemyDyingTextures_;

	//walking
	for (int i = 1; i <= 10; i++)
	{
		fileName = "Assets/Guard/walk/" + std::to_string(i) + ".png";
		enemyWalkTextures_.push_back(sf::Texture(fileName));
	}
	enemyAnimations_.push_back(enemyWalkTextures_);

	//dying
	for (int i = 1; i <= 4; i++)
	{
		fileName = "Assets/Guard/dying/" + std::to_string(i) + ".png";
		enemyDyingTextures_.push_back(sf::Texture(fileName));
	}
	enemyAnimations_.push_back(enemyDyingTextures_);


	//bullet Texture .....................................................................................................................................................
	//bullets only have one animation containing one Texture, but must use the same template as other entities for the contextless VisualComponent to work
	std::vector<sf::Texture> bulletIdleTextures_;
	bulletIdleTextures_.push_back(sf::Texture("Assets/Player/bullet.png"));
	bulletAnimations_.push_back(bulletIdleTextures_);


	//door animations .....................................................................................................................................................
	std::vector<sf::Texture> horizontalDoorClosedTextures_;
	horizontalDoorClosedTextures_.push_back(sf::Texture("Assets/Environment/doorHor.png"));
	doorHorAnimations_.push_back(horizontalDoorClosedTextures_);

	std::vector<sf::Texture> horizontalDoorOpenTextures_;	
	horizontalDoorOpenTextures_.push_back(sf::Texture("Assets/Environment/doorHorBroken.png"));
	doorHorAnimations_.push_back(horizontalDoorOpenTextures_);

	std::vector<sf::Texture> verticalDoorClosedTextures_;
	verticalDoorClosedTextures_.push_back(sf::Texture("Assets/Environment/doorVer.png"));
	doorVerAnimations_.push_back(verticalDoorClosedTextures_);

	std::vector<sf::Texture> verticalDoorOpenTextures_;
	verticalDoorOpenTextures_.push_back(sf::Texture("Assets/Environment/doorVerBroken.png"));
	doorVerAnimations_.push_back(verticalDoorOpenTextures_);


	//wall Texture .....................................................................................................................................................
	std::vector<sf::Texture> wallIdleTextures_;
	wallIdleTextures_.push_back(sf::Texture("Assets/Environment/wall.png"));
	wallAnimations_.push_back(wallIdleTextures_);


	//background Textures .....................................................................................................................................................
	std::vector<sf::Texture> background1Texture_;
	background1Texture_.push_back(sf::Texture("Assets/Environment/map1.png"));
	backgroundAnimations_.push_back(background1Texture_);

	std::vector<sf::Texture> background2Texture_;
	background1Texture_.push_back(sf::Texture("Assets/Environment/map2.png"));
	backgroundAnimations_.push_back(background2Texture_);

	std::vector<sf::Texture> background3Texture_;
	background1Texture_.push_back(sf::Texture("Assets/Environment/map3.png"));
	backgroundAnimations_.push_back(background3Texture_);

}

bool Level::GenerateSoundEffects() {

	//character sound effects
	sf::SoundBuffer characterShoot;
	sf::SoundBuffer characterReload;
	if (!characterShoot.loadFromFile("Assets/Sound/playerShoot.wav")) return false;
	if (!characterReload.loadFromFile("Assets/Sound/playerReload.wav")) return false;

	characterSoundEffects_.insert({ "shoot", characterShoot });
	characterSoundEffects_.insert({ "reload", characterReload });


	//enemy sound effects
	sf::SoundBuffer enemyDeath;
	if (!enemyDeath.loadFromFile("Assets/Sound/guardDeath.wav")) return false;

	enemySoundEffects_.insert({ "die", enemyDeath });


	//bullet sound effects
	sf::SoundBuffer bulletBounce;
	if (!bulletBounce.loadFromFile("Assets/Sound/bulletBounce.wav")) return false;

	bulletSoundEffects_.insert({ "bounce", bulletBounce });


	//door sound effects
	sf::SoundBuffer doorBreak;
	if (!doorBreak.loadFromFile("Assets/Sound/doorBreak.wav")) return false;

	doorSoundEffects_.insert({ "break", doorBreak });


	//global sound effects
	sf::SoundBuffer victory;
	if (!victory.loadFromFile("Assets/Sound/success.wav")) return false;

	globalSoundEffects.insert({ "victory", victory });

	sf::SoundBuffer detected;
	if (!detected.loadFromFile("Assets/Sound/houseAlarm.wav")) return false;

	globalSoundEffects.insert({ "detected", detected });


	//all sound buffers successfully loaded
	return true;
}

void Level::GenerateLevel(const int tileSize, const int levelWidth, const int levelHeight, const std::vector<char>& levelPlan) {

	//create all the level GameObjects and Components
	int currentX = 0, currentY = 0;
	for (int i = 0; i < levelPlan.size() - 1; i++)
	{
		sf::Vector2f position(currentX * tileSize, currentY * tileSize);

		//spawn walls
		if (levelPlan[i] == 'W')
		{
			//create GameObject, pass in its id and position
			//the id will be used to locate its Components when needed
			GameObject wallObj(GameObject::EntityType::WALL, position, (sf::Vector2f)wallAnimations_[0][0].getSize());

			//create VisualComponent, pass in the id needed to identify its GameObject, a pointer to the window, and a reference to its Textures
			//create PhysicsComponent, pass in its id and the type of collider it should have
			VisualComponent vComp(window_, wallAnimations_);
			PhysicsComponent pComp(wallObj.GetType());

			//store the GameObject and Components in this Level instance
			gameObjects_.insert({ currentObjectId, wallObj });
			visualComponents_.insert({ currentObjectId, vComp });
			physicsComponents_.insert({ currentObjectId, pComp });
		}

		//spawn character
		else if (levelPlan[i] == 'P') {
			//create GameObject
			GameObject characterObj(GameObject::EntityType::PLAYER, position, (sf::Vector2f)characterAnimations_[0][0].getSize());

			//create Components
			CharacterController cComp(window_);
			VisualComponent vComp(window_, characterAnimations_);
			PhysicsComponent pComp(characterObj.GetType(), 500.f);
			AudioComponent aComp(characterSoundEffects_);

			//store everything
			gameObjects_.insert({ currentObjectId, characterObj });
			characterControllers_.insert({ currentObjectId, cComp });
			visualComponents_.insert({ currentObjectId, vComp });
			physicsComponents_.insert({ currentObjectId, pComp });
			audioComponents_.insert({ currentObjectId, aComp });
		}

		//spawn enemies
		else if (levelPlan[i] == 'E') {
			//create GameObject
			GameObject enemyObj(GameObject::EntityType::ENEMY, position, (sf::Vector2f)enemyAnimations_[0][0].getSize());

			//create Components
			EnemyController cComp(currentObjectId);
			VisualComponent vComp(window_, enemyAnimations_);
			PhysicsComponent pComp(enemyObj.GetType(), 250.f);
			AudioComponent aComp(enemySoundEffects_);

			//store everything
			gameObjects_.insert({ currentObjectId, enemyObj });
			enemyControllers_.insert({ currentObjectId, cComp });
			visualComponents_.insert({ currentObjectId, vComp });
			physicsComponents_.insert({ currentObjectId, pComp });
			audioComponents_.insert({ currentObjectId, aComp });
		}

		//spawn horizontal doors
		else if (levelPlan[i] == 'H') {
			//create GameObject
			GameObject horDoorObj(GameObject::EntityType::DOOR, position, (sf::Vector2f)doorHorAnimations_[0][0].getSize());

			//create Components
			VisualComponent vComp(window_, doorHorAnimations_);
			PhysicsComponent pComp(horDoorObj.GetType());
			AudioComponent aComp(doorSoundEffects_);

			//store everything
			gameObjects_.insert({ currentObjectId, horDoorObj });
			visualComponents_.insert({ currentObjectId, vComp });
			physicsComponents_.insert({ currentObjectId, pComp });
			audioComponents_.insert({ currentObjectId, aComp });
		}

		//spawn vertical doors
		else if (levelPlan[i] == 'V') {
			//create GameObject
			GameObject verDoorObj(GameObject::EntityType::DOOR, position, (sf::Vector2f)doorVerAnimations_[0][0].getSize());

			//create Components
			VisualComponent vComp(window_, doorVerAnimations_);
			PhysicsComponent pComp(verDoorObj.GetType());
			AudioComponent aComp(doorSoundEffects_);

			//store everything
			gameObjects_.insert({ currentObjectId, verDoorObj });
			visualComponents_.insert({ currentObjectId, vComp });
			physicsComponents_.insert({ currentObjectId, pComp });
			audioComponents_.insert({ currentObjectId, aComp });
		}

		//spawn background
		else if (levelPlan[i] == 'B') {
			//create GameObject (pass in {0,0} as its position so it spawns in the top left regardless of where the 'B' is in the level plan
			GameObject backgroundObj(GameObject::EntityType::BACKGROUND, {0,0}, (sf::Vector2f)backgroundAnimations_[levelId_][0].getSize());

			//create VisualComponent (pass in the levelId as the starting animation to be played)
			VisualComponent vComp(window_, backgroundAnimations_, levelId_);

			//store everything
			gameObjects_.insert({ currentObjectId, backgroundObj });
			visualComponents_.insert({ currentObjectId, vComp });
		}

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
	GameObject bulletObj(GameObject::EntityType::BULLET, startPos, (sf::Vector2f)bulletAnimations_[0][0].getSize(), startDir);

	//create Components
	BulletController cComp;
	VisualComponent vComp(window_, bulletAnimations_);
	PhysicsComponent pComp(bulletObj.GetType(), 1000.f);
	AudioComponent aComp(bulletSoundEffects_);

	//store everything
	gameObjects_.insert({ currentObjectId, bulletObj });
	bulletControllers_.insert({ currentObjectId, cComp });
	visualComponents_.insert({ currentObjectId, vComp });
	physicsComponents_.insert({ currentObjectId, pComp });
	audioComponents_.insert({ currentObjectId, aComp });

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

	for (auto& [id, vComp] : visualComponents_)
	{
		vComp.Update(gameObjects_.at(id), deltaTime);
	}
}

#pragma endregion