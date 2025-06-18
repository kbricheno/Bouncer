#include "GameManager.h"
#include <iostream>
#include <fstream>

#pragma region Game Set-Up

//Define the static bool that allows only one instance of GameManager to exist
bool GameManager::m_isInstantiated = false;

void GameManager::PrepareLevelGeneration(std::ifstream& const inLevelsFile) {
    
	//generate the level plans
	if (!GenerateLevelPlan(inLevelsFile)) std::cout << "ERROR: Could not find the level file path, or file is blank!\n";

	//generate all textures
	if (!GenerateTextures()) std::cout << "ERROR: Could not find all texture file paths!\n";

	//generate all sounds
	if (!GenerateSoundEffects()) std::cout << "ERROR: Could not find all sound effect file paths!\n";

	//prepare the view used for drawing menus and the hud
	m_menuView.setSize((sf::Vector2f)sf::VideoMode::getDesktopMode().size);
	m_menuView.setCenter((sf::Vector2f)sf::VideoMode::getDesktopMode().size / 2.f);

	//set up the title screen
	SetupTitle();
}

void GameManager::CreateLevel(int const inLevelId) {
	//create a Level instance and pass it everything it needs to generate and maintain every entity within the level
	Level nextLevel(m_windowRef, inLevelId, m_tileSize, m_allLevelPlans[inLevelId], m_allLevelSizes[inLevelId], m_allAnimations, m_allSoundBuffers);

	//clear the Level storage vector, then store the Level instance
	m_currentLevel.clear();
	m_currentLevel.push_back(nextLevel);

	//update the game state
	m_state = GameState::IN_LEVEL;
}

#pragma endregion

#pragma region File Loading

