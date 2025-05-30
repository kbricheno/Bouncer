#include "Level.h"

#include <iostream>

Level::Level(sf::RenderWindow* const window, const int levelId, const int tileSize, const int levelWidth, const int levelHeight, const std::vector<char> &levelPlan) {
	levelId_ = levelId;
	window_ = window;
	firstUpdate_ = true;

	GenerateTexturesAndSprites();
	GenerateLevel(tileSize, levelWidth, levelHeight, levelPlan);
}

//create all Textures & Sprites, store them in vector (here rather than on a GameObject or Component so we only create 1 set of each Texture/Sprite for the entire level)
void Level::GenerateTexturesAndSprites() {
	
	//player animations
	//create vectors representing individual animations -- each will hold a number of frames (images) making up the animation
	std::vector<sf::Texture> playerIdleTextures_;
	std::vector<sf::Texture> playerWalkTextures_;
	std::vector<sf::Texture> playerShootTextures_;
	std::vector<sf::Texture> playerReloadTextures_;

	//create a string we can use to generate each frame's file path (frames are saved as numbers in a folder)
	std::string fileName = "";

	//loop through the number of frames in the first animation, idle
	for (int i = 1; i <= 5; i++)
	{
		//create the frame's file path using the iterator to determine the current frame
		fileName = "Assets/Player/idle/" + std::to_string(i) + ".png";

		//create a Texture using the generated file path and immediately store it in the relevant animation vector
		playerIdleTextures_.push_back(sf::Texture(fileName));
	}

	//store the animation in the Level's vector of player animations
	playerAnimations_.push_back(playerIdleTextures_);

	//repeat the process for the walking animation
	for (int i = 1; i <= 5; i++)
	{
		fileName = "Assets/Player/walk/" + std::to_string(i) + ".png";
		playerWalkTextures_.push_back(sf::Texture(fileName));
	}
	playerAnimations_.push_back(playerWalkTextures_);

	//shooting
	for (int i = 1; i <= 3; i++)
	{
		fileName = "Assets/Player/shoot/" + std::to_string(i) + ".png";
		playerShootTextures_.push_back(sf::Texture(fileName));
	}
	playerAnimations_.push_back(playerShootTextures_);

	//reloading
	for (int i = 1; i <= 8; i++)
	{
		fileName = "Assets/Player/reload/" + std::to_string(i) + ".png";
		playerReloadTextures_.push_back(sf::Texture(fileName));
	}
	playerAnimations_.push_back(playerReloadTextures_);


	//enemy


	//bullet
	std::vector<sf::Texture> bulletIdleTextures_;
	bulletIdleTextures_.push_back(sf::Texture("Assets/Player/bullet.png"));
	bulletAnimations_.push_back(bulletIdleTextures_);

	//door
	

	//walls
	//walls have only one animation containing one Texture, but must use the same template as other entities for the contextless VisualComponent to work
	std::vector<sf::Texture> wallIdleTextures_;
	wallIdleTextures_.push_back(sf::Texture("Assets/Environment/wall.png"));
	wallAnimations_.push_back(wallIdleTextures_);
}

