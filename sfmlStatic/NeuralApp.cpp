#include <SFML/Graphics.hpp>
#include "sfmlPanZoomHandler.h"
#include "matrix.h"
int main()
{
    sfmlPanZoomHandler win(sf::VideoMode(200, 200), "SFML works!");
    sf::CircleShape shape(100.f);
    shape.setFillColor(sf::Color::Green);
    std::vector<std::vector<float>> a = { {20,20,20,15},
                                          {20,20,20,2 }};
    matrix ma(a);
    matrix maa(std::vector<std::vector<float>>({{1,0},
                                                {0,1}}));
    std::cout << maa << "\n";
    std::cout << ma << "\n";
    std::cout << maa*ma;
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