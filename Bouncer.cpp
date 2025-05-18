// Bouncer.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <SFML/Graphics.hpp>

const int screenWidth = 1920;
const int screenHeight = 1080;

int main()
{
    sf::VideoMode vm({ screenWidth, screenHeight });
    sf::RenderWindow window(vm, "Bouncer", sf::Style::Close);

    while (window.isOpen()) 
    {
        // handle input
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Escape))
        {
            window.close();
        }

        // update

        // draw
        window.clear();
        window.display();
    }

    return 0;
}

