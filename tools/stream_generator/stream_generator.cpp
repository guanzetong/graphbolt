#include <iostream>
#include <fstream>
#include <string>
#include <stdlib.h>
#include <bits/stdc++.h>
#include <random>
#include "../../core/common/parseCommandLine.h"


#define BASIC_GRAPH_FILE "../../inputs/sample_graph.snap"
#define MAX_VERTEX_NUM  1632803
#define MAX_EDGE_NUM 30622564

#define INIT_GRAPH_FILE "init_graph.snap"
#define STREAM_GRAPH_FILE "stream_graph.txt"
#define BATCH_SIZE 1000
#define BATCH_NUM 10

using namespace std;

struct edge_t {
    long int src_id;
    long int des_id;
};

void shuffle_edge_list (edge_t *edge_list, long int edge_num)
{
    unsigned seed = 0;
    shuffle(edge_list, edge_list+edge_num, default_random_engine(seed));
    // for (size_t i = 0; i < edge_num; i++)
    // {
    //     cout << "src: " << edge_list[i].src_id << "\tdes: " << edge_list[i].des_id << "\n";
    // }
}

// void mutate_swap (edge_t *edge_list, long int loaded_edge_num, long int batch_size)
// {
    
//     edge_t *temp = new edge_t[batch_size/2];
//     memcpy((void *)temp, (void *)edge_list, (batch_size/2)*sizeof(edge_t));
//     memcpy((void *)edge_list, (void *)(edge_list+loaded_edge_num), (batch_size/2)*sizeof(edge_t));
//     memcpy((void *)(edge_list+loaded_edge_num), (void *)temp, (batch_size/2)*sizeof(edge_t));
//     delete [] temp;
// }

void initial_graph_generator(ofstream &fout, edge_t *edge_list, long int edge_num, edge_t *loaded_edge_list, edge_t *idle_edge_list)
{
    // Shuffle the edge_list
    shuffle_edge_list(edge_list, edge_num);
    // Store loaded edge list
    long int loaded_edge_num = edge_num/2;
    cout << "Loaded edges in initial graph : " << loaded_edge_num << "\n";
    for (size_t i = 0; i < loaded_edge_num; i++)
    {
        fout << edge_list[i].src_id << "\t" << edge_list[i].des_id << "\n";
        loaded_edge_list[i] = edge_list[i];
    }
    // Store idle edge list
    for (size_t i = loaded_edge_num; i < edge_num; i++)
    {
        idle_edge_list[i-loaded_edge_num] = edge_list[i];
    }
}

void batch_generator(ofstream &fout, long int batch_size, edge_t *loaded_edge_list, edge_t *idle_edge_list, long int &loaded_edge_num, long int &idle_edge_num)
{
    edge_t *temp_edge_list = new edge_t[batch_size];
    long int edge_addition_num;
    long int edge_deletion_num;

    // Shuffle edge lists
    shuffle_edge_list(loaded_edge_list, loaded_edge_num);
    shuffle_edge_list(idle_edge_list, idle_edge_num);

    while (true)
    {
        edge_addition_num = random() % (batch_size + 1);
        edge_deletion_num = batch_size - edge_addition_num;
        if ((edge_addition_num <= idle_edge_num)
        && (edge_deletion_num <= loaded_edge_num))
        {
            break;
        }
    }

    // Remove edge additions from idle_edge_list
    for (size_t j = idle_edge_num - edge_addition_num; j < idle_edge_num ; j++)
    {
        fout << "a\t" << idle_edge_list[j].src_id << "\t" << idle_edge_list[j].des_id << "\n";
        temp_edge_list[j - (idle_edge_num - edge_addition_num)] = idle_edge_list[j];
    }
    idle_edge_num = idle_edge_num - edge_addition_num;

    // Remove edge deletions from loaded_edge_list
    for (size_t j = loaded_edge_num - edge_deletion_num; j < loaded_edge_num ; j++)
    {
        fout << "d\t" << loaded_edge_list[j].src_id << "\t" << loaded_edge_list[j].des_id << "\n";
    }
    loaded_edge_num = loaded_edge_num - edge_deletion_num;

    // Integrate edge deletions to idle_edge_list
    for (size_t j = 0; j < edge_deletion_num; j++)
    {
        idle_edge_list[j + idle_edge_num] = loaded_edge_list[j + loaded_edge_num];
    }
    idle_edge_num = idle_edge_num + edge_deletion_num;

    // Integrate edge additions to the loaded_edge_list
    for (size_t j = 0; j < edge_addition_num; j++)
    {
        loaded_edge_list[j + loaded_edge_num] = temp_edge_list[j];
    }
    loaded_edge_num = loaded_edge_num + edge_addition_num;
    cout << "Edge additions : " << edge_addition_num << "\n";
    cout << "Edge deletions : " << edge_deletion_num << "\n";
}

