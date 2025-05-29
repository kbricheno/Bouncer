#include "Level.h"

#include <iostream>

Level::Level(sf::RenderWindow* const window, const int levelId, const int tileSize, const int levelWidth, const int levelHeight, const std::vector<char> &levelPlan) {
	levelId_ = levelId;
	window_ = window;
	firstUpdate_ = true;

	GenerateTexturesAndSprites();

	//keep a record of the number of each element we're spawning
	int numWalls = 0, numEnemies = 0, numDoors = 0;

	//create all the level GameObjects and Components
	int currentX = 0, currentY = 0;
	for (int i = 0; i < levelPlan.size() - 1; i++)
	{		
		sf::Vector2f position(currentX * tileSize, currentY * tileSize);
		if (levelPlan[i] == 'W') 
		{
			//create GameObject, pass in its position, direction, and the index where its (visual) Component will be (current size of the Component vector)
			GameObject wallObj(position, -1, physicsComponents_.size(), visualComponents_.size(), -1); 
			
			//create a new Sprite for this wall
			wallSprites_.push_back(sf::Sprite(wallAnimations_.front().front()));

			//create visual Component, pass in the index where its GameObject will be (current size of the GameObject vector) pointer to window, references to its Textures & Sprite
			//gameobject index is used to assign a pointer to this Component's GameObject later -- can't assign the pointer during this loop because the GameObject is in local scope			
			VisualComponent vComp(gameObjects_.size(), window_, wallAnimations_, wallSprites_[numWalls]);
			PhysicsComponent pComp(gameObjects_.size(), PhysicsComponent::ColliderType::WALL, wallSprites_[0].getLocalBounds());

			//store the GameObject and Component in this level instance
			gameObjects_.push_back(wallObj);
			visualComponents_.push_back(vComp);
			physicsComponents_.push_back(pComp);

			numWalls++;

			/* still can't assign the GameObject and Component pointers
			because even though the object and Component are now stored 
			in the class' scope, vectors re-allocate memory every time a 
			new element is added, so its address changes */
		}
		// chars that spawn other GameObjects go here
		if (levelPlan[i] == 'P') {
			GameObject playerObj(position, characterControllers_.size(), physicsComponents_.size(), visualComponents_.size(), -1);
		
			//create a player Sprite with any old texture (Texture assignment will be handled by the VisualComponent)
			playerSprites_.push_back(sf::Sprite(playerAnimations_.front().front()));

			CharacterController cComp(gameObjects_.size(), window_);
			VisualComponent vComp(gameObjects_.size(), window_, playerAnimations_, playerSprites_[0]);
			PhysicsComponent pComp(gameObjects_.size(), PhysicsComponent::ColliderType::PLAYER, wallSprites_[0].getLocalBounds(), wallSprites_[0].getLocalBounds(), 500.f);

			gameObjects_.push_back(playerObj);
			characterControllers_.push_back(cComp);
			visualComponents_.push_back(vComp);
			physicsComponents_.push_back(pComp);
		}

		if (currentX >= levelWidth - 1) {
			currentY++;
			currentX = 0;
		}
		else currentX++;
	}
	/* should be able to assign the GameObject and Component pointers here
	(the vectors are finished changing size) but for some reason the memory 
	addresses *still* change between here and Update() */
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


	//door
	

	//walls
	//walls have only one animation containing one Texture, but must use the same template as other entities for the contextless VisualComponent to work
	std::vector<sf::Texture> wallIdleTextures_;
	wallIdleTextures_.push_back(sf::Texture("Assets/Environment/wall.png"));
	wallAnimations_.push_back(wallIdleTextures_);
}

void Level::GenerateLevel() {

}

void Level::HandleInput() {
	int gameObjectIndex = characterControllers_[0].GetGameObjectIndex();
	characterControllers_[0].Update(gameObjects_[gameObjectIndex]);
}


void Level::Update(float const deltaTime) {

	for (int p = 0; p < physicsComponents_.size(); p++)
	{
		int gameObjectIndex = physicsComponents_[p].GetGameObjectIndex();
		physicsComponents_[p].Update(gameObjects_[gameObjectIndex], deltaTime, physicsComponents_);
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
		visualComponents_[v].Update(gameObjects_[gameObjectIndex], deltaTime);
	}
}