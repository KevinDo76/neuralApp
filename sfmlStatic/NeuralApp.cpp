#include <SFML/Graphics.hpp>
#include "sfmlPanZoomHandler.h"
#include "matrix.h"
#include <chrono>
int main()
{
    sfmlPanZoomHandler win(sf::VideoMode(200, 200), "SFML works!");
    sf::CircleShape shape(100.f);
    shape.setFillColor(sf::Color::Green);
    std::vector<std::vector<float>> a = { {1,2,3},
                                          {4,5,6},
                                          {7,8,9}};
    matrix ma(a);
    matrix maa(std::vector<std::vector<float>>({{1,0,0},
                                                {0,1,0},
                                                {0,0,1}}
    
    ));
    std::cout << maa << "\n";
    std::cout << ma << "\n";
    std::cout << "start\n";
    auto start = std::chrono::system_clock::now();
    for (int i = 0; i < 10000000; i++) {
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