int main (int argc, char *argv[]) 
{
    // Arguments
    commandLine P(argc, argv);
    long int max_edge_num = P.getOptionLongValue("-maxEdgeNum", MAX_EDGE_NUM);
    long int batch_size = P.getOptionLongValue("-batchSize", BATCH_SIZE);
    long int batch_num = P.getOptionLongValue("-batchNum", BATCH_NUM);
    string basic_graph_file = P.getOptionValue("-basicGraphFile", BASIC_GRAPH_FILE);
    string init_graph_file = P.getOptionValue("-initGraphFile", INIT_GRAPH_FILE);
    string stream_graph_file = P.getOptionValue("-streamGraphFile", STREAM_GRAPH_FILE);

    // Basic Graph input file in SNAP format
    ifstream fin;
    // Line buffer
    string line;
    // TAB position in the line to extract src and des IDs
    size_t tab_pos;
    // Src ID
    string src_str;
    long int src_int;
    // Des ID
    string des_str;
    long int des_int;
    // Edge list
    long int edge_list_idx;
    long int edge_num;
    edge_t *edge_list = new edge_t[max_edge_num];
    // Initial graph structure: randomly sample 50% of edges from basic graph
    ofstream fout;
    edge_t *loaded_edge_list = new edge_t[max_edge_num];
    edge_t *idle_edge_list = new edge_t[max_edge_num];
    long int loaded_edge_num;
    long int idle_edge_num;

    // Read the basic graph from file and store into the dynamic matrix
    fin.open(basic_graph_file);
    edge_list_idx = 0;
    if(!fin.is_open()) return 1;
    while (fin) 
    {
        getline(fin, line);
        if ((line[0] != '#') && (line.size() > 0))
        {
            tab_pos = line.find('\t', 0);
            src_str = line.substr(0, tab_pos);
            des_str = line.substr((tab_pos+1), line.size());
            // cout << "Source ID: " << src_str << "\tDest ID: " << des_str << "\n";
            src_int = stoi(src_str);
            des_int = stoi(des_str);
            // cout << "Source ID: " << src_int << "\tDest ID: "<< des_int << "\n";
            // cout << "Prev: " << p[src_int][des_int] << "\n";
            // p[src_int][des_int] = true;
            edge_list[edge_list_idx].src_id = src_int;
            edge_list[edge_list_idx].des_id = des_int;
            // cout << "Curr: " << p[src_int][des_int] << "\n";
            edge_list_idx++;
        }
    }
    fin.close();
    edge_num = edge_list_idx + 1;
    cout << "Read edges from file " << basic_graph_file << " : " << edge_num << "\n";

    // Write Initial edge_list to file
    fout.open(init_graph_file);
    if(!fout.is_open()) return 1;
    cout << "Writing the initial graph to file " << init_graph_file << "\n";
    initial_graph_generator(fout, edge_list, edge_num, loaded_edge_list, idle_edge_list);
    fout.close();
    // Free memory of the original graph
    delete [] edge_list;

    // Generate Stream of edges
    loaded_edge_num = edge_num / 2;
    idle_edge_num   = edge_num - loaded_edge_num;
    fout.open(stream_graph_file);
    if(!fout.is_open()) return 1;
    cout << "Generating randomized streaming batches : " << batch_num << "\n";
    for (size_t i = 0; i < batch_num; i++)
    {
        batch_generator(fout, batch_size, loaded_edge_list, idle_edge_list, loaded_edge_num, idle_edge_num);
        cout << "Batch No." << i+1 << " generated : " << batch_size << "\n";
    }
    fout.close();
    
    delete [] loaded_edge_list;
    delete [] idle_edge_list;
    return 0;
}