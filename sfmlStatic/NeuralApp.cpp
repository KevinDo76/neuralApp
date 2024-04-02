#include <SFML/Graphics.hpp>
#include "sfmlPanZoomHandler.h"
int main()
{
    sfmlPanZoomHandler win(sf::VideoMode(200, 200), "SFML works!");
    sf::CircleShape shape(100.f);
    shape.setFillColor(sf::Color::Green);

    while (win.window.isOpen())
    {
        while (win.window.pollEvent(win.event))
        {
            if (win.event.type == sf::Event::Closed)
                win.window.close();
            win.handleEventPanZoom();
        }

        win.window.clear();
        win.window.draw(shape);
        win.window.display();
    }

    return 0;
}