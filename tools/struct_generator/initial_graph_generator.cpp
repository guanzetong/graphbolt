#include <iostream>
#include <fstream>
#include <string>
#include <stdlib.h>
#include <bits/stdc++.h>

#define BASIC_GRAPH_FILE "p2p-Gnutella08.snap"
#define MAX_VERTEX_NUM  6301
#define MAX_EDGE_NUM 100000

#define INIT_GRAPH_FILE "init_graph.snap"

#define STREAM_GRAPH_FILE "stream_graph.txt"
#define BATCH_SIZE 1000
#define BATCH_NUM 10

using namespace std;

struct edge_t {
    int src_id;
    int des_id;
};

void shuffle_edge_list (edge_t *edge_list, int edge_num)
{
    unsigned seed = 0;
    shuffle(edge_list, edge_list+edge_num, default_random_engine(seed));
    // for (size_t i = 0; i < edge_num; i++)
    // {
    //     cout << "src: " << edge_list[i].src_id << "\tdes: " << edge_list[i].des_id << "\n";
    // }
}

void mutate_swap (edge_t *edge_list, int loaded_edge_num, int batch_size)
{
    
    edge_t *temp = new edge_t[batch_size/2];
    memcpy((void *)temp, (void *)edge_list, (batch_size/2)*sizeof(edge_t));
    memcpy((void *)edge_list, (void *)(edge_list+loaded_edge_num), (batch_size/2)*sizeof(edge_t));
    memcpy((void *)(edge_list+loaded_edge_num), (void *)temp, (batch_size/2)*sizeof(edge_t));
    delete [] temp;
}

int main () 
{
    // Basic Graph input file in SNAP format
    ifstream fin;
    // Line buffer
    string line;
    // TAB position in the line to extract src and des IDs
    size_t tab_pos;
    // Src ID
    string src_str;
    int src_int;
    // Des ID
    string des_str;
    int des_int;
    // Edge list
    int edge_list_idx;
    int edge_num;
    edge_t *edge_list = new edge_t[MAX_EDGE_NUM];
    // Initial graph structure: randomly sample 50% of edges from basic graph
    ofstream fout;

    // Read the basic graph from file and store into the dynamic matrix
    fin.open(BASIC_GRAPH_FILE);
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

    // Shuffle the edge_list
    shuffle_edge_list(edge_list, edge_num);

    // Write Initial edge_list to file
    int loaded_edge_num = edge_num / 2;
    fout.open(INIT_GRAPH_FILE);
    if(!fout.is_open()) return 1;
    for (size_t i = 0; i < loaded_edge_num; i++)
    {
        fout << edge_list[i].src_id << "\t" << edge_list[i].des_id << "\n";
    }
    fout.close();

    // Generate Stream of edges
    fout.open(STREAM_GRAPH_FILE);
    if(!fout.is_open()) return 1;
    for (size_t i = 0; i < BATCH_NUM; i++)
    {
        shuffle_edge_list(edge_list, loaded_edge_num);
        shuffle_edge_list(edge_list+loaded_edge_num, edge_num-loaded_edge_num);
        mutate_swap(edge_list, loaded_edge_num, BATCH_SIZE);
        for (size_t j = 0; j < BATCH_SIZE/2; j++)
        {
            fout << "a\t" << edge_list[j].src_id << "\t" << edge_list[j].des_id << "\n";
        }

        for (size_t j = 0; j < BATCH_SIZE/2; j++)
        {
            fout << "d\t" << edge_list[j+loaded_edge_num].src_id << "\t" << edge_list[j+loaded_edge_num].des_id << "\n";
        }
    }
    fout.close();
    
    delete [] edge_list;
    return 0;
}