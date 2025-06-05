#include "GameManager.h"
#include <iostream>

#pragma region Game Set-Up

//Define the static bool that allows only one instance of GameManager to exist
bool GameManager::instantiated_ = false;

//Construct the GameManager instance, ensuring only one instance can be created
GameManager::GameManager() {
    assert(!instantiated_);
    instantiated_ = true;
}

//Obtain a pointer to the RenderWindow created in the main loop
void GameManager::SetupWindow(sf::RenderWindow* const window) {
    window_ = window;
	window_->setKeyRepeatEnabled(false);
}

//Prepare everything needed to create Level instances (import data from external files)
void GameManager::PrepareLevelGeneration(std::ifstream& const levelsFile) {
    
	//generate the level plans
	if (!GenerateLevelPlan(levelsFile)) std::cout << "ERROR: Could not find the level file path, or file is blank!\n";

	//generate all textures
	if (!GenerateTextures()) std::cout << "ERROR: Could not find all texture file paths!\n";

	//generate all sounds
	if (!GenerateSoundEffects()) std::cout << "ERROR: Could not find all sound effect file paths!\n";
}

#pragma endregion

#pragma region File Loading

//Create every plan needed for Level instances to generate entities (one plan per level)
bool GameManager::GenerateLevelPlan(std::ifstream& const levelsFile) {
	
	//each level will spawn entities based on a level plan (a vector of chars)
	std::vector<char> levelPlan;
	char levelChar;

	//read the external file
	if (levelsFile.is_open()) {
		while (levelsFile.good()) {
			//obtain each char and add it to the current level plan vector
			levelChar = levelsFile.get();
			std::cout << levelChar << "\n";
			levelPlan.push_back(levelChar);

			//TODO: add an identifier to separate level plans (e.g. a specific char indicates end of level plan)
		}
	}
	allLevelPlans_.push_back(levelPlan);

	if (allLevelPlans_.size() == 0) return false; //couldn't find the level file or it was empty
	else return true; //level plans successfully generated
}

