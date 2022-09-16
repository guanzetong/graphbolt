#include <iostream>
#include <fstream>
#include "../../core/common/parseCommandLine.h"
#include <omp.h>
#include <stdlib.h>

using namespace std;

#define VERTEX_NUM 41652230
#define NUM_THREADS 16
#define OUTPUT_FILE "seeds.txt"
#define PROB_0 5
#define PROB_1 5

int main(int argc, char *argv[])
{
    commandLine P(argc, argv);
    long int vertex_num = P.getOptionLongValue("-vertexNum", VERTEX_NUM);
    int set_num_threads = P.getOptionIntValue("-numThreads", NUM_THREADS);
    int prob_1 = P.getOptionIntValue("-prob1", PROB_1);
    int prob_0 = P.getOptionIntValue("-prob0", PROB_0);
    string output_file = P.getOptionValue("-o", OUTPUT_FILE);

    long int i;
    int random_num;
    bool *seeds_flag = new bool[vertex_num];
    int num_threads;
    long int seeds_size;
    int rand_res;
    omp_set_num_threads(set_num_threads);
    #pragma omp parallel for default(none) private(i, random_num, rand_res) shared(seeds_flag, num_threads, vertex_num, cout, prob_0, prob_1)
    for (i = 0; i < vertex_num; i++)
    {
        // if(i == 0)
        // {
        //     num_threads = omp_get_num_threads();
        //     cout << "Number of threads: " << num_threads << "\n";
        // }
        rand_res = rand() % (prob_0 + prob_1);
        if (rand_res < prob_0)
        {
            seeds_flag[i] = false;
        }
        else
        {
            seeds_flag[i] = true;
        }
        // cout << seeds_flag[i] << "\n";
    }

    ofstream ofile;
    ofile.open(output_file);
    seeds_size = 0;
    for (i = 0; i < vertex_num; i++)
    {
        if (seeds_flag[i] == 1)
        {
            ofile << i << "\n";
            // cout << i << "\n";
            seeds_size++;
        }
    }
    cout << seeds_size << "\n";
    ofile.close();

    free(seeds_flag);
}