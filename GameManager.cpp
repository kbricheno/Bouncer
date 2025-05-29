#include "GameManager.h"
#include <iostream>

bool GameManager::instantiated_ = false; // define the static bool

GameManager::GameManager() {
    // allow only 1 instance of GameManager to exist
    assert(!instantiated_);
    instantiated_ = true;
}

void GameManager::SetupWindow(sf::RenderWindow* const window) {
    window_ = window;
}

void GameManager::ReadLevels(std::ifstream& const levelsFile) {
    std::vector<char> currentLevel;
    char currentChar;

    if (levelsFile.is_open()) {
        while (levelsFile.good()) {
            currentChar = levelsFile.get();
            std::cout << currentChar << "\n";
            currentLevel.push_back(currentChar);
        }
    }

    allLevels_.push_back(currentLevel);
}

void GameManager::HandleInput() {
    if (currentLevel_.size() > 0) {
        currentLevel_[0].HandleInput();
    }
}

void GameManager::Update(float const deltaTime) {    
    if (levelCleared_) {
        // if a level already exists, the next level's id will be the current one + 1, otherwise we're about to create level 0
        int nextLevelId = currentLevel_.size() > 0 ? currentLevel_[0].GetLevelId() + 1 : 0;
        
        Level nextLevel(window_, nextLevelId, tileSize_, 10, 10, allLevels_[nextLevelId]);
        currentLevel_.push_back(nextLevel);
        
        levelCleared_ = false;
    }
    else {
        currentLevel_[0].Update(deltaTime);
    }
}

void GameManager::Draw(float const deltaTime) {
    if (currentLevel_.size() > 0) {
        currentLevel_[0].Draw(deltaTime);
    }
}