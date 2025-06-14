#include "GameManager.h"
#include <iostream>

#pragma region Game Set-Up

//Define the static bool that allows only one instance of GameManager to exist
bool GameManager::instantiated_ = false;

//Prepare everything needed to create Level instances (import data from external files)
void GameManager::PrepareLevelGeneration(std::ifstream& const levelsFile) {
    
	//generate the level plans
	if (!GenerateLevelPlan(levelsFile)) std::cout << "ERROR: Could not find the level file path, or file is blank!\n";

	//generate all textures
	if (!GenerateTextures()) std::cout << "ERROR: Could not find all texture file paths!\n";

	//generate all sounds
	if (!GenerateSoundEffects()) std::cout << "ERROR: Could not find all sound effect file paths!\n";

	//prepare the view used for drawing menus and the hud
	hudView_.setSize((sf::Vector2f)sf::VideoMode::getDesktopMode().size);
	hudView_.setCenter((sf::Vector2f)sf::VideoMode::getDesktopMode().size / 2.f);

	//set up the title screen
	SetupTitle();
}

//Delete any existing level, then create and store a new one
void GameManager::CreateLevel(int const levelId) {
	//create a Level instance and pass it everything it needs to generate and maintain every entity within the level
	Level nextLevel(window_, levelId, tileSize_, allLevelPlans_[levelId], allLevelSizes_[levelId], allAnimations_, allSoundEffects_);

	//clear the Level storage vector, then store the Level instance
	currentLevel_.clear();
	currentLevel_.push_back(nextLevel);

	//update the game state
	state_ = GameState::IN_LEVEL;
}

#pragma endregion

#pragma region File Loading