//Create every Texture needed for the entire game, store them in a giant ugly map of maps of vectors
bool GameManager::GenerateTextures() {

	//create maps to hold each entity's name and all of its animations
	std::map<std::string, std::vector<sf::Texture>> characterAnimations_;
	std::map<std::string, std::vector<sf::Texture>> enemyAnimations_;
	std::map<std::string, std::vector<sf::Texture>> bulletAnimations_;
	std::map<std::string, std::vector<sf::Texture>> wallAnimations_;
	std::map<std::string, std::vector<sf::Texture>> doorHorAnimations_;
	std::map<std::string, std::vector<sf::Texture>> doorVerAnimations_;
	std::map<std::string, std::vector<sf::Texture>> backgroundAnimations_;


	//player animations .....................................................................................................................................................
	//create vectors to hold individual animations -- each will hold a number of frames (images) making up the animation
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

		//create a Texture using the generated file path and add it to the relevant animation vector
		sf::Texture characterIdleFrame;
		if (!characterIdleFrame.loadFromFile(fileName)) return false;
		characterIdleTextures_.push_back(characterIdleFrame);
	}

	//add the animation's name and frames to the character animation map
	characterAnimations_.insert({ "idle", characterIdleTextures_ });

	//repeat the process for the walking animation
	for (int i = 1; i <= 5; i++)
	{
		fileName = "Assets/Player/walk/" + std::to_string(i) + ".png";
		
		sf::Texture characterWalkFrame;
		if (!characterWalkFrame.loadFromFile(fileName)) return false;
		characterWalkTextures_.push_back(characterWalkFrame);
	}
	characterAnimations_.insert({ "walk", characterWalkTextures_ });

	//shooting
	for (int i = 1; i <= 3; i++)
	{
		fileName = "Assets/Player/shoot/" + std::to_string(i) + ".png";

		sf::Texture characterShootFrame;
		if (!characterShootFrame.loadFromFile(fileName)) return false;
		characterShootTextures_.push_back(characterShootFrame);
	}
	characterAnimations_.insert({ "shoot", characterShootTextures_ });

	//reloading
	for (int i = 1; i <= 4; i++)
	{
		fileName = "Assets/Player/reload/" + std::to_string(i) + ".png";

		sf::Texture characterReloadFrame;
		if (!characterReloadFrame.loadFromFile(fileName)) return false;
		characterReloadTextures_.push_back(characterReloadFrame);
	}
	characterAnimations_.insert({ "reload", characterReloadTextures_ });

	//finally, store the character's name and all character animations to the "allAnimations" map stored in the GameManager object
	allAnimations_.insert({ "character", characterAnimations_ });


	//enemy animations .....................................................................................................................................................
	std::vector<sf::Texture> enemyIdleTextures_;
	std::vector<sf::Texture> enemyWalkTextures_;
	std::vector<sf::Texture> enemyDyingTextures_;

	//idling
	for (int i = 1; i <= 1; i++)
	{
		fileName = "Assets/Guard/idle/" + std::to_string(i) + ".png";

		sf::Texture enemyIdleFrame;
		if (!enemyIdleFrame.loadFromFile(fileName)) return false;
		enemyIdleTextures_.push_back(enemyIdleFrame);
	}
	enemyAnimations_.insert({ "idle", enemyIdleTextures_ });

	//walking
	for (int i = 1; i <= 10; i++)
	{
		fileName = "Assets/Guard/walk/" + std::to_string(i) + ".png";

		sf::Texture enemyWalkFrame;
		if (!enemyWalkFrame.loadFromFile(fileName)) return false;
		enemyWalkTextures_.push_back(enemyWalkFrame);
	}
	enemyAnimations_.insert({ "walk", enemyWalkTextures_ });

	//dying
	for (int i = 1; i <= 4; i++)
	{
		fileName = "Assets/Guard/dying/" + std::to_string(i) + ".png";

		sf::Texture enemyDyingFrame;
		if (!enemyDyingFrame.loadFromFile(fileName)) return false;
		enemyDyingTextures_.push_back(enemyDyingFrame);
	}
	enemyAnimations_.insert({ "die", enemyDyingTextures_ });

	allAnimations_.insert({ "enemy", enemyAnimations_ });


	//bullet Texture .....................................................................................................................................................
	//bullets only have one animation containing one Texture, but must use the same template as other entities for the contextless VisualComponent to work
	std::vector<sf::Texture> bulletIdleTextures_;

	sf::Texture bulletIdleFrame;
	if (!bulletIdleFrame.loadFromFile("Assets/Player/bullet.png")) return false;
	bulletIdleTextures_.push_back(bulletIdleFrame);	

	bulletAnimations_.insert({ "idle", bulletIdleTextures_ });

	allAnimations_.insert({ "bullet", bulletAnimations_ });


	//door animations .....................................................................................................................................................
	std::vector<sf::Texture> horizontalDoorClosedTextures_;
	std::vector<sf::Texture> horizontalDoorOpenTextures_;
	std::vector<sf::Texture> verticalDoorClosedTextures_;
	std::vector<sf::Texture> verticalDoorOpenTextures_;

	sf::Texture horDoorIdleFrame;
	if (!horDoorIdleFrame.loadFromFile("Assets/Environment/doorHor.png")) return false;
	horizontalDoorClosedTextures_.push_back(horDoorIdleFrame);
	doorHorAnimations_.insert({ "idle", horizontalDoorClosedTextures_ });

	sf::Texture horDoorBrokenFrame;
	if (!horDoorBrokenFrame.loadFromFile("Assets/Environment/doorHorBroken.png")) return false;
	horizontalDoorOpenTextures_.push_back(horDoorBrokenFrame);
	doorHorAnimations_.insert({ "break", horizontalDoorOpenTextures_ });

	sf::Texture verDoorIdleFrame;
	if (!verDoorIdleFrame.loadFromFile("Assets/Environment/doorVer.png")) return false;
	verticalDoorClosedTextures_.push_back(verDoorIdleFrame);
	doorVerAnimations_.insert({ "idle", verticalDoorClosedTextures_ });

	sf::Texture verDoorBrokenFrame;
	if (!verDoorBrokenFrame.loadFromFile("Assets/Environment/doorVerBroken.png")) return false;
	verticalDoorOpenTextures_.push_back(verDoorBrokenFrame);
	doorVerAnimations_.insert({ "break", verticalDoorOpenTextures_ });

	allAnimations_.insert({ "doorHor", doorHorAnimations_ });
	allAnimations_.insert({ "doorVer", doorVerAnimations_ });


	//wall Texture .....................................................................................................................................................
	std::vector<sf::Texture> wallIdleTextures_;

	sf::Texture wallIdleFrame;
	if (!wallIdleFrame.loadFromFile("Assets/Environment/wall.png")) return false;
	wallIdleTextures_.push_back(wallIdleFrame);

	wallAnimations_.insert({ "idle", wallIdleTextures_ });

	allAnimations_.insert({ "wall", wallAnimations_ });


	//background Textures .....................................................................................................................................................
	std::vector<sf::Texture> background1Texture_;
	std::vector<sf::Texture> background2Texture_;
	std::vector<sf::Texture> background3Texture_;

	sf::Texture backGround1Frame;
	if (!backGround1Frame.loadFromFile("Assets/Environment/map1.png")) return false;
	background1Texture_.push_back(backGround1Frame);
	backgroundAnimations_.insert({ "0", background1Texture_ });
	
	sf::Texture backGround2Frame;
	if (!backGround2Frame.loadFromFile("Assets/Environment/map2.png")) return false;
	background2Texture_.push_back(backGround2Frame);
	backgroundAnimations_.insert({ "1", background2Texture_ });
	
	sf::Texture backGround3Frame;
	if (!backGround3Frame.loadFromFile("Assets/Environment/map3.png")) return false;
	background3Texture_.push_back(backGround3Frame);
	backgroundAnimations_.insert({ "2", background3Texture_ });

	allAnimations_.insert({ "background", backgroundAnimations_ });


	//all textures successfully loaded
	return true;
}

