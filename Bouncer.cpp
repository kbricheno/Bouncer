// Bouncer.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <fstream>
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include "GameManager.h"

int main()
{
    sf::Clock clock;

    sf::VideoMode vMode(sf::VideoMode::getDesktopMode().size);
    sf::RenderWindow window(vMode, "Bouncer", sf::Style::Close);
    window.setKeyRepeatEnabled(false);

    std::ifstream levelsFile("Assets/levels.txt");

    //debug text
    sf::Font f("Assets/UI/AGENCYR.ttf");
    sf::Text debugText(f);

    GameManager gm(window, f);
    gm.PrepareLevelGeneration(levelsFile);

    while (window.isOpen()) 
    {
        sf::Time deltaTime = clock.restart();

        //handle input
        gm.HandleInput(deltaTime.asSeconds());

        //update
        debugText.setFont(f);
        debugText.setCharacterSize(50);
        debugText.setPosition({ 500, 500 });
        debugText.setString("");
        gm.Update(deltaTime.asSeconds());

        //draw
        window.clear(sf::Color::Color(0,0,0));
        gm.Draw(deltaTime.asSeconds());
        window.draw(debugText);
        window.display();
    }

    return 0;
}

