#pragma once
#include "Level.h"
#include "Button.h"

class GameManager {
public:
	//Construct the GameManager instance, ensuring only one instance can be created
	GameManager(sf::RenderWindow& const inWindowRef, sf::Font& const inFontRef) : m_windowRef(inWindowRef), m_fontRef(inFontRef)
	{
		assert(!m_isInstantiated);
		m_isInstantiated = true;
	}
	~GameManager() { m_isInstantiated = false; }

	//Enum for all of the game states
	enum class GameState {
		TITLE,
		MAIN_MENU,
		HOW_TO_PLAY,
		IN_LEVEL,
		PAUSED,
		LEVEL_FAILED,
		LEVEL_CLEARED
	};

	//Create the level plans, sounds, and images needed for creating Level instances
	void PrepareLevelGeneration(std::ifstream& const inLevelsFile);
	
	//Handle all user input
	void HandleInput(float const deltaTime);
	//Update the game; pass the Update command to the current Level instance and check the Level's win/lose conditions
	void Update(float const deltaTime);
	//Draw everything in the game to the next buffer
	void Draw(float const deltaTime);


private:
	static bool m_isInstantiated; //static bool that ensures only one instance of GameManager can ever be created

	sf::RenderWindow& const m_windowRef; //reference to the window so GameManager can draw menus and pass it to Level
	sf::Font& const m_fontRef; //reference to the font so GameManager can create Text objects in menu screens

	int const m_tileSize = 100; //size of each tile in the game, used in Level to determine the spacing between objects


	//LEVEL GENERATION - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
	std::vector<std::vector<char>> m_allLevelPlans; //used in Level to determine which entity to spawn
	std::vector<sf::Vector2i> m_allLevelSizes; //used in Level to determine the dimension of the current level
	std::map<std::string, std::map<std::string, std::vector<sf::Texture>>> m_allAnimations; //used in Level to pass animations to entities' VisualComponents
	std::map<std::string, std::map<std::string, sf::SoundBuffer>> m_allSoundBuffers; //used in Level to pass sound effects to entities' AudioComponents
	std::vector<Level> m_currentLevel; //the current Level instance

	//Create m_allLevelPlans and m_allLevelSizes needed for Level instances to generate entities (one plan per level)
	bool GenerateLevelPlan(std::ifstream& const inLevelsFile);
	//Create m_allAnimations (every Texture for the entire game), store them in a giant ugly map of maps of vectors
	bool GenerateTextures();
	//Create and store every SoundBuffer needed for the entire game
	bool GenerateSoundEffects();
	//Delete any existing level, then create and store a new one
	void CreateLevel(int const inLevelId);


	//MENUS & HUD - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
	GameState m_state = GameState::TITLE; //which menu or level state the game is currently in

	sf::View m_menuView; //view for the menu objects to be drawn to

	std::map<std::string, sf::Texture> m_menuImages; //map of Textures used to create menu background images
	std::vector<sf::Sprite> m_activeMenuImages; //vector of menu background images that need to be drawn this frame
	std::map<std::string, std::map<std::string, sf::Texture>> m_buttonImages; //map of Textures used to create Buttons for menus
	std::map<std::string, Button> m_activeMenuButtons; //vector of Buttons that need to be drawn this frame
	std::vector<sf::Text> m_activeMenuText; //vector of Text objects that need to be drawn this frame
	
	//Clear any existing background image, buttons, and text objects currently stored
	void ClearMenu();

	//Set up each unique menu's background image (or lack thereof), Buttons, and text objects
	void SetupTitle();
	void SetupMainMenu();
	void SetupHowToPlay();
	void SetupLevelHud();
	void SetupPause();
	void SetupLevelCleared();
	void SetupLevelFailed(std::string const inReasonForFailure);
	
	//Only the HUD has variables that update over time -- all other menus only need to be set up and then drawn.
	void UpdateLevelHud();
	void UpdateVolumeSlider(sf::Vector2f const inMousePos);
	
	//Draw the currently-active menu objects
	void DrawMenu();


	//USER INPUT - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
	//Handle all events from  queue -- closing window, singular key presses, mouse clicks
	void HandleEventQueue();
	//Handle all movement input, which consists of the WASD keys held down or released
	void HandleMovementInput();
};