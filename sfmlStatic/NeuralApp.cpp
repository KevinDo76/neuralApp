#include <SFML/Graphics.hpp>
#include "sfmlPanZoomHandler.h"
#include "matrix.h"
#include <chrono>
#include <iostream>
#include "matrix.h"
#include "MLPNeural.h"
int main()
{
    sfmlPanZoomHandler mainWin(sf::VideoMode(400, 400), "SFML works!");

    //matrix a({
    //    {1,2,3},
    //    {4,5,6},
    //    });
    //
    //matrix b(3, 2);
    //
    //b[0][0] = 10;
    //b[1][0] = 20;
    //b[2][0] = 30;
    //
    //b[0][1] = 11;
    //b[1][1] = 21;
    //b[2][1] = 31;
    //
    //std::cout << a << "\n";
    //std::cout << b << "\n";
    //std::cout << a * b << "\n";
    std::vector<int>neuralConfig({ 3,4,2 });

    MLPNeural testNet(neuralConfig);

    matrix inp({ {1.2, 3.5, 5.5} });
    matrix result(1, 2);


    testNet.weightMatrices[0] = matrix({ 
        {0.2, 0.3, 0.4, 0.5},
        {0.3, 0.2, 0.1, 0.5},
        {0.1, 0.2, 0.3, 0.5}
    });

    testNet.weightMatrices[1] = matrix({
        {0.2, 0.5},
        {0.1, 0.3},
        {0.5, 0.4},
        {0.7, 0.6}
    });

    testNet.computeFeedForward(inp, result);

  
    while (mainWin.window.isOpen())
    {
        while (mainWin.window.pollEvent(mainWin.event))
        {
            if (mainWin.event.type == sf::Event::Closed)
                mainWin.window.close();
            mainWin.handleEventPanZoom();
        }


        mainWin.window.clear();
        testNet.renderSelf(mainWin.window);
        mainWin.window.display();
    }

    return 0;
}