void Level::GenerateLevel(const int tileSize, const int levelWidth, const int levelHeight, const std::vector<char>& levelPlan) {
	//keep a record of the number of each element we're spawning
	int numWalls = 0, numEnemies = 0, numDoors = 0;

	//create all the level GameObjects and Components
	int currentX = 0, currentY = 0;
	for (int i = 0; i < levelPlan.size() - 1; i++)
	{
		sf::Vector2f position(currentX * tileSize, currentY * tileSize);

		//spawn wall
		if (levelPlan[i] == 'W')
		{
			//create GameObject, pass in its position and the index where its (physics & visual) Components will be (current size of the relevant Component vector)
			GameObject wallObj(position, -1, physicsComponents_.size(), visualComponents_.size(), -1);

			//create the wall's Sprite
			sf::Sprite wallSprite(wallAnimations_.front().front());
			wallSprite.setOrigin(wallSprite.getLocalBounds().size / 2.f);

			//create the wall's Collider
			sf::FloatRect wallCollider(wallSprite.getLocalBounds());

			//create visual Component, pass in the index where its GameObject will be (current size of the GameObject vector) pointer to window, references to its Textures & Sprite
			//create physics Component, pass in the index where its GameObject will be, the type of collider it has, and the bounds of the collider
			//gameobject index is used to assign a pointer to this Component's GameObject later -- can't assign the pointer during this loop because the GameObject is in local scope			
			VisualComponent vComp(gameObjects_.size(), window_, wallAnimations_, sprites_.size());
			PhysicsComponent pComp(gameObjects_.size(), PhysicsComponent::ColliderType::WALL, colliders_.size());

			//store the GameObject and Component in this level instance
			gameObjects_.push_back(wallObj);
			sprites_.push_back(wallSprite);
			colliders_.push_back(wallCollider);
			visualComponents_.push_back(vComp);
			physicsComponents_.push_back(pComp);

			numWalls++;

			/* still can't assign the GameObject and Component pointers
			because even though the object and Component are now stored
			in the class' scope, vectors re-allocate memory every time a
			new element is added, so its address changes */
		}

		//spawn player
		if (levelPlan[i] == 'P') {
			//create GameObject
			GameObject playerObj(position, characterControllers_.size(), physicsComponents_.size(), visualComponents_.size(), -1);

			//create Sprite
			sf::Sprite playerSprite(playerAnimations_.front().front());
			playerSprite.setOrigin(playerSprite.getLocalBounds().size / 2.f);

			//create Collider
			sf::FloatRect playerCollider(playerSprite.getLocalBounds());

			//create Components
			CharacterController cComp(gameObjects_.size(), window_);
			VisualComponent vComp(gameObjects_.size(), window_, playerAnimations_, sprites_.size());
			PhysicsComponent pComp(gameObjects_.size(), PhysicsComponent::ColliderType::PLAYER, colliders_.size(), -1, 500.f);

			//store everything
			gameObjects_.push_back(playerObj);
			sprites_.push_back(playerSprite);
			colliders_.push_back(playerCollider);
			characterControllers_.push_back(cComp);
			visualComponents_.push_back(vComp);
			physicsComponents_.push_back(pComp);
		}

		//spawn enemy


		//spawn door


		//spawn background


		if (currentX >= levelWidth - 1) {
			currentY++;
			currentX = 0;
		}
		else currentX++;
	}
}

void Level::SpawnBullet(sf::Vector2f const startPos, sf::Vector2f const startDir) {
	std::cout << "we need more boollet\n";

	//create GameObject
	GameObject bulletObj(startPos, bulletControllers_.size(), physicsComponents_.size(), visualComponents_.size(), -1);

	//create Sprite
	sf::Sprite bulletSprite(bulletAnimations_.front().front());
	bulletSprite.setOrigin(bulletSprite.getLocalBounds().size / 2.f);

	//create Collider
	sf::FloatRect bulletCollider(bulletSprite.getLocalBounds());

	//create Components
	BulletController cComp(gameObjects_.size(), startDir);
	VisualComponent vComp(gameObjects_.size(), window_, bulletAnimations_, sprites_.size());
	PhysicsComponent pComp(gameObjects_.size(), PhysicsComponent::ColliderType::BULLET, colliders_.size(), -1, 1000.f);

	//store everything
	gameObjects_.push_back(bulletObj);
	sprites_.push_back(bulletSprite);
	colliders_.push_back(bulletCollider);
	bulletControllers_.push_back(cComp);
	visualComponents_.push_back(vComp);
	physicsComponents_.push_back(pComp);
}

#pragma region Gameplay Loop

void Level::HandleInput(float const deltaTime) {
	//update character
	if (characterControllers_[0].HandleInput(gameObjects_[characterControllers_[0].GetGameObjectIndex()], deltaTime)) //this is really bad, need to think of a better way to spawn bullets
	{
		sf::Angle startAngle = gameObjects_[characterControllers_[0].GetGameObjectIndex()].GetRotation();
		sf::Vector2f startDir = { cos(startAngle.asRadians()), sin(startAngle.asRadians()) };

		SpawnBullet(gameObjects_[characterControllers_[0].GetGameObjectIndex()].GetPosition(), startDir);
	}

	//update enemies

	//update bullets
	for (int b = 0; b < bulletControllers_.size(); b++)
	{
		int gameObjectIndex = bulletControllers_[b].GetGameObjectIndex();
		bulletControllers_[b].HandleInput(gameObjects_[gameObjectIndex], deltaTime);
	}
}


void Level::Update(float const deltaTime) {

	for (int p = 0; p < physicsComponents_.size(); p++)
	{
		int gameObjectIndex = physicsComponents_[p].GetGameObjectIndex();
		physicsComponents_[p].Update(gameObjects_[gameObjectIndex], deltaTime, colliders_);
	}

	//for (int a = 0; a < audioComponents_.size(); a++)
	//{
	//	audioComponents_[a].Update();
	//}
}

void Level::Draw(float const deltaTime) {
	for (int v = 0; v < visualComponents_.size(); v++)
	{
		int gameObjectIndex = visualComponents_[v].GetGameObjectIndex();
		int spriteIndex = visualComponents_[v].GetSpriteIndex();
		visualComponents_[v].Update(gameObjects_[gameObjectIndex], deltaTime, sprites_[spriteIndex]);
	}
}

#pragma endregion