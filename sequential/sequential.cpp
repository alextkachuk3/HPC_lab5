#include <iostream>
#include <iomanip>
#include <chrono>

#include "../Matrix.h"

bool print_values = false;
bool evaluation_test = false;

void test_sequential_floyd_solving(const int& size)
{
	Matrix matrix(size);
	matrix.random_data_initialization();

	if (print_values)
	{
		int outputWide = 10;
		matrix.set_output_wide(outputWide);

		std::cout << "Matrix" << std::endl << matrix;
	}

	auto start = std::chrono::high_resolution_clock::now();

	matrix.serial_floyd();

	auto finish = std::chrono::high_resolution_clock::now();

	auto duration = std::chrono::duration_cast<std::chrono::nanoseconds>(finish - start).count() * 1.0e-9;

	if (print_values)
	{
		std::cout << "The matrix after Floyd algorithm:" << std::endl << matrix;
	}

	std::cout << "Number of vertices: " << size << std::endl;
	std::cout << "Time of execution: " << std::fixed << std::setprecision(12) << duration << std::endl << std::endl;
}

int main(int argc, char* argv[])
{
	srand(clock());

	for (int i = 0; i < argc; i++)
	{
		if (strcmp(argv[i], "-p") == 0)
		{
			print_values = true;
		}
		else if (strcmp(argv[i], "-t") == 0)
		{
			evaluation_test = true;
		}
	}

	if (evaluation_test)
	{
		int evaluation_sizes[] = { 10, 500, 600, 700, 800, 900, 1000 };

		for (int i = 0; i < sizeof(evaluation_sizes) / sizeof(size_t); i++)
		{
			test_sequential_floyd_solving(evaluation_sizes[i]);
		}
		return 0;
	}

	int size;

	std::cout << "Enter the number of vertices:";

	std::cin >> size;

	test_sequential_floyd_solving(size);

	return 0;

}