//Create and store every SoundBuffer needed for the entire game
bool GameManager::GenerateSoundEffects() {

	//create maps to hold each entity's name and all of its sound buffers
	std::map<std::string, sf::SoundBuffer> characterSoundEffects_;
	std::map<std::string, sf::SoundBuffer> enemySoundEffects_;
	std::map<std::string, sf::SoundBuffer> bulletSoundEffects_;
	std::map<std::string, sf::SoundBuffer> doorSoundEffects_;
	std::map<std::string, sf::SoundBuffer> globalSoundEffects_;


	//create & load character sound buffers
	sf::SoundBuffer characterShoot;
	sf::SoundBuffer characterReload;
	if (!characterShoot.loadFromFile("Assets/Sound/playerShoot.wav")) return false;
	if (!characterReload.loadFromFile("Assets/Sound/playerReload.wav")) return false;

	//store each character sound buffer in the character's sounds map
	characterSoundEffects_.insert({ "shoot", characterShoot });
	characterSoundEffects_.insert({ "reload", characterReload });

	//finally, store the character's name and all character sound buffers to the "allSoundEffects" map stored in the GameManager object
	allSoundEffects_.insert({ "character", characterSoundEffects_ });


	//enemy sound effects
	sf::SoundBuffer enemyDeath;
	if (!enemyDeath.loadFromFile("Assets/Sound/guardDeath.wav")) return false;

	enemySoundEffects_.insert({ "die", enemyDeath });

	allSoundEffects_.insert({ "enemy", enemySoundEffects_ });


	//bullet sound effects
	sf::SoundBuffer bulletBounce;
	if (!bulletBounce.loadFromFile("Assets/Sound/bulletBounce.wav")) return false;

	bulletSoundEffects_.insert({ "bounce", bulletBounce });

	allSoundEffects_.insert({ "bullet", bulletSoundEffects_ });


	//door sound effects
	sf::SoundBuffer doorBreak;
	if (!doorBreak.loadFromFile("Assets/Sound/doorBreak.wav")) return false;

	doorSoundEffects_.insert({ "break", doorBreak });

	allSoundEffects_.insert({ "door", doorSoundEffects_ });


	//global sound effects
	sf::SoundBuffer victory;
	sf::SoundBuffer detected;
	if (!victory.loadFromFile("Assets/Sound/success.wav")) return false;
	if (!detected.loadFromFile("Assets/Sound/houseAlarm.wav")) return false;

	globalSoundEffects_.insert({ "victory", victory });
	globalSoundEffects_.insert({ "detected", detected });

	allSoundEffects_.insert({ "global", globalSoundEffects_ });


	//all sound buffers successfully loaded
	return true;
}

