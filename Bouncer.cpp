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

    GameManager gm;

    sf::VideoMode vMode(gm.GetScreenDimensions());
    sf::RenderWindow window(vMode, "Bouncer", sf::Style::Close);

    std::ifstream levelsFile("Assets/levels.txt");

    //debug text
    sf::Font f("Assets/UI/AGENCYR.ttf");
    sf::Text debugText(f);

    gm.SetupWindow(&window);
    gm.ReadLevels(levelsFile);

    while (window.isOpen()) 
    {
        //handle input
        gm.HandleInput();

        // update
        sf::Time deltaTime = clock.restart();
        debugText.setFont(f);
        debugText.setCharacterSize(50);
        debugText.setPosition({ 500, 500 });
        debugText.setString("");
        gm.Update(deltaTime.asSeconds());

        // draw
        window.clear(sf::Color::White);
        gm.Draw();
        window.draw(debugText);
        window.display();
    }

    return 0;
}

