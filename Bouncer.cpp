// Bouncer.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <fstream>
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include "GameManager.h"

GameManager gm;

sf::VideoMode vMode(gm.GetScreenDimensions());
sf::RenderWindow window(vMode, "Bouncer", sf::Style::Close);

std::ifstream levelsFile("Assets/levels.txt");

//debug text
sf::Font f("Assets/UI/AGENCYR.ttf");
sf::Text debugText(f);


int main()
{
    gm.SetupWindow(&window);
    gm.ReadLevels(levelsFile);

    while (window.isOpen()) 
    {
        // handle input through event queue
        while (const std::optional event = window.pollEvent())
        {
            if (event != std::nullopt) 
            {
                // closing window
                if (event->is<sf::Event::Closed>())
                {
                    window.close();
                }
                // quit game through escape for now
                else if (const auto* keyPressed = event->getIf<sf::Event::KeyPressed>())
                {
                    if (keyPressed->scancode == sf::Keyboard::Scancode::Escape)
                        window.close();
                }
            }
        }


        // update
        debugText.setFont(f);
        debugText.setCharacterSize(50);
        debugText.setPosition({ 500, 500 });
        debugText.setString("");


        // draw
        window.clear(sf::Color::White);
        gm.Update();
        window.draw(debugText);
        window.display();
    }

    return 0;
}

