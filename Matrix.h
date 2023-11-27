#pragma once
#include <iostream>
#include <iomanip>
#include <sstream>

class Matrix
{
public:
	Matrix();
	Matrix(const int& size);
	Matrix(const Matrix& other);
	~Matrix();

	void serial_floyd();
	static int min(const int& a, const int& b);

	void random_data_initialization();

	std::string to_string() const;
	int get_size() const;
	int*& get_values();

	void set_output_wide(const int& outputWide);

	Matrix& operator=(const Matrix& other);
	bool operator==(const Matrix& other);
	int& operator[](const int& index) const;
	Matrix& operator+=(const Matrix& other);

	friend std::ostream& operator<< (std::ostream& out, const Matrix& matrix);

private:
	int size;
	int* values;

	int outputWide;

	const static double infinities_percent;
	const static int default_output_wide;
};