//Create every plan needed for Level instances to generate entities (one plan per level)
bool GameManager::GenerateLevelPlan(std::ifstream& const levelsFile) {
	
	//each level will spawn entities based on a level plan (a vector of chars)
	std::vector<char> levelPlan;
	char levelChar;

	//some characters in the file between plans are irrelevant (names, colons, spaces, etc.), so use a bool to determine whether to write to the level plan
	bool writing = false;

	sf::Vector2i levelSize = { 0,1 };

	//read the external file
	if (levelsFile.is_open()) {
		while (levelsFile.good()) {
			//obtain each char and add it to the current level plan vector
			levelChar = levelsFile.get();
			if (writing)
			{
				if (levelChar == '}') //end of level indicator
				{
					writing = false;
					allLevelPlans_.push_back(levelPlan); //push the finished level plan into the vector
					allLevelSizes_.push_back(levelSize); //push the calculated level size into the vector
				}
				else
				{
					if (levelChar != ' ') //ignore all whitespace
					{
						if (levelChar == '\n')
						{
							levelSize.y++; //use whitespaces to measure the level height
						}
						else 
						{
							if (levelSize.y == 1) levelSize.x++; //only measure the width before the first whitespace
							levelPlan.push_back(levelChar);
						}
					}
				}
			}
			else 
			{
				if (levelChar == '{') 
				{
					writing = true; //start of level indicator

					levelPlan.clear(); //clear the temporary level plan vector ready to write for this level
					levelSize = { 0,1 }; //reset the level size ready to measure for this level
				}
			}
		}
	}
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


	//menu Textures .....................................................................................................................................................
	sf::Texture titleBackground;
	if (!titleBackground.loadFromFile("Assets/UI/titleScreen.png")) return false;
	menuImages_.insert({ "title", titleBackground });

	sf::Texture howToPlayBackground;
	if (!howToPlayBackground.loadFromFile("Assets/UI/instructions.png")) return false;
	menuImages_.insert({ "howToPlay", howToPlayBackground });

	sf::Texture volumeBackground;
	if (!volumeBackground.loadFromFile("Assets/UI/volume.png")) return false;
	menuImages_.insert({ "volume", volumeBackground });


	//button Textures .....................................................................................................................................................
	std::map<std::string, sf::Texture> startImages;
	std::map<std::string, sf::Texture> continueImages;
	std::map<std::string, sf::Texture> quitImages;
	std::map<std::string, sf::Texture> howToPlayImages;
	std::map<std::string, sf::Texture> mainMenuImages;
	std::map<std::string, sf::Texture> volumeKnobImages;

	sf::Texture startTexture;
	sf::Texture startHoveredTexture;
	if (!startTexture.loadFromFile("Assets/UI/start.png")) return false;
	if (!startHoveredTexture.loadFromFile("Assets/UI/startHover.png")) return false;
	startImages.insert({ "unhovered", startTexture });
	startImages.insert({ "hovered", startHoveredTexture });

	buttonImages_.insert({ "start", startImages });

	sf::Texture continueTexture;
	sf::Texture continueHoveredTexture;
	if (!continueTexture.loadFromFile("Assets/UI/continue.png")) return false;
	if (!continueHoveredTexture.loadFromFile("Assets/UI/continueHover.png")) return false;
	continueImages.insert({ "unhovered", continueTexture });
	continueImages.insert({ "hovered", continueHoveredTexture });

	buttonImages_.insert({ "continue", continueImages });

	sf::Texture quitTexture;
	sf::Texture quitHoveredTexture;
	if (!quitTexture.loadFromFile("Assets/UI/quit.png")) return false;
	if (!quitHoveredTexture.loadFromFile("Assets/UI/quitHover.png")) return false;
	quitImages.insert({ "unhovered", quitTexture });
	quitImages.insert({ "hovered", quitHoveredTexture });

	buttonImages_.insert({ "quit", quitImages });

	sf::Texture howToPlayTexture;
	sf::Texture howToPlayHoveredTexture;
	if (!howToPlayBackground.loadFromFile("Assets/UI/howToPlay.png")) return false;
	if (!howToPlayHoveredTexture.loadFromFile("Assets/UI/howToPlayHover.png")) return false;
	howToPlayImages.insert({ "unhovered", howToPlayBackground });
	howToPlayImages.insert({ "hovered", howToPlayHoveredTexture });

	buttonImages_.insert({ "howToPlay", howToPlayImages });

	sf::Texture mainMenuTexture;
	sf::Texture mainMenuHoveredTexture;
	if (!mainMenuTexture.loadFromFile("Assets/UI/mainMenu.png")) return false;
	if (!mainMenuHoveredTexture.loadFromFile("Assets/UI/mainMenuHover.png")) return false;
	mainMenuImages.insert({ "unhovered", mainMenuTexture });
	mainMenuImages.insert({ "hovered", mainMenuHoveredTexture });

	buttonImages_.insert({ "mainMenu", mainMenuImages });

	sf::Texture volumeKnobTexture;
	if (!volumeKnobTexture.loadFromFile("Assets/UI/barKnob.png")) return false;
	volumeKnobImages.insert({ "unhovered", volumeKnobTexture });

	buttonImages_.insert({ "volumeKnob", volumeKnobImages });


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

#pragma region Menus & HUD

//Clear any existing background image, buttons, and text objects currently stored
void GameManager::ClearMenu() {
	activeImages_.clear();
	activeButtons_.clear();
	activeText_.clear();
}

//Set up each unique menu's background image (or lack thereof), buttons, and text objects
void GameManager::SetupTitle() {
	//update the game state
	state_ = GameState::TITLE;

	//wipe any previous menu
	ClearMenu();

	//create the new menu
	//create a sprite from the title background image
	sf::Sprite titleBackground(menuImages_.at("title"));
	titleBackground.setOrigin(titleBackground.getLocalBounds().size / 2.f);
	titleBackground.setPosition(hudView_.getCenter());

	//store the background sprite
	activeImages_.push_back(titleBackground);
}

void GameManager::SetupMainMenu() {
	//update the game state and wipe any previous menu
	state_ = GameState::MAIN_MENU;
	ClearMenu();

	//add the buttons needed for this menu
	sf::Vector2f buttonSize = (sf::Vector2f)buttonImages_.at("start").at("hovered").getSize();

	Button startButton(buttonImages_.at("start"), sf::Vector2f(hudView_.getCenter().x - buttonSize.x / 2.f, hudView_.getCenter().y - buttonSize.y * 2.f));
	Button howToPlayButton(buttonImages_.at("howToPlay"), sf::Vector2f(hudView_.getCenter().x - buttonSize.x / 2.f, hudView_.getCenter().y - buttonSize.y));
	Button quitButton(buttonImages_.at("quit"), sf::Vector2f(hudView_.getCenter().x - buttonSize.x / 2.f, hudView_.getCenter().y + buttonSize.y));
	activeButtons_.insert({ "start", startButton });
	activeButtons_.insert({ "howToPlay", howToPlayButton });
	activeButtons_.insert({ "quit", quitButton });

	//volume slider
	//create the background for the volume slider
	sf::Sprite volumeBackground(menuImages_.at("volume"));
	volumeBackground.setOrigin(volumeBackground.getLocalBounds().size / 2.f);
	volumeBackground.setPosition(sf::Vector2f(hudView_.getCenter().x - buttonSize.x / 2.f, hudView_.getCenter().y));
	activeImages_.push_back(volumeBackground);

	//create and add the button for the volume slider
	sf::Vector2f knobSize = (sf::Vector2f)buttonImages_.at("volumeKnob").at("unhovered").getSize();
	Button volumeKnob(buttonImages_.at("volumeKnob"), volumeBackground.getGlobalBounds().position + knobSize, volumeBackground.getGlobalBounds().position.x, volumeBackground.getGlobalBounds().size.x);
	activeButtons_.insert({ "volumeKnob", volumeKnob });
}

void GameManager::SetupHowToPlay() {
	//update the game state and wipe any previous menu
	state_ = GameState::HOW_TO_PLAY;
	ClearMenu();

	//add the background image and buttons needed for this menu
	sf::Sprite howToPlayBackground(menuImages_.at("howToPlay"));
	howToPlayBackground.setOrigin(howToPlayBackground.getLocalBounds().size / 2.f);
	howToPlayBackground.setPosition(hudView_.getCenter());	
	activeImages_.push_back(howToPlayBackground);

	//add the button needed for this menu
	sf::Vector2f buttonSize = (sf::Vector2f)buttonImages_.at("start").at("hovered").getSize();

	Button startButton(buttonImages_.at("start"), sf::Vector2f(hudView_.getCenter().x - buttonSize.x / 2.f, hudView_.getSize().y - buttonSize.y * 1.5f));
	activeButtons_.insert({ "start", startButton });
}

void GameManager::SetupPause() {
	//update the game state and wipe any previous menu
	state_ = GameState::PAUSED;
	ClearMenu();

	//add the buttons needed for this menu
	sf::Vector2f buttonSize = (sf::Vector2f)buttonImages_.at("start").at("hovered").getSize();

	Button continueButton(buttonImages_.at("continue"), sf::Vector2f(hudView_.getCenter().x - buttonSize.x / 2.f, hudView_.getCenter().y - buttonSize.y * 2.f));
	Button mainMenuButton(buttonImages_.at("mainMenu"), sf::Vector2f(hudView_.getCenter().x - buttonSize.x / 2.f, hudView_.getCenter().y));
	activeButtons_.insert({ "continue", continueButton });
	activeButtons_.insert({ "mainMenu", mainMenuButton });

	//volume slider
	//create the background for the volume slider
	sf::Sprite volumeBackground(menuImages_.at("volume"));
	volumeBackground.setOrigin(volumeBackground.getLocalBounds().size / 2.f);
	volumeBackground.setPosition(sf::Vector2f(hudView_.getCenter().x - buttonSize.x / 2.f, hudView_.getCenter().y - buttonSize.y));
	activeImages_.push_back(volumeBackground);

	//create and add the button for the volume slider
	sf::Vector2f knobSize = (sf::Vector2f)buttonImages_.at("volumeKnob").at("unhovered").getSize();
	Button volumeKnob(buttonImages_.at("volumeKnob"), volumeBackground.getGlobalBounds().position + knobSize, volumeBackground.getGlobalBounds().position.x, volumeBackground.getGlobalBounds().size.x);
	activeButtons_.insert({ "volumeKnob", volumeKnob });

	//add the "paused" text
	sf::Text pausedText(font_);
	pausedText.setString("PAUSED");
	pausedText.setCharacterSize(50);
	pausedText.setFillColor(sf::Color::White);
	pausedText.setPosition({ hudView_.getCenter().x - pausedText.getLocalBounds().size.x / 2.f, hudView_.getCenter().y - buttonSize.y * 3.f });
	activeText_.push_back(pausedText);
}

void GameManager::SetupLevelCleared() {
	//update the game state and wipe any previous menu
	state_ = GameState::LEVEL_CLEARED;
	ClearMenu();

	//create and add the text needed for this menu
	sf::Text infoText(font_);
	infoText.setString("Level cleared!");
	infoText.setCharacterSize(50);
	infoText.setFillColor(sf::Color::White);
	infoText.setPosition({ hudView_.getCenter().x - (infoText.getLocalBounds().size.x / 2.f), hudView_.getCenter().y - 50.f });
	activeText_.push_back(infoText);

	//add the button needed for this menu
	sf::Vector2f buttonSize = (sf::Vector2f)buttonImages_.at("start").at("hovered").getSize();

	Button continueButton(buttonImages_.at("continue"), sf::Vector2f(hudView_.getCenter().x - buttonSize.x / 2.f, hudView_.getSize().y - buttonSize.y * 1.5f));
	activeButtons_.insert({ "continue", continueButton });
}

void GameManager::SetupLevelFailed(std::string reason) {
	//update the game state and wipe any previous menu
	state_ = GameState::LEVEL_FAILED;
	ClearMenu();

	//create and add the text needed for this menu
	sf::Text infoText(font_);
	infoText.setString(reason);
	infoText.setCharacterSize(50);
	infoText.setFillColor(sf::Color::White);
	infoText.setPosition({ hudView_.getCenter().x - (infoText.getLocalBounds().size.x / 2.f), hudView_.getCenter().y - 50.f });
	activeText_.push_back(infoText);

	sf::Text restartText(font_);
	restartText.setString("Press R to restart!");
	restartText.setCharacterSize(50);
	restartText.setFillColor(sf::Color::White);
	restartText.setPosition({ hudView_.getCenter().x - (restartText.getLocalBounds().size.x / 2.f), hudView_.getCenter().y + 50.f });
	activeText_.push_back(restartText);

	//add the button needed for this menu
	sf::Vector2f buttonSize = (sf::Vector2f)buttonImages_.at("start").at("hovered").getSize();

	Button continueButton(buttonImages_.at("continue"), sf::Vector2f(hudView_.getCenter().x - buttonSize.x / 2.f, hudView_.getSize().y - buttonSize.y * 1.5f));
	activeButtons_.insert({ "continue", continueButton });
}

void GameManager::SetupLevel() {
	//update the game state and wipe any previous menu
	state_ = GameState::IN_LEVEL;
	ClearMenu();

	sf::Text ammoText(font_);
	ammoText.setCharacterSize(50);
	ammoText.setFillColor(sf::Color::White);
	activeText_.push_back(ammoText);

	sf::Text enemiesText(font_);
	enemiesText.setCharacterSize(50);
	enemiesText.setFillColor(sf::Color::White);
	activeText_.push_back(enemiesText);
}

//Only the HUD has variables that update over time -- all other menus only need to be set up and then drawn.
void GameManager::UpdateHud() {

	//while IN_LEVEL, activeText_ contains 2 identical empty sf::Text objects; ask the Level instance to assign their strings and positions
	activeText_[0].setString(currentLevel_[0].CurrentAmmoString());
	activeText_[0].setPosition({ 20.f, 20.f });

	activeText_[1].setString(currentLevel_[0].EnemiesRemainingString());
	activeText_[1].setPosition({ hudView_.getSize().x - activeText_[1].getLocalBounds().size.x - 40.f, 20.f });
}

void GameManager::UpdateVolumeSlider(sf::Vector2f const inMousePos) {

	//create a local variable for the volume knob Button (for readability)
	Button volumeKnob = activeButtons_.at("volumeKnob");

	//determine the X position based on the mouse's X position clamped within the bounds of the volume slider
	float posX = inMousePos.x;
	if (posX > volumeKnob.GetMaxX()) posX = volumeKnob.GetMaxX();
	else if (posX < volumeKnob.GetMinX()) posX = volumeKnob.GetMinX();

	//the Y position will remain the same
	float posY = activeButtons_.at("volumeKnob").GetSprite().getPosition().y;

	//set the volume knob Button's position accordingly
	activeButtons_.at("volumeKnob").GetSprite().setPosition({ posX, posY });

	//generate a volume value from the position of the volume knob relative to its slider
	float volume = (volumeKnob.GetSprite().getPosition().x - volumeKnob.GetMinX()) / (volumeKnob.GetMaxX() - volumeKnob.GetMinX());
	std::cout << volume;
}

//Draw the currently-active menu objects
void GameManager::DrawMenu() {

	//set the view to be drawn to
	window_.setView(hudView_);
	
	//draw a background image, if one exists
	for (int i = 0; i < activeImages_.size(); i++)
	{
		window_.draw(activeImages_[i]);
	}

	//draw any text that exists
	for (int i = 0; i < activeText_.size(); i++)
	{
		window_.draw(activeText_[i]);
	}

	//draw any buttons that exist
	for (auto& [name, button] : activeButtons_)
	{
		button.CheckIsHovered(window_.mapPixelToCoords(sf::Mouse::getPosition(), hudView_));
		
		window_.draw(button.GetSprite());
	}
}

#pragma endregion

#pragma region Input Management

//Handle all events from the queue -- closing window, singular key presses, mouse clicks
void GameManager::HandleEventQueue() {

	//handle input from the event queue
	while (const std::optional event = window_.pollEvent())
	{
		//closing window
		if (event->is<sf::Event::Closed>())
		{
			window_.close();
		}

		//key press events
		else if (const auto* keyPressed = event->getIf<sf::Event::KeyPressed>())
		{
			//menu progression key presses
			switch (state_)
			{

			//while in the title, pressing any key progresses to the main menu
			case GameManager::GameState::TITLE:
				SetupMainMenu();
				break;

			//while in level, pressing Esc pauses the game, and R requests a weapon reload
			case GameManager::GameState::IN_LEVEL:
				if (keyPressed->scancode == sf::Keyboard::Scancode::Escape)
				{
					SetupPause();
				}
				else if (keyPressed->scancode == sf::Keyboard::Scancode::R)
				{
					currentLevel_[0].CommandReload();
				}
				break;

			//while the game is paused, pressing Esc unpauses the game, and Backspace returns to the main menu
			case GameManager::GameState::PAUSED:
				if (keyPressed->scancode == sf::Keyboard::Scancode::Escape)
				{
					SetupLevel();
				}
				break;

			//while the player is detected, R restarts the current level
			case GameManager::GameState::LEVEL_FAILED:
				if (keyPressed->scancode == sf::Keyboard::Scancode::R)
				{
					CreateLevel(currentLevel_[0].GetLevelId());
					SetupLevel();
				}
				break;

			default:
				break;
			}
		}

		//mouse click events
		else if (const auto* mouseButtonPressed = event->getIf<sf::Event::MouseButtonPressed>())
		{
			if (mouseButtonPressed->button == sf::Mouse::Button::Left)
			{	
				//record the mouse position when the left button was clicked
				sf::Vector2f mousePos = window_.mapPixelToCoords(sf::Mouse::getPosition(), hudView_);

				//check button clicks for menu progression
				switch (state_)
				{

				//while in the title, clicking anywhere progresses to the main menu
				case GameManager::GameState::TITLE:
					SetupMainMenu();
					break;

				case GameManager::GameState::MAIN_MENU:
					for (auto& [name, button] : activeButtons_)
					{
						if (button.CheckIsHovered(mousePos)) 
						{
							if (name == "start") 
							{
								CreateLevel(0); 
								SetupLevel();
								break;
							}
							if (name == "howToPlay") 
							{
								SetupHowToPlay();
								break;
							}
							if (name == "volumeKnob")
							{
								UpdateVolumeSlider(mousePos);
								break;
							}
							if (name == "quit") 
							{
								window_.close();
								break;
							}
						}
					}
					break;

				case GameManager::GameState::HOW_TO_PLAY:
					for (auto& [name, button] : activeButtons_) 
					{
						if (button.CheckIsHovered(mousePos))
						{
							CreateLevel(0);
							SetupLevel();
							break;
						}
					}
					break;

				//while in level, clicking shoots the character's weapon
				case GameManager::GameState::IN_LEVEL:
					currentLevel_[0].CommandShoot();
					break;

				case GameManager::GameState::PAUSED:
					for (auto& [name, button] : activeButtons_) 
					{
						if (button.CheckIsHovered(mousePos))
						{
							if (name == "continue")
							{
								SetupLevel();
								break;
							}
							if (name == "mainMenu")
							{
								//destroy existing level
								currentLevel_.clear();
								SetupMainMenu();
								break;
							}
						}
					}
					break;

				case GameManager::GameState::LEVEL_FAILED:
					for (auto& [name, button] : activeButtons_) 
					{
						if (button.CheckIsHovered(mousePos))
						{
							CreateLevel(currentLevel_[0].GetLevelId());
							SetupLevel();
							break;
						}
					}
					break;

				case GameManager::GameState::LEVEL_CLEARED:
					for (auto& [name, button] : activeButtons_)
					{
						if (button.CheckIsHovered(mousePos))
						{
							if (currentLevel_[0].GetLevelId() < allLevelPlans_.size() - 1)
							{
								CreateLevel(currentLevel_[0].GetLevelId() + 1);
								SetupLevel();
								break;
							}
							else
							{
								//destroy existing level
								currentLevel_.clear();
								SetupTitle();
								break;
							}
						}
					}
					break;

				default:
					break;
				}
			}
		}
	}
}

//Handle all movement input, which consists of the WASD keys held down or released
void GameManager::HandleMovementInput() {
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
	if (currentLevel_.size() > 0) HandleMovementInput();
}

//Create a new Level instance if the previous level has been cleared, otherwise tell the Level instance to update its entities
void GameManager::Update(float const deltaTime) {    
   
	if (state_ == GameManager::GameState::IN_LEVEL) {

		//update the level and level HUD
		currentLevel_[0].Update(deltaTime);
		UpdateHud();


		//check win condition
		//check if the level has been cleared (happens when the number of enemies in the level reaches 0)
		if (currentLevel_[0].GetEnemyCount() == 0) 
		{
			SetupLevelCleared();
		}

		//check lose conditions
		//check if the player has been detected
		if (currentLevel_[0].CheckCharacterDetected()) 
		{
			SetupLevelFailed("You were spotted by a guard!");
		}
		//check if a dead enemy was detected
		if (currentLevel_[0].CheckBodyDetected())
		{
			SetupLevelFailed("A guard found a body!");
		}
		//check if the player shot themselves
		if (currentLevel_[0].CheckCharacterShot())
		{
			SetupLevelFailed("You shot yourself!");
		}
	}
}

//If a Level instance exists, tell it to draw its entities
void GameManager::Draw(float const deltaTime) {

	//if a level currently exists
    if (currentLevel_.size() > 0) 
	{
		//pass along the instruction to draw
        currentLevel_[0].Draw(deltaTime);
    }

	//draw the current menu objects
	DrawMenu();
}

#pragma endregion