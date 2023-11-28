#pragma comment(linker, "/STACK:200000000")

#include <iostream>
#include <iomanip>
#include <chrono>

#include "HPC.h"

bool print_values = false;
bool evaluation_test = false;

void test_sorting(HPC& hpc, const int& size)
{
	std::cout << std::endl << "The number of vertices: " << size << std::endl << std::endl;

	Matrix matrix(size);
	matrix.random_data_initialization();

	if (print_values)
	{
		std::cout << "Matrix: " << std::endl << matrix << std::endl;
	}

	hpc.set_matrix(matrix);

	auto start = std::chrono::high_resolution_clock::now();

	hpc.parallel_floyd();

	auto finish = std::chrono::high_resolution_clock::now();

	if (print_values)
	{
		std::cout << "The matrix after parallel Floyd algorithm:: " << std::endl << hpc.get_matrix();
	}

	double duration = std::chrono::duration_cast<std::chrono::nanoseconds>(finish - start).count() * 1.0e-9;

	std::cout << "Time of execution: " << std::fixed << std::setprecision(8) << duration << std::setprecision(4) << std::endl << std::endl;

	matrix.serial_floyd();

	if (hpc.get_matrix() == matrix)
	{
		std::cout << "The results of parallel and sequential Floyd algorithms are identical!" << std::endl;
	}
	else
	{
		std::cout << "The results of parallel and sequential Floyd algorithms are NOT identical!" << std::endl;
	}
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

	HPC hpc(argc, argv);

	int evaluation_sizes[] = { 10, 500, 600, 700, 800, 900, 1000 };

	if (hpc.get_process_rank() == 0)
	{
		if (evaluation_test)
		{
			for (int i = 0; i < sizeof(evaluation_sizes) / sizeof(int); i++)
			{
				test_sorting(hpc, evaluation_sizes[i]);
			}
			return 0;
		}
		else
		{
			std::cout << "Enter the number of vertices:";

			int size;
			std::cin >> size;

			test_sorting(hpc, size);
			return 0;
		}
	}
	else
	{
		if (evaluation_test)
		{
			for (int i = 0; i < sizeof(evaluation_sizes) / sizeof(int); i++)
			{
				hpc.parallel_floyd();
			}
		}
		else
		{
			hpc.parallel_floyd();
		}
	}

	return 0;
}
