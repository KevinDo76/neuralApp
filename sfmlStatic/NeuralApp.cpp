#include <SFML/Graphics.hpp>
#include "sfmlPanZoomHandler.h"
#include "matrix.h"
#include <chrono>
int main()
{
    sfmlPanZoomHandler win(sf::VideoMode(200, 200), "SFML works!");
    sf::CircleShape shape(100.f);
    shape.setFillColor(sf::Color::Green);
    std::vector<std::vector<float>> a = { {5,2,1},
                                          {6,2,4},
                                          {-2,4,10}};
    matrix ma(a);
    matrix maa(std::vector<std::vector<float>>({{-1/22.f, 2/11.f, -3/44.f},
                                                {17/22.f, -13/22.f, 7/44.f},
                                                {-7/22.f, 3/11.f, 1/44.f}}
    
    ));
    std::cout << maa << "\n";
    std::cout << ma << "\n";
    std::cout << "start\n";
    auto start = std::chrono::system_clock::now();
    for (int i = 0; i < 1000000; i++) {
        matrix b = maa * ma;
    }
    auto end = std::chrono::system_clock::now();
    auto elapsed = end - start;
    std::cout << elapsed.count()/10000000.f << '\n';
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