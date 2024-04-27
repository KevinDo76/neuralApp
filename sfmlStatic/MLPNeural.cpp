#include "MLPNeural.h"
#include "matrix.h"
#define circlRad 20
#define colorScale 1

float MLPNeural::activationFunc(float i) {
	return std::fmax(0, i);
}

void MLPNeural::applyActivation(matrix& input) {
	for (int row = 0; row < input.rowCount; row++) {
		for (int column = 0; column < input.columnCount; column++) {
			input[row][column] = this->activationFunc(input[row][column]);
		}
	}
}

MLPNeural::MLPNeural(std::vector<int> layerCfg) {
	if (layerCfg.size() < 3) {
		throw std::invalid_argument("layer configuration list is too short, 3 minimum. One for Input Layer, one for a hidden layer, and one for output");
	}
	for (int i = 0; i < layerCfg.size(); i++) {
		if (layerCfg[i] < 1) {
			throw std::invalid_argument("Layer size is less than 1");
		}
	}
	//weight/bias matrices generation
	for (int i = 1; i < layerCfg.size() - 1; i++) {
		std::cout << layerCfg[i] << "\n";
	}

	this->layerConfig = layerCfg;

	for (int i = 1; i < layerCfg.size(); i++) {
		this->weightMatrices.emplace_back(layerCfg[i-1], layerConfig[i]);
		this->biasMatrices.emplace_back( 1,layerCfg[i]);	
	}
}

void MLPNeural::computeFeedForward(matrix& input, matrix& output) {
	if (input.columnCount != this->layerConfig[0]) {
		throw std::invalid_argument("invalid input matrix dimension");
	}

	if (output.columnCount != this->layerConfig[this->layerConfig.size()-1]) {
		throw std::invalid_argument("invalid input matrix dimension");
	}

	matrix workingLine = input;
	for (int i = 0; i < this->weightMatrices.size(); i++) {
		workingLine = workingLine * this->weightMatrices[i];
		workingLine = workingLine + this->biasMatrices[i];
		this->applyActivation(workingLine);
		std::cout << workingLine << "\n";
	}
}

void MLPNeural::renderSelf(sf::RenderWindow& win) {

	for (int i = 0; i < this->layerConfig.size() - 1; i++) {
		for (int startCircleIndex = 0; startCircleIndex < this->layerConfig[i]; startCircleIndex++) {

			for (int endCircleIndex = 0; endCircleIndex < this->layerConfig[i + 1]; endCircleIndex++) {
				sf::VertexArray buff(sf::Lines, 2);
				buff[0].position = sf::Vector2f(sf::Vector2f(i * 150 + circlRad, circlRad + startCircleIndex * 100 - (this->layerConfig[i] * 100 / 2)));
				buff[1].position = sf::Vector2f(sf::Vector2f((i + 1) * 150 + circlRad, circlRad + endCircleIndex * 100 - (this->layerConfig[i + 1] * 100 / 2)));
				sf::Color lineColor = sf::Color::Green;
				if (this->weightMatrices[i][startCircleIndex][endCircleIndex] < 0) {
					lineColor = sf::Color::Red;
				}

				lineColor.a = std::abs(this->weightMatrices[i][startCircleIndex][endCircleIndex]) / colorScale * 255;

				buff[0].color = lineColor;
				buff[1].color = lineColor;
				win.draw(buff);
			}

		}
	}

	for (int i = 0; i < this->layerConfig.size(); i++) {
		for (int circleIndex = 0; circleIndex < this -> layerConfig[i]; circleIndex++) {
			sf::CircleShape circle;
			circle.setRadius(circlRad);
			circle.setPosition(sf::Vector2f(i * 150, circleIndex * 100 - (this->layerConfig[i]*100/2)));
			win.draw(circle);
		}
	}

}