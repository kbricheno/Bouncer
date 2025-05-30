#pragma once

#include <fstream>

#include <SFML/Graphics.hpp>

#include "Level.h"

class GameManager {
public:
	GameManager(); // receive txt file, spit out allLevels_
	~GameManager() { instantiated_ = false; }

	void SetupWindow(sf::RenderWindow* const window);
	void ReadLevels(std::ifstream& const levelsFile);

	void HandleInput(float const deltaTime);
	void Update(float const deltaTime);
	void Draw(float const deltaTime);

	// getters
	sf::Vector2u const GetScreenDimensions() const { return sf::Vector2u(screenWidth_, screenHeight_); }

	void TogglePause() { paused_ = !paused_; }
	void ToggleLevelCleared() { levelCleared_ = !levelCleared_; }

private:
	static bool instantiated_;

	sf::RenderWindow *window_ = nullptr;
	int const screenWidth_ = /*960*/1920;
	int const screenHeight_ = /*540*/1080;
	int const tileSize_ = 100;

	bool running_ = true;
	bool paused_ = false;

	// level control variables
	std::vector<std::vector<char>> allLevels_;
	
	bool levelCleared_ = true;
	std::vector<Level> currentLevel_; // only ever need 1 of these -- is there a way to avoid using a vector? can't define a "null" Level instance to be filled during construction

	// camera state?
};