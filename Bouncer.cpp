// Bouncer.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <fstream>
#include "GameManager.h"

int main()
{
    //create some global variables needed for the game's setup
    sf::Clock clock; //used to generate deltaTime
    
    sf::VideoMode videoMode(sf::VideoMode::getDesktopMode().size); //create a VideoMode with the desktop's size
    
    sf::RenderWindow window(videoMode, "Bouncer", sf::Style::Close, sf::State::Fullscreen); //create the fullscreen game window
    window.setKeyRepeatEnabled(false); //turn off key repeat to prevent multiple key press events being triggered by holding a key down
    
    sf::Font font("Assets/UI/AGENCYR.ttf"); //load the font needed for any and all Text objects
    
    std::ifstream levelsFile("Assets/levels.txt"); //load the file containing all level plans for the game

    sf::Music bgMusic("Assets/Sound/music.wav"); //load the background music


    //create a GameManager instance and tell it to set up the level plans, sounds, and textures needed for creating levels
    GameManager manager(window, font);
    manager.PrepareLevelGeneration(levelsFile);

    //loop that runs the entire time the game window is open
    while (window.isOpen()) 
    {
        if (bgMusic.getStatus() == sf::Music::Status::Stopped) bgMusic.play(); //if the background music isn't playing, play it
        bgMusic.setVolume(AudioComponent::m_globalVolume * 100); //set the background music's volume to the static global volume variable

        sf::Time deltaTime = clock.restart(); //restart the clock to obtain the elapsed time between this frame and the last

        //handle input
        manager.HandleInput(deltaTime.asSeconds()); //tell the GameManager instance to receive and handle the user's input

        //update
        manager.Update(deltaTime.asSeconds()); //tell the GameManager instance to call Update on every object

        //draw
        window.clear(sf::Color::Black); //clear the next buffer using the colour black
        manager.Draw(deltaTime.asSeconds()); //tell the GameManager instance to draw everything to the next buffer
        window.display(); //flip the buffer
    }

    return 0;
}