#pragma endregion

#pragma region Input Management

//Handle all events from the queue -- closing window, singular key presses, mouse clicks
void GameManager::HandleEventQueue() {

	//handle input from the event queue
	while (const std::optional event = window_->pollEvent())
	{
		//closing window
		if (event->is<sf::Event::Closed>())
		{
			window_->close();
		}

		//key press events
		else if (const auto* keyPressed = event->getIf<sf::Event::KeyPressed>())
		{
			//menu key press events
			//pausing game (quits game atm)
			if (keyPressed->scancode == sf::Keyboard::Scancode::Escape)
			{
				window_->close();
			}


			//in-game key press events (commands sent to character)
			//only parse these commands if a Level instance currently exists
			if (currentLevel_.size() > 0)
			{
				//reload weapon
				if (keyPressed->scancode == sf::Keyboard::Scancode::R)
				{
					currentLevel_[0].CommandReload();
				}
			}
		}

		//mouse click events
		else if (const auto* mouseButtonPressed = event->getIf<sf::Event::MouseButtonPressed>())
		{
			if (mouseButtonPressed->button == sf::Mouse::Button::Left)
			{
				//menu mouse clicks
				//left clicking on menu buttons

				
				//in-game mouse clicks (commands sent to character)
				//only parse these commands if a Level instance currently exists
				if (currentLevel_.size() > 0)
				{
					//shoot weapon
					currentLevel_[0].CommandShoot();
				}
			}
		}
	}
}

//Handle all movement input, which consists of the WASD keys held down or released
void GameManager::HandleMovementInput() {

	//if no Level instance currently exists, ignore movement input
	if (currentLevel_.size() <= 0) return;

	/*
	I avoided using Event::KeyPressed for movement input because key-held-down events are generated with a significant delay (the
	same delay as holding a key down in a word processor). I actually switched Event key repeat off completely so it can be used 
	for singular key presses exclusively -- I don't want users to be able to, e.g., hold down Esc to pause/unpause constantly.
	*/

	//upwards movement
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W))
	{
		currentLevel_[0].CommandMoveUp(true);
	}
	else
	{
		currentLevel_[0].CommandMoveUp(false);
	}
	
	//leftwards movement
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A))
	{
		currentLevel_[0].CommandMoveLeft(true);
	}
	else
	{
		currentLevel_[0].CommandMoveLeft(false);
	}

	//downward movement
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S))
	{
		currentLevel_[0].CommandMoveDown(true);
	}
	else
	{
		currentLevel_[0].CommandMoveDown(false);
	}

	//rightward movement
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D))
	{
		currentLevel_[0].CommandMoveRight(true);
	}
	else
	{
		currentLevel_[0].CommandMoveRight(false);
	}
}

#pragma endregion

#pragma region Game Loop

//Handle all user input and send off commands to the Level instance
void GameManager::HandleInput(float const deltaTime) {
    
	HandleEventQueue();
	HandleMovementInput();
}

//Create a new Level instance if the previous level has been cleared, otherwise tell the Level instance to update its entities
void GameManager::Update(float const deltaTime) {    
   
	//if the level has been cleared, generate a new level
	if (levelCleared_) 
	{
        //if a level already exists, the next level's id will be the current one + 1, otherwise we're about to create level 0
        int nextLevelId = currentLevel_.size() > 0 ? currentLevel_[0].GetLevelId() + 1 : 0;
        
		//create a Level instance and pass it everything it needs to generate and maintain every entity within the level
        Level nextLevel(window_, nextLevelId, tileSize_, 10, 10, allLevelPlans_[nextLevelId], allAnimations_, allSoundEffects_);
        
		//store the Level instance
		currentLevel_.push_back(nextLevel);
        
        levelCleared_ = false;
    }

	//if the level has not been cleared, pass along the instruction to update
    else 
	{
        currentLevel_[0].Update(deltaTime);
    }
}

//If a Level instance exists, tell it to draw its entities
void GameManager::Draw(float const deltaTime) {

	//if a level currently exists, pass along the instruction to draw
    if (currentLevel_.size() > 0) 
	{
        currentLevel_[0].Draw(deltaTime);
    }
}

#pragma endregion