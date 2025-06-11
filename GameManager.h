#pragma once

#include <fstream>
#include <SFML/Graphics.hpp>

#include "Level.h"
#include "Button.h"

class GameManager {
public:
	enum class GameState {
		TITLE,
		MAIN_MENU,
		HOW_TO_PLAY,
		IN_LEVEL,
		PAUSED,
		LEVEL_FAILED,
		LEVEL_CLEARED
	};

	//Construct the GameManager instance, ensuring only one instance can be created
	GameManager(sf::RenderWindow& const window, sf::Font& const font) : window_(window), font_(font)
	{
		assert(!instantiated_);
		instantiated_ = true;
	}
	~GameManager() { instantiated_ = false; }

	//game set-up
	void PrepareLevelGeneration(std::ifstream& const levelsFile);
	bool GenerateLevelPlan(std::ifstream& const levelsFile);
	bool GenerateTextures();
	bool GenerateSoundEffects();
	bool CreateButtons();
	void CreateLevel(int const levelId);

	//menus & hud	
	void ClearMenu();
	void SetupTitle();
	void SetupMainMenu();
	void SetupHowToPlay();
	void SetupLevel();
	void SetupPause();
	void SetupLevelCleared();
	void SetupLevelFailed(std::string reason);
	void UpdateHud();
	void DrawMenu();

	//user input
	void HandleEventQueue();
	void HandleMovementInput();

	//game loop
	void HandleInput(float const deltaTime);
	void Update(float const deltaTime);
	void Draw(float const deltaTime);

private:
	static bool instantiated_;

	GameState state_ = GameState::TITLE;

	sf::RenderWindow& window_;
	sf::Font& font_;

	int const tileSize_ = 100;


	//level variables
	std::vector<std::vector<char>> allLevelPlans_; //used in Level to determine which entity to spawn
	std::vector<sf::Vector2i> allLevelSizes_; //used in Level to determine the dimension of the current level
	std::map<std::string, std::map<std::string, std::vector<sf::Texture>>> allAnimations_; //used in Level to pass animations to entities' VisualComponents
	std::map<std::string, std::map<std::string, sf::SoundBuffer>> allSoundEffects_; //used in Level to pass sound effects to entities' AudioComponents

	std::vector<Level> currentLevel_; // only ever need 1 of these -- is there a way to avoid using a vector? can't define a "null" Level instance to be filled during construction


	//hud & menu variables
	sf::View hudView;

	std::map<std::string, sf::Texture> menuImages_;
	std::vector<sf::Sprite> activeImages_;
	
	std::map<std::string, std::map<std::string, sf::Texture>> buttonImages_;
	std::map<std::string, Button> activeButtons_;

	std::vector<sf::Text> activeText_;

	float volumeSlider = 0.f;
};