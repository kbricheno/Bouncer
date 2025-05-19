#pragma once
#include <vector>
#include <fstream>
#include <string>

#include <SFML/Graphics.hpp>

#include "Level.h"

class GameManager {
public:
	GameManager(); // receive txt file, spit out allLevels_
	~GameManager() { instantiated_ = false; }

	void SetupWindow(sf::RenderWindow* window);
	void ReadLevels(std::ifstream& levelsFile);
	void Update();

	// getters
	const sf::Vector2u GetScreenDimensions() { return sf::Vector2u(screenWidth_, screenHeight_); }

	void TogglePause() { paused_ = !paused_; }
	void ToggleLevelCleared() { levelCleared_ = !levelCleared_; }

private:
	static bool instantiated_;

	sf::RenderWindow *window_ = nullptr;
	const int screenWidth_ = 1920;
	const int screenHeight_ = 1080;
	const int tileSize_ = 100;

	bool running_ = true;
	bool paused_ = false;

	// level control variables
	std::vector<std::vector<char>> allLevels_;
	
	bool levelCleared_ = true;
	Level *currentLevel_ = nullptr;

	// camera state?
};