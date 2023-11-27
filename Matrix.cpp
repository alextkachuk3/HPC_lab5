#include "Matrix.h"

const double Matrix::infinities_percent = 50.0;
const int Matrix::default_output_wide = 10;

Matrix::Matrix()
{
	outputWide = default_output_wide;
	size = 0;
	values = new int[size] {};
}

Matrix::Matrix(const int& size)
{
	outputWide = default_output_wide;
	this->size = size;
	values = new int[size * size] {};
}

Matrix::Matrix(const Matrix& other)
{
	size = other.size;
	values = new int[size * size];	
	memcpy_s(values, size * size * sizeof(int), other.values, other.size * other.size * sizeof(int));
	outputWide = other.outputWide;
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
				if ((values[i * size + k] != -1) && (values[k * size + j] != -1))
				{
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

Matrix& Matrix::operator=(const Matrix& other)
{
	if (this == &other)
	{
		return *this;
	}

	size = other.size;
	values = new int[size * size];
	memcpy_s(values, size * sizeof(double), other.values, other.size * sizeof(double));
	outputWide = other.outputWide;
	return *this;
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
