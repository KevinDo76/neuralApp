#pragma once
#include <vector>
#include "matrix.h"
#include "SFML/Graphics.hpp"
class MLPNeural
{
public:
	std::vector<int>layerConfig;
	std::vector<matrix>weightMatrices;
	std::vector<matrix>biasMatrices;
public:
	MLPNeural(std::vector<int> layerConfig);
	void renderSelf(sf::RenderWindow& win);
	void computeFeedForward(matrix& input, matrix& output);
	void applyActivation(matrix& input);
	float activationFunc(float i);
private:
};

