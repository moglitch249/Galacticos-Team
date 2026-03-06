#include <SFML/Graphics.hpp>
using namespace sf;

int main() 
{
    RenderWindow window(VideoMode({800, 600}), "My Window");

    while (window.isOpen())
    {
        while (const auto event = window.pollEvent())
        {
            if (event->is<sf::Event::Closed>())
                window.close();
        }

        window.clear();
        window.display();
    }

    return 0;
}
