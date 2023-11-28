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

void HPC::parallel_floyd()
{
	if (process_rank == 0)
	{
		size = matrix.get_size();
	}
	MPI_Bcast(&size, 1, MPI_INT, 0, MPI_COMM_WORLD);
	int rest_rows = size;
	for (int i = 0; i < process_rank; i++)
		rest_rows = rest_rows - rest_rows / (process_num - i);
	int row_num = rest_rows / (process_num - process_rank);

	process_values = new int[size * row_num];
	process_row = new int[size];

	MPI_Barrier(MPI_COMM_WORLD);

	data_distribution();

	int t1, t2;
	for (int k = 0; k < size; k++) {
		row_distribution(k);
		for (int i = 0; i < row_num; i++)
			for (int j = 0; j < size; j++)
				if ((process_values[i * size + k] != -1) &&
					(process_row[j] != -1)) {
					t1 = process_values[i * size + j];
					t2 = process_values[i * size + k] + process_row[j];
					process_values[i * size + j] = Matrix::min(t1, t2);
				}
	}

	MPI_Barrier(MPI_COMM_WORLD);

	result_collection();

	delete[] process_row;
	delete[] process_values;
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

void HPC::data_distribution()
{
	int rest_rows = size;
	int* send_ind = new int[process_num];
	int* send_num = new int[process_num];

	int row_num = size / process_num;
	send_num[0] = row_num * size;
	send_ind[0] = 0;
	for (int i = 1; i < process_num; i++) 
	{
		rest_rows -= row_num;
		row_num = rest_rows / (process_num - i);
		send_num[i] = row_num * size;
		send_ind[i] = send_ind[i - 1] + send_num[i - 1];
	}

	MPI_Scatterv(matrix.get_values(), send_num, send_ind, MPI_INT, process_values, send_num[process_rank], MPI_INT, 0, MPI_COMM_WORLD);
	
	delete[] send_num;
	delete[] send_ind;
}

void HPC::result_collection()
{
	int* receive_num;
	int* receive_ind;
	int rest_rows = size;

	receive_num = new int[process_num];
	receive_ind = new int[process_num];

	int row_num = size / process_num;
	receive_ind[0] = 0;
	receive_num[0] = row_num * size;
	for (int i = 1; i < process_num; i++) {
		rest_rows -= row_num;
		row_num = rest_rows / (process_num - i);
		receive_num[i] = row_num * size;
		receive_ind[i] = receive_ind[i - 1] + receive_num[i - 1];
	}
	MPI_Gatherv(process_values, receive_num[process_rank], MPI_INT, matrix.get_values(), receive_num, receive_ind, MPI_INT, 0, MPI_COMM_WORLD);

	delete[] receive_num;
	delete[] receive_ind;
}

void HPC::row_distribution(int k)
{
	int proc_row_rank;
	int proc_row_num;
	int rest_rows = size;
	int ind = 0;
	int num = size / process_num;

	for (proc_row_rank = 1; proc_row_rank < process_num + 1; proc_row_rank++) {
		if (k < ind + num) break;
		rest_rows -= num;
		ind += num;
		num = rest_rows / (process_num - proc_row_rank);
	}
	proc_row_rank = proc_row_rank - 1;
	proc_row_num = k - ind;
	if (proc_row_rank == process_rank)
	{
		std::copy(&process_values[proc_row_num * size], &process_values[(proc_row_num + 1) * size], process_row);
	}

	MPI_Bcast(process_row, size, MPI_INT, proc_row_rank, MPI_COMM_WORLD);	
}

void HPC::log(std::string message)
{
	std::cout << ("Process rank:" + std::to_string(process_rank) + " " + message + "\n");
	MPI_Barrier(MPI_COMM_WORLD);
}
