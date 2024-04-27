#pragma once
#include <vector>
#include <iostream>
class matrix
{
public:
	std::vector<std::vector<float>> data;
	matrix(const std::vector<std::vector<float>>& init);
	matrix(int c, int r);

	int rowCount;
	int columnCount;

	friend std::ostream& operator<<(std::ostream& os, const matrix& ma);
	matrix operator+(const matrix& obj);
	matrix operator-(const matrix& obj);
	matrix operator*(float scalar);
	matrix operator*(matrix m2);
	std::vector<float>& operator[](int index);
private:
};

