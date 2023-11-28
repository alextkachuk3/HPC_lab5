#pragma once
#include <mpi.h> 
#include <iostream>
#include <string>

#include "../Matrix.h"

class HPC
{
public:
	HPC(int argc, char* argv[]);
	~HPC();

	void parallel_floyd();

	int get_process_rank() const;
	int get_process_num() const;
	
	Matrix& get_matrix();
	void set_matrix(const Matrix& array);
private:
	void data_distribution();
	void result_collection();
	void row_distribution(int k);

	int process_num = 0;
	int process_rank = -1;

	Matrix matrix;

	int* process_row;
	int* process_values;
	int size = 0;
	int block_size = 0;

	void log(std::string message);
};

