// Bouncer.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <fstream>
#include "GameManager.h"

int main()
{
    //create some variables needed for the game's setup
    sf::Clock clock;
    sf::VideoMode videoMode(sf::VideoMode::getDesktopMode().size);
    sf::RenderWindow window(videoMode, "Bouncer", sf::Style::Close, sf::State::Fullscreen);
    window.setKeyRepeatEnabled(false); //turn off key repeat to prevent multiple key press events being triggered by holding a key down
    sf::Font font("Assets/UI/AGENCYR.ttf");
    std::ifstream levelsFile("Assets/levels.txt");

    //create a GameManager instance and tell it to set up the level plans, sounds, and textures needed for creating levels
    GameManager manager(window, font);
    manager.PrepareLevelGeneration(levelsFile);

    //load the background music
    sf::Music bgMusic("Assets/Sound/music.wav");

    while (window.isOpen()) 
    {
        if (bgMusic.getStatus() == sf::Music::Status::Stopped) bgMusic.play(); //if the background music isn't playing, play it
        bgMusic.setVolume(AudioComponent::m_globalVolume * 100); //set the background music's volume to the static global volume variable

        sf::Time deltaTime = clock.restart(); //restart the clock to obtain the elapsed time between this frame and the last

        //handle input
        manager.HandleInput(deltaTime.asSeconds()); //tell the GameManager instance to receive and handle the user's input

        //update
        manager.Update(deltaTime.asSeconds()); //tell the GameManager instance to update every object

        //draw
        window.clear(sf::Color::Black); //clear the next buffer using black
        manager.Draw(deltaTime.asSeconds()); //tell the GameManager instance to draw everything to the next buffer
        window.display(); //flip the buffer
    }

    return 0;
}

