#include <fstream>
#include <iostream>
#include <string>
#include "../../core/common/parseCommandLine.h"

#define IN_PATH "../../apps/program.out"
#define SNAPSHOT_PATH "../../apps/snapshot.out"
#define INITIAL_PATH "time_analysis_initial.csv"
#define STREAM_PATH "time_analysis_stream.csv"

using namespace std;

struct data_entry {
    int batch_size;
    double deletion_time;
    double addition_time;
    int deletion_num;
    int addition_num;
    double initial_time;
    double update_time;
    double increment_time;
    double snapshot_time;
};

int main (int argc, char *argv[])
{
    commandLine P(argc, argv);
    string in_path = P.getOptionValue("-inPath", IN_PATH);
    string snapshot_path = P.getOptionValue("-snapshotPath", SNAPSHOT_PATH);
    // string initial_path = P.getOptionValue("-initPath", INITIAL_PATH);
    string stream_path = P.getOptionValue("-streamPath", STREAM_PATH);

    ifstream f_in;
    // ofstream f_initial;
    ofstream f_stream;
    string line;

    f_in.open(in_path);
    if(!f_in.is_open()) return 1;

    // f_initial.open(initial_path);
    // if(!f_initial.is_open()) return 1;
    // f_initial << "Number, Initial Time, \n";
    // cout << "Initial File Created!\n";

    double time;
    int batch_size;
    double deletion_time;
    double addition_time;
    int deletion_num;
    int addition_num;
    double initial_time;
    double update_time;
    double increment_time;
    double snapshot_time;

    data_entry time_list [100];

    int batch_num = 0;

    while (f_in)
    {
        getline(f_in, line);
        if (line.find("Initial graph processing") != string::npos)
        {
            // cout << line << "\n";
            initial_time = stod(line.substr(line.find(":")+2));
        }
        else if (line.find("Batch Size:") != string::npos)
        {
            // cout << line << "\n";
            batch_size = stoi(line.substr(line.find(":")+2));
            // f_stream << batch_num << "," << int_buffer << ",";
        }
        else if (line.find("Edge deletion time") != string::npos)
        {
            deletion_time = stod(line.substr(line.find(":")+2));
            // f_stream << deletion_time << ",";
        }
        else if (line.find("Edge addition time") != string::npos)
        {
            addition_time = stod(line.substr(line.find(":")+2));
            // f_stream << addition_time << ",";
        }
        else if (line.find("Edge Additions in batch") != string::npos)
        {
            addition_num = stoi(line.substr(line.find(":")+2));
            // f_stream << int_buffer << ",";
        }
        else if (line.find("Edge Deletions in batch") != string::npos)
        {
            deletion_num = stoi(line.substr(line.find(":")+2));
            update_time = addition_time + deletion_time;
            // f_stream << int_buffer << "," << initial_time << "," << (addition_time + deletion_time) << ",";
        }
        else if (line.find("Finished batch") != string::npos)
        {
            increment_time = stod(line.substr(line.find(":")+2));
            // f_stream << time << ",\n";
            time_list[batch_num].batch_size = batch_size;
            time_list[batch_num].deletion_time = deletion_time;
            time_list[batch_num].addition_time = addition_time;
            time_list[batch_num].deletion_num = deletion_num;
            time_list[batch_num].addition_num = addition_num;
            time_list[batch_num].initial_time = initial_time;
            time_list[batch_num].update_time = update_time;
            time_list[batch_num].increment_time = increment_time;
            batch_num++;
        }
    }
    f_in.close();

    f_in.open(snapshot_path);
    if(!f_in.is_open()) return 1;

    batch_num = 0;
    while (f_in)
    {
        getline(f_in, line);
        if (line.find("Initial graph processing") != string::npos)
        {
            snapshot_time = stod(line.substr(line.find(":")+2));
            time_list[batch_num].snapshot_time = snapshot_time;
            batch_num++;
        }
    }
    f_in.close();


    f_stream.open(stream_path);
    if(!f_stream.is_open()) return 1;
    f_stream << "Number,Batch Size,Deletion Time,Addition Time,Deletion Num,Addition Num,Initial Time,Update Time,Increment Time,Snapshot Time\n";
    for (size_t i = 0; i < 100; i++)
    {
        f_stream << (i+1) << ",";
        f_stream << time_list[i].batch_size << ",";
        f_stream << time_list[i].deletion_time << ",";
        f_stream << time_list[i].addition_time << ",";
        f_stream << time_list[i].deletion_num << ",";
        f_stream << time_list[i].addition_num << ",";
        f_stream << time_list[i].initial_time << ",";
        f_stream << time_list[i].update_time << ",";
        f_stream << time_list[i].increment_time << ",";
        f_stream << time_list[i].snapshot_time << "\n";
    }

    f_stream.close();
    return 0;
}