#include "HPC.h"

HPC::HPC(int argc, char* argv[])
{
	MPI_Init(&argc, &argv);
	MPI_Comm_size(MPI_COMM_WORLD, &process_num);
	MPI_Comm_rank(MPI_COMM_WORLD, &process_rank);
	setvbuf(stdout, 0, _IONBF, 0);
}

HPC::~HPC()
{
	MPI_Finalize();
}

int HPC::get_process_rank() const
{
	return process_rank;
}

int HPC::get_process_num() const
{
	return process_num;
}

Matrix& HPC::get_matrix()
{
	return matrix;
}

void HPC::set_matrix(const Matrix& matrix)
{
	this->matrix = matrix;
}

void HPC::log(std::string message)
{
	std::cout << ("Process rank:" + std::to_string(process_rank) + " " + message + "\n");
	MPI_Barrier(MPI_COMM_WORLD);
}
