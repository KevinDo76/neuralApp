#include "matrix.h"
#include <iostream>
#include <vector>
matrix::matrix(const std::vector<std::vector<float>>& init) : data(init) {
	if (init.size() > 0) { //check for empty array
		int rowSize = init[0].size();
		if (rowSize < 1) {
			throw std::invalid_argument("Empty Matrix");
		}

		for (int column = 0; column < init.size(); column++) {
			if (!(init[column].size() == rowSize)) { //check for row with different sizes
				throw std::invalid_argument("Inconsistence row size");
			}
		}

	}
	else {
		throw std::invalid_argument("Empty matrix");
	}
}

matrix::matrix(int r, int c) {
	for (int row = 0; row < r; row++) {
		data.push_back({});
		for (int column = 0; column < c; column++) {
			data[row].push_back(0);
		}
	}
}

matrix matrix::operator+(const matrix& obj) {
	if (this->data.size() == obj.data.size() && this->data[0].size() == obj.data[0].size()) {
		matrix newMatrix(this->data.size(), this->data[0].size());
		for (int row = 0; row < obj.data.size(); row++) {
			for (int column = 0; column < obj.data[row].size(); column++) {
				newMatrix.data[row][column] = data[row][column] + obj.data[row][column];
			}
		}
		return newMatrix;
	}
	else {
		throw std::invalid_argument("Mismatch matrix size");
	}
}

matrix matrix::operator-(const matrix& obj) {
	if (this->data.size() == obj.data.size() && this->data[0].size() == obj.data[0].size()) {
		matrix newMatrix(this->data.size(), this->data[0].size());
		for (int row = 0; row < obj.data.size(); row++) {
			for (int column = 0; column < obj.data[row].size(); column++) {
				newMatrix.data[row][column] = data[row][column] - obj.data[row][column];
			}
		}
		return newMatrix;
	}
	else {
		throw std::invalid_argument("Mismatch matrix size");
	}
}

matrix matrix::operator*(float scalar) {
	matrix newMatrix(this->data.size(), this->data[0].size());
	for (int row = 0; row < this->data.size(); row++) {
		for (int column = 0; column < this->data[row].size(); column++) {
			newMatrix.data[row][column] = this->data[row][column] * scalar;
		}
	}
	return newMatrix;
}

matrix matrix::operator*(matrix m2) {
	if (this->data[0].size() == m2.data.size()) {
		matrix newMatrix(this->data.size(), this->data[0].size());
		std::cout << data.size() << " " << m2.data[0].size() << "\n";
		return newMatrix;
	}
	else {
		throw std::invalid_argument("Mismatch dimension");
	}
}

std::ostream& operator<<(std::ostream& os, const matrix& ma) {
	for (int columns = 0; columns < ma.data.size(); columns++) {
		for (int row = 0; row < ma.data[columns].size(); row++) {
			os << ma.data[columns][row] << " ";
		}
		os << "\n";
	}

	return os;
}