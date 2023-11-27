#include "Matrix.h"

const double Matrix::infinities_percent = 50.0;
const int Matrix::default_output_wide = 10.0;

Matrix::Matrix(const int& size)
{
	outputWide = default_output_wide;

	this->size = size;

	values = new int[size * size] {0};
}

Matrix::Matrix(const int* values, const int& size)
{
	this->values = new int[size * size];
	this->size = size;
	this->outputWide = default_output_wide;
	memcpy_s(this->values, this->size * this->size * sizeof(int), values, size * size * sizeof(int));
}

Matrix::~Matrix()
{
	delete[] values;
}

void Matrix::serial_floyd()
{
	int t1, t2;
	for (int k = 0; k < size; k++)
	{
		for (int i = 0; i < size; i++)
		{
			for (int j = 0; j < size; j++)
			{
				if ((values[i * size + k] != -1) &&
					(values[k * size + j] != -1)) {
					t1 = values[i * size + j];
					t2 = values[i * size + k] + values[k * size + j];
					values[i * size + j] = min(t1, t2);
				}
			}
		}
	}
}

void Matrix::random_data_initialization()
{
	srand((unsigned)time(0));

	for (int i = 0; i < size; i++)
	{
		for (int j = 0; j < size; j++)
		{
			if (i != j)
			{
				if ((rand() % 100) < infinities_percent)
				{
					values[i * size + j] = -1;
				}
				else
				{
					values[i * size + j] = rand() + 1;
				}
			}
			else
			{
				values[i * size + j] = 0;
			}
		}
	}
}

std::string Matrix::to_string() const
{
	std::stringstream string;
	for (size_t i = 0; i < size; i++)
	{
		for (size_t j = 0; j < size; j++)
		{
			string << std::setw(outputWide) << std::fixed << std::setprecision(3) << values[i * size + j];
		}
		string << std::setw(0) << std::endl;
	}
	return string.str();
}

int Matrix::get_size() const
{
	return size;
}

int*& Matrix::get_values()
{
	return values;
}

void Matrix::set_output_wide(const int& outputWide)
{
	this->outputWide = outputWide;
}

Matrix::Matrix(const Matrix& matrix)
{
	values = new int[matrix.size * matrix.size];
	size = matrix.size;
	memcpy_s(values, size * size * sizeof(size), matrix.values, matrix.size * matrix.size * sizeof(size));
	outputWide = matrix.outputWide;
}

bool Matrix::operator==(const Matrix& other)
{
	const static double accuracy = 1.e-6;

	if (this->size != other.size)
	{
		return false;
	}

	for (size_t i = 0; i < size; i++)
	{
		for (size_t j = 0; j < size; j++)
		{
			if (fabs(other.values[i * size + j] - values[i * size + j] >= accuracy))
			{
				return false;
			}
		}
	}

	return true;
}

int& Matrix::operator[](const int& index) const
{
	return values[index];
}

Matrix& Matrix::operator+=(const Matrix& other)
{
	for (size_t i = 0; i < size; i++)
	{
		for (size_t j = 0; j < size; j++)
		{
			values[i * size + j] += other.values[i * size + j];
		}
	}
	return *this;
}

int Matrix::min(const int& a, const int& b)
{
	int result = (a < b) ? a : b;
	if ((a < 0) && (b >= 0)) result = b;
	if ((b < 0) && (a >= 0)) result = a;
	if ((a < 0) && (b < 0)) result = -1;
	return result;
}

std::ostream& operator<<(std::ostream& out, const Matrix& matrix)
{
	out << matrix.to_string();

	return out;
}
