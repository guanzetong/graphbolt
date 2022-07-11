#include <iostream>
#include <fstream>
#include "../../core/common/parseCommandLine.h"
#include <omp.h>

using namespace std;
#define BASIC_GRAPH_FILE "../../inputs/soc-pokec-relationships.txt"
#define INIT_GRAPH_FILE "../../inputs/snapshot_pokec_500000/init_graph_500000.snap"
#define IDLE_GRAPH_FILE "../../inputs/snapshot_pokec_500000/idle_graph_500000.snap"
#define MAX_EDGE_NUM 30622564

struct edge_t
{
    long int src_id;
    long int des_id;

    bool operator==(const edge_t &other)
    {
        return ((src_id == other.src_id) && (des_id == other.des_id));
    }
};

int read_graph_from_file(string file_path, edge_t *edge_list)
{
    ifstream fin;
    long int edge_list_idx;
    string line;
    size_t tab_pos;
    // Src ID
    string src_str;
    long int src_int;
    // Des ID
    string des_str;
    long int des_int;

    fin.open(file_path);
    edge_list_idx = 0;
    if (!fin.is_open())
        return 0;
    while (!fin.eof())
    {
        getline(fin, line);
        if ((line[0] != '#') && (line.size() > 0))
        {
            tab_pos = line.find(' ', 0);
            src_str = line.substr(0, tab_pos);
            des_str = line.substr((tab_pos + 1), line.size());
            src_int = stoi(src_str);
            des_int = stoi(des_str);
            edge_list[edge_list_idx].src_id = src_int;
            edge_list[edge_list_idx].des_id = des_int;
            edge_list_idx++;
        }
    }
    fin.close();
    cout << "Read edges from file " << file_path << " : " << edge_list_idx << "\n";
    return edge_list_idx;
}

int diff_compute(edge_t *basic_edge_list, edge_t *init_edge_list, edge_t *idle_edge_list, long int init_edge_num, long int basic_edge_num)
{
    int thread_id;
    int idle_idx = 0;
    int num_threads;
    long int i, j;
    bool found;
    
#pragma omp parallel private(thread_id, i, j, found) shared(idle_idx, num_threads, basic_edge_list, init_edge_list, idle_edge_list)
    {
#pragma omp single
        {
            num_threads = omp_get_num_threads();
            cout << "Number of threads: " << num_threads << "\n";
        }
        thread_id = omp_get_thread_num();

        for (i = thread_id; i < basic_edge_num; i += num_threads)
        {
            found = false;
            for (j = 0; j < init_edge_num; j++)
            {
                if (basic_edge_list[i] == init_edge_list[j])
                {
                    found = true;
                    break;
                }
            }
            if (found == false)
            {
#pragma omp critical
                {
                    idle_edge_list[idle_idx].src_id = basic_edge_list[i].src_id;
                    idle_edge_list[idle_idx].des_id = basic_edge_list[i].des_id;
                    idle_idx++;
                }
            }
        }
    }
    cout << "Number of Idle edges: " << idle_idx << "\n";
    return idle_idx;
}

int write_idle_list_to_file(string file_path, edge_t *idle_edge_list, long int idle_edge_num) {
    ofstream fout;
    fout.open(file_path);
    if (!fout.is_open())
        return 1;
    for (long int i = 0; i < idle_edge_num; i++)
    {
        fout << idle_edge_list[i].src_id << "\t" << idle_edge_list[i].des_id << "\n";
    }
    fout.close();
    cout << "Written into file " << file_path << "\n";
    return 0;
}

int main(int argc, char *argv[])
{
    commandLine P(argc, argv);
    long int max_edge_num = P.getOptionLongValue("-maxEdgeNum", MAX_EDGE_NUM);
    string basic_graph_file = P.getOptionValue("-basic", BASIC_GRAPH_FILE);
    string init_graph_file = P.getOptionValue("-init", INIT_GRAPH_FILE);
    string idle_graph_file = P.getOptionValue("-idle", IDLE_GRAPH_FILE);
    edge_t *basic_edge_list = new edge_t[max_edge_num];
    edge_t *init_edge_list = new edge_t[max_edge_num];
    edge_t *idle_edge_list = new edge_t[max_edge_num];

    long int basic_edge_num;
    long int init_edge_num;
    long int idle_edge_num;

    basic_edge_num = read_graph_from_file(basic_graph_file, basic_edge_list);
    init_edge_num = read_graph_from_file(init_graph_file, init_edge_list);
    idle_edge_num = diff_compute(basic_edge_list, init_edge_list, idle_edge_list, init_edge_num, basic_edge_num);

    write_idle_list_to_file(idle_graph_file, idle_edge_list, idle_edge_num);
}