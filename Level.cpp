#include "Level.h"

#include <iostream>

Level::Level(sf::RenderWindow* const window, const int levelId, const int tileSize, const int levelWidth, const int levelHeight, const std::vector<char> &levelPlan) {
	levelId_ = levelId;
	window_ = window;
	firstUpdate_ = true;

	//create wall Textures & Sprites, store them in vector (here rather than on a GameObject or Component so we only create 1 set of each Texture/Sprite for the entire level)
	wallTextures_.push_back(sf::Texture("Assets/Environment/wall.png"));
	wallSprites_.push_back(sf::Sprite(wallTextures_.back())); //Texture assignment here won't work but can't create a Sprite without passing a Texture to the constructor

	//create other Textures & Sprites here


	//create all the level GameObjects and Components
	int currentX = 0, currentY = 0;
	for (int i = 0; i < levelPlan.size() - 1; i++)
	{		
		sf::Vector2f position(currentX * tileSize, currentY * tileSize);
		if (levelPlan[i] == 'W') 
		{
			//create GameObject, pass in its position, direction, and the index where its (visual) Component will be (current size of the Component vector)
			GameObject wallObj(position, -1, physicsComponents_.size(), visualComponents_.size(), -1); 

			//create visual Component, pass in the index where its GameObject will be (current size of the GameObject vector) pointer to window, references to its Textures & Sprites
			//gameobject index is used to assign a pointer to this Component's GameObject later -- can't assign the pointer during this loop because the GameObject is in local scope
			VisualComponent vComp(gameObjects_.size(), window_, wallTextures_, wallSprites_);
			PhysicsComponent pComp(gameObjects_.size(), PhysicsComponent::ColliderType::WALL, wallSprites_[0].getLocalBounds());

			//store the GameObject and Component in this level instance
			gameObjects_.push_back(wallObj);
			visualComponents_.push_back(vComp);
			physicsComponents_.push_back(pComp);

			/* still can't assign the GameObject and Component pointers
			because even though the object and Component are now stored 
			in the class' scope, vectors re-allocate memory every time a 
			new element is added, so its address changes */
		}
		// chars that spawn other GameObjects go here
		if (levelPlan[i] == 'P') {
			GameObject playerObj(position, characterControllers_.size(), physicsComponents_.size(), visualComponents_.size(), -1);
		
			CharacterController cComp(gameObjects_.size(), window_);
			VisualComponent vComp(gameObjects_.size(), window_, wallTextures_, wallSprites_);
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

void Level::HandleInput() {
	characterControllers_[0].Update();
}


void Level::Update(float deltaTime) {

	//use a bool to assign the GameObject and Component pointers on the first Update() call
	if (firstUpdate_) 
	{
		//go through all GameObjects and Components and finally assign pointers based on the index we passed in during their construction
		for (int o = 0; o < gameObjects_.size(); o++)
		{
			int controllerIndex = gameObjects_[o].GetControllerIndex();
			int physicsIndex = gameObjects_[o].GetPhysicsIndex();
			int visualIndex = gameObjects_[o].GetVisualIndex();
			int audioIndex = gameObjects_[o].GetAudioIndex();

			ControllerComponent* cComp = controllerIndex == -1 ? nullptr : &characterControllers_[0]; //temp
			PhysicsComponent* pComp = physicsIndex == -1 ? nullptr : &physicsComponents_[physicsIndex];
			VisualComponent* vComp = visualIndex == -1 ? nullptr : &visualComponents_[visualIndex];
			AudioComponent* aComp = audioIndex == -1 ? nullptr : &audioComponents_[audioIndex];

			gameObjects_[o].SetComponents(cComp, pComp, vComp, aComp);
			//std::cout << "actual obj: " << &gameObjects_[o] << "\n";
		}

		for (int c = 0; c < characterControllers_.size(); c++)
		{
			characterControllers_[c].SetGameObject(&gameObjects_[characterControllers_[c].GetGameObjectIndex()]);
		}

		for (int p = 0; p < physicsComponents_.size(); p++)
		{
			physicsComponents_[p].SetGameObject(&gameObjects_[physicsComponents_[p].GetGameObjectIndex()]);
		}

		for (int v = 0; v < visualComponents_.size(); v++)
		{
			visualComponents_[v].SetGameObject(&gameObjects_[visualComponents_[v].GetGameObjectIndex()]);
			//std::cout << "vector obj add: " << &gameObjects_[visualComponents_[v].GetGameObjectIndex()] << "\n";
			//std::cout << "vc obj add: " << &*visualComponents_[v].GetGameObject() << "\n";
		}

		for (int a = 0; a < audioComponents_.size(); a++)
		{
			audioComponents_[a].SetGameObject(&gameObjects_[audioComponents_[a].GetGameObjectIndex()]);
		}
		firstUpdate_ = false;
	}

	for (int o = 0; o < gameObjects_.size(); o++)
	{
		//std::cout << &gameObjects_[o] << " obj " << o << ": " << gameObjects_[o].GetPosition().x << ", " << gameObjects_[o].GetPosition().y << "\n";
	}

	for (int p = 0; p < physicsComponents_.size(); p++)
	{
		physicsComponents_[p].Update(deltaTime, physicsComponents_);
	}

	//for (int a = 0; a < audioComponents_.size(); a++)
	//{
	//	audioComponents_[a].Update();
	//}
}

void Level::Draw() {
	for (int v = 0; v < visualComponents_.size(); v++)
	{
		//std::cout << &*visualComponents_[v].GetGameObject() << " vc obj " << v << ": " << visualComponents_[v].GetGameObject()->GetPosition().x << ", " << visualComponents_[v].GetGameObject()->GetPosition().y << "\n";
		visualComponents_[v].Update();
	}
}