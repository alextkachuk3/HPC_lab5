#pragma once
#include <mpi.h> 
#include <iostream>
#include <string>

#include "../Matrix.h"

enum SplitMode { KEEP_FIRST_HALF, KEEP_SECOND_HALF };

class HPC
{
public:
	HPC(int argc, char* argv[]);
	~HPC();

	int get_process_rank() const;
	int get_process_num() const;
	
	Matrix& get_matrix();
	void set_matrix(const Matrix& array);
private:
	int process_num = 0;
	int process_rank = -1;

	Matrix matrix;

	int* process_values;
	int size = 0;
	int block_size = 0;

	void log(std::string message);
};

