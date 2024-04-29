#include <SFML/Graphics.hpp>
#include "sfmlPanZoomHandler.h"
#include "matrix.h"
#include <chrono>
#include <iostream>
#include "matrix.h"
#include "MLPNeural.h"
#include "graph.h"

std::vector<int>neuralConfig({ 1,5,4,3,3,2,1 });
MLPNeural testNet(neuralConfig);

float computeCostAgainst(float (func)(float)) {
    float rangeStart = -10;
    float rangeStop = 10;
    float rangeStep = 0.1;
    float cost = 0;
    for (float i = rangeStart; i < rangeStop; i += rangeStep) {
        matrix inp(1, 1);
        matrix res(1, 1);
        inp[0][0] = i;
        testNet.computeFeedForward(inp, res);
        cost += powf(res[0][0] - func(i), 2);
    }
    return cost;
}

float testAgainst(float i) {
    return sin(i);
}

int main()
{
    srand(5);
    sfmlPanZoomHandler mainWin(sf::VideoMode(200, 200), "SFML works!");
    sfmlPanZoomHandler graphWin(sf::VideoMode(200, 200), "graph");

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

    
    
    graph mainGraph;

    graphObj graphLine1(sf::Color::Red, [](float i) -> float {
        matrix inp(1, 1);
        matrix res(1, 1);
        inp[0][0] = i;
        testNet.computeFeedForward(inp, res);
        return res[0][0];
    });
    graphObj graphLine2(sf::Color::Green, testAgainst);

    testNet.randomize();

    matrix inp({ {1.2 } });
    matrix result(1, 1);

    //testNet.weightMatrices[0][0][0] = 1;
    //testNet.weightMatrices[1][0][0] = 1;
    //
    //testNet.weightMatrices[0][0][1] = -1;
    //testNet.weightMatrices[1][1][0] = -1;


    //testNet.weightMatrices[0] = matrix({ 
    //    {0.2, 0.3, 0.4, 0.5},
    //    {0.3, 0.2, 0.1, 0.5},
    //    {0.1, 0.2, 0.3, 0.5}
    //});

    //testNet.weightMatrices[1] = matrix({
    //    {0.2, 0.5},
    //    {0.1, 0.3},
    //    {0.5, 0.4},
    //    {0.7, 0.6}
    //});

    mainGraph.insertGraphFunctions(graphLine1);
    mainGraph.insertGraphFunctions(graphLine2);

    graphWin.zoom = 0.1;
    graphWin.view.setCenter(sf::Vector2f());

    std::cout << computeCostAgainst(testAgainst) << '\n';;

    while (mainWin.window.isOpen() && graphWin.window.isOpen())
    {
        while (mainWin.window.pollEvent(mainWin.event))
        {
            if (mainWin.event.type == sf::Event::Closed)
                mainWin.window.close();
            mainWin.handleEventPanZoom();
        }

        while (graphWin.window.pollEvent(graphWin.event))
        {
            if (graphWin.event.type == sf::Event::Closed)
                graphWin.window.close();
            graphWin.handleEventPanZoom();
        }

        //training
        //traing weight
        for (int i = 0; i < 10; i++) {
            float stepSize = 0.001;
            int selectedWeightLayer = rand()%(testNet.weightMatrices.size());
            int selectedRow = rand()%(testNet.weightMatrices[selectedWeightLayer].rowCount);
            int selectedColumn = rand()%(testNet.weightMatrices[selectedWeightLayer].columnCount);
            float originalValue = testNet.weightMatrices[selectedWeightLayer][selectedRow][selectedColumn];
            float magScale = std::fmaxf(0.01f, std::abs(testNet.weightMatrices[selectedWeightLayer][selectedRow][selectedColumn]));
            testNet.weightMatrices[selectedWeightLayer][selectedRow][selectedColumn] = originalValue + (stepSize) / magScale;
            float upScore = computeCostAgainst(testAgainst);
            testNet.weightMatrices[selectedWeightLayer][selectedRow][selectedColumn] = originalValue - stepSize / magScale;
            float downScore = computeCostAgainst(testAgainst);
            if (upScore < downScore) {
                testNet.weightMatrices[selectedWeightLayer][selectedRow][selectedColumn] = originalValue + stepSize;
                std::cout << upScore << " " << selectedWeightLayer << " " << selectedRow << " " << selectedColumn << "\n";
            }
            else {
                std::cout << downScore << " " << selectedWeightLayer << " " << selectedRow << " " << selectedColumn << "\n";
            }
        }
        //train bias
        for (int i = 0; i < 10; i++) {
            float stepSize = 0.001;
            int selectedWeightLayer = rand() % (testNet.biasMatrices.size());
            int selectedColumn = rand() % testNet.biasMatrices[selectedWeightLayer].columnCount;
            float originalValue = testNet.biasMatrices[selectedWeightLayer][0][selectedColumn];
            testNet.biasMatrices[selectedWeightLayer][0][selectedColumn] = originalValue + stepSize;
            float upScore = computeCostAgainst(testAgainst);
            testNet.biasMatrices[selectedWeightLayer][0][selectedColumn] = originalValue - stepSize;
            float downScore = computeCostAgainst(testAgainst);
            if (upScore < downScore) {
                testNet.biasMatrices[selectedWeightLayer][0][selectedColumn] = originalValue + stepSize;
                std::cout << upScore << " " << selectedWeightLayer << " " << 0 << " " << selectedColumn << "\n";
            }
            else {
                std::cout << downScore << " " << selectedWeightLayer << " " << 0 << " " << selectedColumn << "\n";
            }
        }
        //training end

        mainWin.window.clear();
        testNet.renderSelf(mainWin.window);
        mainWin.window.display();

        graphWin.window.clear();
        mainGraph.renderSelf(graphWin.window);
        graphWin.window.display();
    }

    return 0;
}