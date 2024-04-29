#include "matrix.h"
#include <iostream>
#include <vector>
#include <thread>
matrix::matrix(const std::vector<std::vector<float>>& init) : data(init) {
	if (init.size() > 0) { //check for empty array
		int rowS = init.size();
		this->rowCount = rowS;
		this->columnCount = init[0].size();
		if (rowS < 1) {
			throw std::invalid_argument("Empty Matrix");
		}
	
		for (int row = 0; row < init.size(); row++) {
			if (!(init[row].size() == columnCount)) { //check for row with different sizes
				throw std::invalid_argument("Inconsistence row size");
			}
		}
	
	}
	else {
		throw std::invalid_argument("Empty matrix");
	}
}

matrix::matrix(int r, int c) : rowCount(r), columnCount(c) {
	data.reserve(rowCount);
	for (int row = 0; row < r; row++) {
		data.emplace_back();
		data[row].reserve(row);
		for (int column = 0; column < c; column++) {
			data[row].emplace_back();
		}
	}
}

matrix matrix::operator+(const matrix& obj) {
	if (this->columnCount == obj.columnCount && this->rowCount == obj.rowCount) {
		matrix newMatrix(this->rowCount, this->columnCount);
		for (int column = 0; column < this->columnCount; column++) {
			for (int row = 0; row < obj.rowCount; row++) {
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
	if (this->columnCount == obj.columnCount && this->rowCount == obj.rowCount) {
		matrix newMatrix(this->rowCount, this->columnCount);
		for (int column = 0; column < obj.columnCount; column++) {
			for (int row = 0; row < obj.rowCount; row++) {
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
	matrix newMatrix(this->columnCount, this->rowCount);
	for (int column = 0; column < this->columnCount; column++) {
		for (int row = 0; row < this->rowCount; row++) {
			newMatrix.data[row][column] = this->data[row][column] * scalar;
		}
	}
	return newMatrix;
}

std::vector<float>& matrix::operator[](int index) {
	if (index < this->rowCount) {
		return this->data[index];
	}
	else {
		std::cout << this->rowCount << " " << index << " out of bound\n";
		//throw std::invalid_argument("Index out of bound");
	}
}

//data[row][column]
matrix matrix::operator*(matrix m2) {
	if ((*this).columnCount == m2.rowCount) {
		matrix newMatrix(this->rowCount, m2.columnCount);

		for (int row1 = 0; row1 < newMatrix.rowCount; ++row1) {
			
			std::vector<std::thread> threads;

			for (int column1 = 0; column1 < newMatrix.columnCount; ++column1) {
				float sum = 0;
				
				for (int sumIndex = 0; sumIndex < this->columnCount; ++sumIndex) {
					sum += (*this)[row1][sumIndex] * m2[sumIndex][column1]; // second one is vertically indexed
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