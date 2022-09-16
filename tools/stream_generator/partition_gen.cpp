#include <iostream>
#include <fstream>
#include "../../core/common/parseCommandLine.h"
#include <omp.h>
#include <stdlib.h>

using namespace std;

#define VERTEX_NUM 41652230
#define NUM_THREADS 16
#define OUTPUT_FILE "partition.txt"

int main(int argc, char *argv[])
{
    commandLine P(argc, argv);
    long int vertex_num = P.getOptionLongValue("-vertexNum", VERTEX_NUM);
    int set_num_threads = P.getOptionIntValue("-numThreads", NUM_THREADS);
    string output_file = P.getOptionValue("-o", OUTPUT_FILE);

    long int i;
    int random_num;
    bool *partition_flag = new bool[vertex_num];
    int num_threads;
    long int partition1_size;
    omp_set_num_threads(set_num_threads);
    #pragma omp parallel for default(none) private(i, random_num) shared(partition_flag, num_threads, vertex_num, cout)
    for (i = 0; i < vertex_num; i++)
    {
        // if(i == 0)
        // {
        //     num_threads = omp_get_num_threads();
        //     cout << "Number of threads: " << num_threads << "\n";
        // }
        partition_flag[i] = (bool)(rand() % 2);
        // cout << partition_flag[i] << "\n";
    }

    ofstream ofile;
    ofile.open(output_file);
    partition1_size = 0;
    for (i = 0; i < vertex_num; i++)
    {
        if (partition_flag[i] == 1)
        {
            ofile << i << "\n";
            // cout << i << "\n";
            partition1_size++;
        }
    }
    cout << partition1_size << "\n";
    ofile.close();

    free(partition_flag);
}