#include "GameManager.h"

bool GameManager::instantiated_ = false; // define the static bool

GameManager::GameManager() {
    // allow only 1 instance of GameManager to exist
    assert(!instantiated_);
    instantiated_ = true;
}

void GameManager::SetupWindow(sf::RenderWindow* window) {
    window_ = window;
}

void GameManager::ReadLevels(std::ifstream &levelsFile) {
    std::vector<char> currentLevel;
    char currentChar;

    if (levelsFile.is_open()) {
        while (levelsFile) {
            currentChar = levelsFile.get();
            currentLevel.push_back(currentChar);
        }
    }

    allLevels_.push_back(currentLevel);
}

void GameManager::Update() {
    if (levelCleared_) {
        int lastLevelNum = -1;

        if (currentLevel_) lastLevelNum = currentLevel_->GetLevelId();

        Level newLevel(window_, tileSize_, lastLevelNum + 1, 15, 15, allLevels_[lastLevelNum + 1]);
        currentLevel_ = &newLevel;
        
        levelCleared_ = false;
    }
    else {
        currentLevel_->Update();
    }
}