bool GameManager::GenerateLevelPlan(std::ifstream& const inLevelsFile) {
	
	//each level will spawn entities based on a level plan (a vector of chars)
	std::vector<char> levelPlan;
	char levelChar;

	//some characters in the file between plans are irrelevant (names, colons, spaces, etc.), so use a bool to determine whether to write to the level plan
	bool writing = false;

	sf::Vector2i levelSize = { 0,1 };

	//read the external file
	if (inLevelsFile.is_open()) {
		while (inLevelsFile.good()) {
			//obtain each char and add it to the current level plan vector
			levelChar = inLevelsFile.get();
			if (writing)
			{
				if (levelChar == '}') //end of level indicator
				{
					writing = false;
					m_allLevelPlans.push_back(levelPlan); //push the finished level plan into the vector
					m_allLevelSizes.push_back(levelSize); //push the calculated level size into the vector
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
	if (m_allLevelPlans.size() == 0) return false; //couldn't find the level file or it was empty
	else return true; //level plans successfully generated
}

bool GameManager::GenerateTextures() {

	//create maps to hold each entity's name and all of its animations
	std::map<std::string, std::vector<sf::Texture>> heroAnimations;
	std::map<std::string, std::vector<sf::Texture>> enemyAnimations;
	std::map<std::string, std::vector<sf::Texture>> bulletAnimations;
	std::map<std::string, std::vector<sf::Texture>> wallAnimations;
	std::map<std::string, std::vector<sf::Texture>> doorHorAnimations;
	std::map<std::string, std::vector<sf::Texture>> doorVerAnimations;
	std::map<std::string, std::vector<sf::Texture>> backgroundAnimations;


	//player animations .....................................................................................................................................................
	//create vectors to hold individual animations -- each will hold a number of frames (images) making up the animation
	std::vector<sf::Texture> heroIdleTextures;
	std::vector<sf::Texture> heroWalkTextures;
	std::vector<sf::Texture> heroShootTextures;
	std::vector<sf::Texture> heroReloadTextures;

	//create a string we can use to generate each frame's file path (frames are saved as numbers in a folder)
	std::string fileName = "";

	//loop through the number of frames in the first animation, idle
	for (int i = 1; i <= 10; i++)
	{
		//create the frame's file path using the iterator to determine the current frame
		fileName = "Assets/Player/idle/" + std::to_string(i) + ".png";

		//create a Texture using the generated file path and add it to the relevant animation vector
		sf::Texture heroIdleFrame;
		if (!heroIdleFrame.loadFromFile(fileName)) return false;
		heroIdleTextures.push_back(heroIdleFrame);
	}

	//add the animation's name and frames to the hero animation map
	heroAnimations.insert({ "idle", heroIdleTextures });

	//repeat the process for the walking animation
	for (int i = 1; i <= 5; i++)
	{
		fileName = "Assets/Player/walk/" + std::to_string(i) + ".png";
		
		sf::Texture heroWalkFrame;
		if (!heroWalkFrame.loadFromFile(fileName)) return false;
		heroWalkTextures.push_back(heroWalkFrame);
	}
	heroAnimations.insert({ "walk", heroWalkTextures });

	//shooting
	for (int i = 1; i <= 3; i++)
	{
		fileName = "Assets/Player/shoot/" + std::to_string(i) + ".png";

		sf::Texture heroShootFrame;
		if (!heroShootFrame.loadFromFile(fileName)) return false;
		heroShootTextures.push_back(heroShootFrame);
	}
	heroAnimations.insert({ "shoot", heroShootTextures });

	//reloading
	for (int i = 1; i <= 4; i++)
	{
		fileName = "Assets/Player/reload/" + std::to_string(i) + ".png";

		sf::Texture heroReloadFrame;
		if (!heroReloadFrame.loadFromFile(fileName)) return false;
		heroReloadTextures.push_back(heroReloadFrame);
	}
	heroAnimations.insert({ "reload", heroReloadTextures });

	//finally, store the hero's name and all hero animations to the "allAnimations" map stored in the GameManager object
	m_allAnimations.insert({ "hero", heroAnimations });


	//enemy animations .....................................................................................................................................................
	std::vector<sf::Texture> enemyIdleTextures;
	std::vector<sf::Texture> enemyWalkTextures;
	std::vector<sf::Texture> enemyDyingTextures;

	//idling
	for (int i = 1; i <= 1; i++)
	{
		fileName = "Assets/Guard/idle/" + std::to_string(i) + ".png";

		sf::Texture enemyIdleFrame;
		if (!enemyIdleFrame.loadFromFile(fileName)) return false;
		enemyIdleTextures.push_back(enemyIdleFrame);
	}
	enemyAnimations.insert({ "idle", enemyIdleTextures });

	//walking
	for (int i = 1; i <= 10; i++)
	{
		fileName = "Assets/Guard/walk/" + std::to_string(i) + ".png";

		sf::Texture enemyWalkFrame;
		if (!enemyWalkFrame.loadFromFile(fileName)) return false;
		enemyWalkTextures.push_back(enemyWalkFrame);
	}
	enemyAnimations.insert({ "walk", enemyWalkTextures });

	//dying
	for (int i = 1; i <= 4; i++)
	{
		fileName = "Assets/Guard/dying/" + std::to_string(i) + ".png";

		sf::Texture enemyDyingFrame;
		if (!enemyDyingFrame.loadFromFile(fileName)) return false;
		enemyDyingTextures.push_back(enemyDyingFrame);
	}
	enemyAnimations.insert({ "die", enemyDyingTextures });

	m_allAnimations.insert({ "enemy", enemyAnimations });


	//bullet Texture .....................................................................................................................................................
	//bullets only have one animation containing one Texture, but must use the same template as other entities for the contextless VisualComponent to work
	std::vector<sf::Texture> bulletIdleTextures;

	sf::Texture bulletIdleFrame;
	if (!bulletIdleFrame.loadFromFile("Assets/Player/bullet.png")) return false;
	bulletIdleTextures.push_back(bulletIdleFrame);	

	bulletAnimations.insert({ "idle", bulletIdleTextures });

	m_allAnimations.insert({ "bullet", bulletAnimations });


	//door animations .....................................................................................................................................................
	std::vector<sf::Texture> horizontalDoorClosedTextures;
	std::vector<sf::Texture> horizontalDoorOpenTextures;
	std::vector<sf::Texture> verticalDoorClosedTextures;
	std::vector<sf::Texture> verticalDoorOpenTextures;

	sf::Texture horDoorIdleFrame;
	if (!horDoorIdleFrame.loadFromFile("Assets/Environment/doorHor.png")) return false;
	horizontalDoorClosedTextures.push_back(horDoorIdleFrame);
	doorHorAnimations.insert({ "idle", horizontalDoorClosedTextures });

	sf::Texture horDoorBrokenFrame;
	if (!horDoorBrokenFrame.loadFromFile("Assets/Environment/doorHorBroken.png")) return false;
	horizontalDoorOpenTextures.push_back(horDoorBrokenFrame);
	doorHorAnimations.insert({ "break", horizontalDoorOpenTextures });

	sf::Texture verDoorIdleFrame;
	if (!verDoorIdleFrame.loadFromFile("Assets/Environment/doorVer.png")) return false;
	verticalDoorClosedTextures.push_back(verDoorIdleFrame);
	doorVerAnimations.insert({ "idle", verticalDoorClosedTextures });

	sf::Texture verDoorBrokenFrame;
	if (!verDoorBrokenFrame.loadFromFile("Assets/Environment/doorVerBroken.png")) return false;
	verticalDoorOpenTextures.push_back(verDoorBrokenFrame);
	doorVerAnimations.insert({ "break", verticalDoorOpenTextures });

	m_allAnimations.insert({ "doorHor", doorHorAnimations });
	m_allAnimations.insert({ "doorVer", doorVerAnimations });


	//wall Texture .....................................................................................................................................................
	std::vector<sf::Texture> wallIdleTextures;

	sf::Texture wallIdleFrame;
	if (!wallIdleFrame.loadFromFile("Assets/Environment/wall.png")) return false;
	wallIdleTextures.push_back(wallIdleFrame);

	wallAnimations.insert({ "idle", wallIdleTextures });

	m_allAnimations.insert({ "wall", wallAnimations });


	//background Textures .....................................................................................................................................................
	std::vector<sf::Texture> background1Texture_;
	std::vector<sf::Texture> background2Texture_;
	std::vector<sf::Texture> background3Texture_;

	sf::Texture backGround1Frame;
	if (!backGround1Frame.loadFromFile("Assets/Environment/map1.png")) return false;
	background1Texture_.push_back(backGround1Frame);
	backgroundAnimations.insert({ "0", background1Texture_ });
	
	sf::Texture backGround2Frame;
	if (!backGround2Frame.loadFromFile("Assets/Environment/map2.png")) return false;
	background2Texture_.push_back(backGround2Frame);
	backgroundAnimations.insert({ "1", background2Texture_ });
	
	sf::Texture backGround3Frame;
	if (!backGround3Frame.loadFromFile("Assets/Environment/map3.png")) return false;
	background3Texture_.push_back(backGround3Frame);
	backgroundAnimations.insert({ "2", background3Texture_ });

	m_allAnimations.insert({ "background", backgroundAnimations });


	//menu Textures .....................................................................................................................................................
	sf::Texture titleBackground;
	if (!titleBackground.loadFromFile("Assets/UI/titleScreen.png")) return false;
	m_menuImages.insert({ "title", titleBackground });

	sf::Texture howToPlayBackground;
	if (!howToPlayBackground.loadFromFile("Assets/UI/instructions.png")) return false;
	m_menuImages.insert({ "howToPlay", howToPlayBackground });

	sf::Texture volumeBackground;
	if (!volumeBackground.loadFromFile("Assets/UI/volume.png")) return false;
	m_menuImages.insert({ "volume", volumeBackground });


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

	m_buttonImages.insert({ "start", startImages });

	sf::Texture continueTexture;
	sf::Texture continueHoveredTexture;
	if (!continueTexture.loadFromFile("Assets/UI/continue.png")) return false;
	if (!continueHoveredTexture.loadFromFile("Assets/UI/continueHover.png")) return false;
	continueImages.insert({ "unhovered", continueTexture });
	continueImages.insert({ "hovered", continueHoveredTexture });

	m_buttonImages.insert({ "continue", continueImages });

	sf::Texture quitTexture;
	sf::Texture quitHoveredTexture;
	if (!quitTexture.loadFromFile("Assets/UI/quit.png")) return false;
	if (!quitHoveredTexture.loadFromFile("Assets/UI/quitHover.png")) return false;
	quitImages.insert({ "unhovered", quitTexture });
	quitImages.insert({ "hovered", quitHoveredTexture });

	m_buttonImages.insert({ "quit", quitImages });

	sf::Texture howToPlayTexture;
	sf::Texture howToPlayHoveredTexture;
	if (!howToPlayBackground.loadFromFile("Assets/UI/howToPlay.png")) return false;
	if (!howToPlayHoveredTexture.loadFromFile("Assets/UI/howToPlayHover.png")) return false;
	howToPlayImages.insert({ "unhovered", howToPlayBackground });
	howToPlayImages.insert({ "hovered", howToPlayHoveredTexture });

	m_buttonImages.insert({ "howToPlay", howToPlayImages });

	sf::Texture mainMenuTexture;
	sf::Texture mainMenuHoveredTexture;
	if (!mainMenuTexture.loadFromFile("Assets/UI/mainMenu.png")) return false;
	if (!mainMenuHoveredTexture.loadFromFile("Assets/UI/mainMenuHover.png")) return false;
	mainMenuImages.insert({ "unhovered", mainMenuTexture });
	mainMenuImages.insert({ "hovered", mainMenuHoveredTexture });

	m_buttonImages.insert({ "mainMenu", mainMenuImages });

	sf::Texture volumeKnobTexture;
	sf::Texture volumeKnobHoveredTexture;
	if (!volumeKnobTexture.loadFromFile("Assets/UI/barKnob.png")) return false;
	if (!volumeKnobHoveredTexture.loadFromFile("Assets/UI/barKnobHover.png")) return false;
	volumeKnobImages.insert({ "unhovered", volumeKnobTexture });
	volumeKnobImages.insert({ "hovered", volumeKnobHoveredTexture });

	m_buttonImages.insert({ "volumeKnob", volumeKnobImages });


	//all textures successfully loaded
	return true;
}

bool GameManager::GenerateSoundEffects() {

	//create maps to hold each entity's name and all of its sound buffers
	std::map<std::string, sf::SoundBuffer> heroSoundEffects;
	std::map<std::string, sf::SoundBuffer> enemySoundEffects;
	std::map<std::string, sf::SoundBuffer> bulletSoundEffects;
	std::map<std::string, sf::SoundBuffer> doorSoundEffects;
	std::map<std::string, sf::SoundBuffer> globalSoundEffects;


	//create & load hero sound buffers
	sf::SoundBuffer heroShoot;
	sf::SoundBuffer heroReload;
	if (!heroShoot.loadFromFile("Assets/Sound/playerShoot.wav")) return false;
	if (!heroReload.loadFromFile("Assets/Sound/playerReload.wav")) return false;

	//store each hero sound buffer in the hero's sounds map
	heroSoundEffects.insert({ "shoot", heroShoot });
	heroSoundEffects.insert({ "reload", heroReload });

	//finally, store the hero's name and all hero sound buffers to the "allSoundEffects" map stored in the GameManager object
	m_allSoundBuffers.insert({ "hero", heroSoundEffects });


	//enemy sound effects
	sf::SoundBuffer enemyDeath;
	if (!enemyDeath.loadFromFile("Assets/Sound/guardDeath.wav")) return false;

	enemySoundEffects.insert({ "die", enemyDeath });

	m_allSoundBuffers.insert({ "enemy", enemySoundEffects });


	//bullet sound effects
	sf::SoundBuffer bulletBounce;
	if (!bulletBounce.loadFromFile("Assets/Sound/bulletBounce.wav")) return false;

	bulletSoundEffects.insert({ "bounce", bulletBounce });

	m_allSoundBuffers.insert({ "bullet", bulletSoundEffects });


	//door sound effects
	sf::SoundBuffer doorBreak;
	if (!doorBreak.loadFromFile("Assets/Sound/doorBreak.wav")) return false;

	doorSoundEffects.insert({ "break", doorBreak });

	m_allSoundBuffers.insert({ "door", doorSoundEffects });


	//global sound effects
	sf::SoundBuffer victory;
	sf::SoundBuffer detected;
	if (!victory.loadFromFile("Assets/Sound/success.wav")) return false;
	if (!detected.loadFromFile("Assets/Sound/houseAlarm.wav")) return false;

	globalSoundEffects.insert({ "victory", victory });
	globalSoundEffects.insert({ "detected", detected });

	m_allSoundBuffers.insert({ "global", globalSoundEffects });


	//all sound buffers successfully loaded
	return true;
}

#pragma endregion

#pragma region Menus & HUD

void GameManager::ClearMenu() {
	m_activeMenuImages.clear();
	m_activeMenuButtons.clear();
	m_activeMenuText.clear();
}

void GameManager::SetupTitle() {
	//update the game state
	m_state = GameState::TITLE;

	//wipe any previous menu
	ClearMenu();

	//create the new menu
	//create a sprite from the title background image
	sf::Sprite titleBackground(m_menuImages.at("title"));
	titleBackground.setOrigin(titleBackground.getLocalBounds().size / 2.f);
	titleBackground.setPosition(m_menuView.getCenter());

	//store the background sprite
	m_activeMenuImages.push_back(titleBackground);
}

void GameManager::SetupMainMenu() {
	//update the game state and wipe any previous menu
	m_state = GameState::MAIN_MENU;
	ClearMenu();

	//add the buttons needed for this menu
	sf::Vector2f buttonSize = (sf::Vector2f)m_buttonImages.at("start").at("hovered").getSize();

	Button startButton(m_buttonImages.at("start"), sf::Vector2f(m_menuView.getCenter().x - buttonSize.x / 2.f, m_menuView.getCenter().y - buttonSize.y * 2.f));
	Button howToPlayButton(m_buttonImages.at("howToPlay"), sf::Vector2f(m_menuView.getCenter().x - buttonSize.x / 2.f, m_menuView.getCenter().y - buttonSize.y));
	Button quitButton(m_buttonImages.at("quit"), sf::Vector2f(m_menuView.getCenter().x - buttonSize.x / 2.f, m_menuView.getCenter().y + buttonSize.y));
	m_activeMenuButtons.insert({ "start", startButton });
	m_activeMenuButtons.insert({ "howToPlay", howToPlayButton });
	m_activeMenuButtons.insert({ "quit", quitButton });

	//create a volume slider
	CreateVolumeSlider(sf::Vector2f(m_menuView.getCenter().x - buttonSize.x / 2.f, m_menuView.getCenter().y));
}

void GameManager::SetupHowToPlay() {
	//update the game state and wipe any previous menu
	m_state = GameState::HOW_TO_PLAY;
	ClearMenu();

	//add the background image and buttons needed for this menu
	sf::Sprite howToPlayBackground(m_menuImages.at("howToPlay"));
	howToPlayBackground.setOrigin(howToPlayBackground.getLocalBounds().size / 2.f);
	howToPlayBackground.setPosition(m_menuView.getCenter());	
	m_activeMenuImages.push_back(howToPlayBackground);

	//add the button needed for this menu
	sf::Vector2f buttonSize = (sf::Vector2f)m_buttonImages.at("start").at("hovered").getSize();

	Button startButton(m_buttonImages.at("start"), sf::Vector2f(m_menuView.getCenter().x - buttonSize.x / 2.f, m_menuView.getSize().y - buttonSize.y * 1.5f));
	m_activeMenuButtons.insert({ "start", startButton });
}

void GameManager::SetupPause() {
	//update the game state and wipe any previous menu
	m_state = GameState::PAUSED;
	ClearMenu();

	//add the buttons needed for this menu
	sf::Vector2f buttonSize = (sf::Vector2f)m_buttonImages.at("start").at("hovered").getSize();

	Button continueButton(m_buttonImages.at("continue"), sf::Vector2f(m_menuView.getCenter().x - buttonSize.x / 2.f, m_menuView.getCenter().y - buttonSize.y * 2.f));
	Button mainMenuButton(m_buttonImages.at("mainMenu"), sf::Vector2f(m_menuView.getCenter().x - buttonSize.x / 2.f, m_menuView.getCenter().y));
	m_activeMenuButtons.insert({ "continue", continueButton });
	m_activeMenuButtons.insert({ "mainMenu", mainMenuButton });

	//create a volume slider
	CreateVolumeSlider(sf::Vector2f(m_menuView.getCenter().x - buttonSize.x / 2.f, m_menuView.getCenter().y - buttonSize.y));

	//add the "paused" text
	sf::Text pausedText(m_fontRef);
	pausedText.setString("PAUSED");
	pausedText.setCharacterSize(50);
	pausedText.setFillColor(sf::Color::White);
	pausedText.setPosition({ m_menuView.getCenter().x - pausedText.getLocalBounds().size.x / 2.f, m_menuView.getCenter().y - buttonSize.y * 3.f });
	m_activeMenuText.push_back(pausedText);
}

void GameManager::SetupLevelCleared() {
	//update the game state and wipe any previous menu
	m_state = GameState::LEVEL_CLEARED;
	ClearMenu();

	//create and add the text needed for this menu
	sf::Text infoText(m_fontRef);
	infoText.setString("Level cleared!");
	infoText.setCharacterSize(50);
	infoText.setFillColor(sf::Color::White);
	infoText.setPosition({ m_menuView.getCenter().x - (infoText.getLocalBounds().size.x / 2.f), m_menuView.getCenter().y - 50.f });
	m_activeMenuText.push_back(infoText);

	//add the button needed for this menu
	sf::Vector2f buttonSize = (sf::Vector2f)m_buttonImages.at("start").at("hovered").getSize();

	Button continueButton(m_buttonImages.at("continue"), sf::Vector2f(m_menuView.getCenter().x - buttonSize.x / 2.f, m_menuView.getSize().y - buttonSize.y * 1.5f));
	m_activeMenuButtons.insert({ "continue", continueButton });
}

void GameManager::SetupLevelFailed(std::string const inReasonForFailure) {
	//update the game state and wipe any previous menu
	m_state = GameState::LEVEL_FAILED;
	ClearMenu();

	//create and add the text needed for this menu
	sf::Text infoText(m_fontRef);
	infoText.setString(inReasonForFailure);
	infoText.setCharacterSize(50);
	infoText.setFillColor(sf::Color::White);
	infoText.setPosition({ m_menuView.getCenter().x - (infoText.getLocalBounds().size.x / 2.f), m_menuView.getCenter().y - 50.f });
	m_activeMenuText.push_back(infoText);

	sf::Text restartText(m_fontRef);
	restartText.setString("Press R to restart!");
	restartText.setCharacterSize(50);
	restartText.setFillColor(sf::Color::White);
	restartText.setPosition({ m_menuView.getCenter().x - (restartText.getLocalBounds().size.x / 2.f), m_menuView.getCenter().y + 50.f });
	m_activeMenuText.push_back(restartText);

	//add the button needed for this menu
	sf::Vector2f buttonSize = (sf::Vector2f)m_buttonImages.at("start").at("hovered").getSize();

	Button continueButton(m_buttonImages.at("continue"), sf::Vector2f(m_menuView.getCenter().x - buttonSize.x / 2.f, m_menuView.getSize().y - buttonSize.y * 1.5f));
	m_activeMenuButtons.insert({ "continue", continueButton });

	//play the house alarm sound
	alarm.setVolume(AudioComponent::m_globalVolume * 100);
	if (alarm.getStatus() == sf::Music::Status::Stopped) alarm.play();
}

void GameManager::SetupLevelHud() {
	//update the game state and wipe any previous menu
	m_state = GameState::IN_LEVEL;
	ClearMenu();

	sf::Text ammoText(m_fontRef);
	ammoText.setCharacterSize(50);
	ammoText.setFillColor(sf::Color::White);
	m_activeMenuText.push_back(ammoText);

	sf::Text enemiesText(m_fontRef);
	enemiesText.setCharacterSize(50);
	enemiesText.setFillColor(sf::Color::White);
	m_activeMenuText.push_back(enemiesText);
}

void GameManager::UpdateLevelHud() {

	//while IN_LEVEL, activeText_ contains 2 identical empty sf::Text objects; ask the Level instance to assign their strings and positions
	m_activeMenuText[0].setString(m_currentLevel[0].GetCurrentAmmoString());
	m_activeMenuText[0].setPosition({ 20.f, 20.f });

	m_activeMenuText[1].setString(m_currentLevel[0].GetEnemiesRemainingString());
	m_activeMenuText[1].setPosition({ m_menuView.getSize().x - m_activeMenuText[1].getLocalBounds().size.x - 40.f, 20.f });
}

void GameManager::DrawMenu() {

	//set the view to be drawn to
	m_windowRef.setView(m_menuView);
	
	//draw a background image, if one exists
	for (int i = 0; i < m_activeMenuImages.size(); i++)
	{
		m_windowRef.draw(m_activeMenuImages[i]);
	}

	//draw any text that exists
	for (int i = 0; i < m_activeMenuText.size(); i++)
	{
		m_windowRef.draw(m_activeMenuText[i]);
	}

	//draw any buttons that exist
	for (auto& [name, button] : m_activeMenuButtons)
	{
		button.CheckIsHovered(m_windowRef.mapPixelToCoords(sf::Mouse::getPosition(), m_menuView));
		
		m_windowRef.draw(button.GetSprite());
	}
}

void GameManager::CreateVolumeSlider(sf::Vector2f const inStartPos) {

	//create the background image for the volume slider, set its position, add it to the active images vector
	sf::Sprite volumeBackground(m_menuImages.at("volume"));
	volumeBackground.setPosition(inStartPos);
	m_activeMenuImages.push_back(volumeBackground);

	//create a shorthand variable for the size of the volume Button just for readability
	sf::Vector2f volumeButtonSize = (sf::Vector2f)m_buttonImages.at("volumeKnob").at("unhovered").getSize();

	//calculate the variables needed for the volume Button constructor
	float minXPos = volumeBackground.getPosition().x + volumeButtonSize.x / 2.f;
	float maxXPos = volumeBackground.getPosition().x + volumeBackground.getLocalBounds().size.x - volumeButtonSize.x / 2.f;
	float startPosX = AudioComponent::m_globalVolume * (maxXPos - minXPos) + minXPos; //the x starting position varies depending on the current global volume
	float startPosY = volumeBackground.getPosition().y + volumeBackground.getLocalBounds().size.y - volumeButtonSize.y;

	//create a slider Button variant, passing in its Textures, starting position, min x position, and max x position -- then add it to the active buttons vector
	Button volumeKnob(m_buttonImages.at("volumeKnob"), { startPosX - volumeButtonSize.x / 2.f, startPosY }, minXPos, maxXPos);
	m_activeMenuButtons.insert({ "volumeKnob", volumeKnob });
}

void GameManager::ChangeVolume(sf::Vector2f const inMousePos) {

	//create a local variable for the volume knob Button (for readability)
	Button volumeKnob = m_activeMenuButtons.at("volumeKnob");

	//determine the X position based on the mouse's X position clamped within the bounds of the volume slider
	float posX = inMousePos.x;
	if (posX > volumeKnob.GetMaxX()) posX = volumeKnob.GetMaxX();
	else if (posX < volumeKnob.GetMinX()) posX = volumeKnob.GetMinX();

	//the Y position will remain the same
	float posY = m_activeMenuButtons.at("volumeKnob").GetSprite().getPosition().y;

	//set the volume knob Button's position accordingly
	m_activeMenuButtons.at("volumeKnob").GetSprite().setPosition({ posX - volumeKnob.GetSprite().getLocalBounds().size.x / 2.f, posY });

	//generate a volume value from the position of the volume knob relative to its slider
	AudioComponent::m_globalVolume = (posX - volumeKnob.GetMinX()) / (volumeKnob.GetMaxX() - volumeKnob.GetMinX());
}

#pragma endregion

#pragma region Input Management

void GameManager::HandleEventQueue() {

	//handle input from the event queue
	while (const std::optional event = m_windowRef.pollEvent())
	{
		//closing window
		if (event->is<sf::Event::Closed>())
		{
			m_windowRef.close();
		}

		//key press events
		else if (const auto* keyPressed = event->getIf<sf::Event::KeyPressed>())
		{
			//menu progression key presses
			switch (m_state)
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
					m_currentLevel[0].CommandReload();
				}
				break;

			//while the game is paused, pressing Esc unpauses the game, and Backspace returns to the main menu
			case GameManager::GameState::PAUSED:
				if (keyPressed->scancode == sf::Keyboard::Scancode::Escape)
				{
					SetupLevelHud();
				}
				break;

			//while the player is detected, R restarts the current level
			case GameManager::GameState::LEVEL_FAILED:
				if (keyPressed->scancode == sf::Keyboard::Scancode::R)
				{
					alarm.stop(); //stop the alarm sound as soon as the game state is no longer LEVEL_FAILED
					CreateLevel(m_currentLevel[0].GetLevelId());
					SetupLevelHud();
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
				sf::Vector2f mousePos = m_windowRef.mapPixelToCoords(sf::Mouse::getPosition(), m_menuView);

				//check button clicks for menu progression
				switch (m_state)
				{

				//while in the title, clicking anywhere progresses to the main menu
				case GameManager::GameState::TITLE:
					SetupMainMenu();
					break;

				case GameManager::GameState::MAIN_MENU:
					for (auto& [name, button] : m_activeMenuButtons)
					{
						if (button.CheckIsHovered(mousePos)) 
						{
							if (name == "start") 
							{
								CreateLevel(0); 
								SetupLevelHud();
								break;
							}
							if (name == "howToPlay") 
							{
								SetupHowToPlay();
								break;
							}
							if (name == "volumeKnob")
							{
								m_isChangingVolume = true;
								break;
							}
							if (name == "quit") 
							{
								m_windowRef.close();
								break;
							}
						}
					}
					break;

				case GameManager::GameState::HOW_TO_PLAY:
					for (auto& [name, button] : m_activeMenuButtons) 
					{
						if (button.CheckIsHovered(mousePos))
						{
							CreateLevel(0);
							SetupLevelHud();
							break;
						}
					}
					break;

				//while in level, clicking shoots the hero's weapon
				case GameManager::GameState::IN_LEVEL:
					m_currentLevel[0].CommandShoot();
					break;

				case GameManager::GameState::PAUSED:
					for (auto& [name, button] : m_activeMenuButtons) 
					{
						if (button.CheckIsHovered(mousePos))
						{
							if (name == "continue")
							{
								SetupLevelHud();
								break;
							}
							if (name == "volumeKnob")
							{
								m_isChangingVolume = true;
								break;
							}
							if (name == "mainMenu")
							{
								//destroy existing level
								m_currentLevel.clear();
								SetupMainMenu();
								break;
							}
						}
					}
					break;

				case GameManager::GameState::LEVEL_FAILED:
					for (auto& [name, button] : m_activeMenuButtons) 
					{
						if (button.CheckIsHovered(mousePos))
						{
							alarm.stop(); //stop the alarm sound as soon as the game state is no longer LEVEL_FAILED
							CreateLevel(m_currentLevel[0].GetLevelId());
							SetupLevelHud();
							break;
						}
					}
					break;

				case GameManager::GameState::LEVEL_CLEARED:
					for (auto& [name, button] : m_activeMenuButtons)
					{
						if (button.CheckIsHovered(mousePos))
						{
							if (m_currentLevel[0].GetLevelId() < m_allLevelPlans.size() - 1)
							{
								CreateLevel(m_currentLevel[0].GetLevelId() + 1);
								SetupLevelHud();
								break;
							}
							else
							{
								//destroy existing level
								m_currentLevel.clear();
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

		//only bother checking for a mouse button release event if the volume is currently being changed
		if (m_isChangingVolume) 
		{
			if (const auto* mouseButtonReleased = event->getIf<sf::Event::MouseButtonReleased>())
			{
				if (mouseButtonReleased->button == sf::Mouse::Button::Left)
				{
					//stop updating the volume slider
					m_isChangingVolume = false;
				}
			}
		}
	}
}

void GameManager::HandleMovementInput() {
	/*
	I avoided using Event::KeyPressed for movement input because key-held-down events are generated with a significant delay (the
	same delay as holding a key down in a word processor). I actually switched Event key repeat off completely so it can be used 
	for singular key presses exclusively -- I don't want users to be able to, e.g., hold down Esc to pause/unpause constantly.
	*/

	//upwards movement
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W))
	{
		m_currentLevel[0].CommandMoveUp(true);
	}
	else
	{
		m_currentLevel[0].CommandMoveUp(false);
	}
	
	//leftwards movement
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A))
	{
		m_currentLevel[0].CommandMoveLeft(true);
	}
	else
	{
		m_currentLevel[0].CommandMoveLeft(false);
	}

	//downward movement
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S))
	{
		m_currentLevel[0].CommandMoveDown(true);
	}
	else
	{
		m_currentLevel[0].CommandMoveDown(false);
	}

	//rightward movement
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D))
	{
		m_currentLevel[0].CommandMoveRight(true);
	}
	else
	{
		m_currentLevel[0].CommandMoveRight(false);
	}
}

#pragma endregion

#pragma region Game Loop

void GameManager::HandleInput(float const deltaTime) {
    
	HandleEventQueue();
	if (m_currentLevel.size() > 0) HandleMovementInput();
}

void GameManager::Update(float const deltaTime) {    
   
	if (m_state == GameManager::GameState::IN_LEVEL) {

		//update the level and level HUD
		m_currentLevel[0].Update(deltaTime);
		UpdateLevelHud();


		//check win condition
		//check if the level has been cleared (happens when the number of enemies in the level reaches 0)
		if (m_currentLevel[0].AreAllEnemiesDead()) 
		{
			SetupLevelCleared();
		}

		//check lose conditions
		//check if the player has been detected
		if (m_currentLevel[0].IsHeroDetected()) 
		{
			SetupLevelFailed("You were spotted by a guard!");
		}
		//check if a dead enemy was detected
		if (m_currentLevel[0].IsBodyDetected())
		{
			SetupLevelFailed("A guard found a body!");
		}
		//check if the player shot themselves
		if (m_currentLevel[0].IsHeroShot())
		{
			SetupLevelFailed("You shot yourself!");
		}
	}
	
	//if currently clicking in a volume slider, update it
	if (m_isChangingVolume) ChangeVolume(m_windowRef.mapPixelToCoords(sf::Mouse::getPosition(), m_menuView));
}

void GameManager::Draw(float const deltaTime) {

	//if a level currently exists
    if (m_currentLevel.size() > 0) 
	{
		//pass along the instruction to draw
        m_currentLevel[0].Draw(deltaTime);
    }

	//draw the current menu objects
	DrawMenu();
}

#pragma endregion