#include "matrix.h"
#include <iostream>
#include <vector>
matrix::matrix(const std::vector<std::vector<float>>& init) : data(init) {
	if (init.size() > 0) { //check for empty array
		int rowS = init[0].size();
		this->rowSize = rowSize;
		this->columnSize = init.size();
		if (rowS < 1) {
			throw std::invalid_argument("Empty Matrix");
		}

		for (int column = 0; column < init.size(); column++) {
			if (!(init[column].size() == rowS)) { //check for row with different sizes
				throw std::invalid_argument("Inconsistence row size");
			}
		}

	}
	else {
		throw std::invalid_argument("Empty matrix");
	}
}

matrix::matrix(int r, int c) : rowSize(r), columnSize(c) {
	data.reserve(rowSize);
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

std::vector<float>& matrix::operator[](int index) {
	if (index < this->data.size()) {
		return this->data[index];
	}
	else {
		throw std::invalid_argument("Index out of bound");
	}
}

//data[row][column]
matrix matrix::operator*(matrix m2) {
	if ((*this)[0].size() == m2.data.size()) {
		matrix newMatrix(this->rowSize, m2.columnSize);

		for (int row1 = 0; row1 < newMatrix.rowSize; ++row1) {
			
			for (int column1 = 0; column1 < newMatrix.columnSize; ++column1) {
				float sum = 0;

				for (int column2 = 0; column2 < this->columnSize; ++column2) {
					sum += (*this)[row1][column2] * m2[column2][column1]; // second one is vertically indexed
				}

				newMatrix[row1][column1] = sum;
			}
		}

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