#include <fstream>
#include <iostream>
#include <string>
#include "../../core/common/parseCommandLine.h"

#define IN_PATH "../../apps/program.out"
#define INITIAL_PATH "time_analysis_initial.csv"
#define STREAM_PATH "time_analysis_stream.csv"

using namespace std;

int main (int argc, char *argv[])
{
    commandLine P(argc, argv);
    string in_path = P.getOptionValue("-inPath", IN_PATH);
    string initial_path = P.getOptionValue("-initPath", INITIAL_PATH);
    string stream_path = P.getOptionValue("-streamPath", STREAM_PATH);

    ifstream f_in;
    ofstream f_initial;
    ofstream f_stream;
    string line;

    f_in.open(in_path);
    if(!f_in.is_open()) return 1;

    f_initial.open(initial_path);
    if(!f_initial.is_open()) return 1;
    f_initial << "Number, Initial Time, \n";
    cout << "Initial File Created!\n";

    f_stream.open(stream_path);
    if(!f_stream.is_open()) return 1;
    f_stream << "Number,Batch Size,Deletion Time,Addition Time,Deletion Num,Addition Num,Update Time,Increment Time,\n";

    bool init_flag = false;
    double time;
    double addition_time;
    double deletion_time;
    int int_buffer;

    int initial_number = 1;
    int stream_number = 1;
    
    while (f_in)
    {
        getline(f_in, line);
        if (line.find("Initial graph processing") != string::npos)
        {
            cout << line << "\n";
            time = stod(line.substr(line.find(":")+2));
            f_initial << initial_number << "," << time << ",\n";
            initial_number++;
        }
        else if (line.find("Batch Size:") != string::npos)
        {
            cout << line << "\n";
            int_buffer = stoi(line.substr(line.find(":")+2));
            f_stream << stream_number << "," << int_buffer << ",";
        }
        else if (line.find("Edge deletion time") != string::npos)
        {
            deletion_time = stod(line.substr(line.find(":")+2));
            f_stream << deletion_time << ",";
        }
        else if (line.find("Edge addition time") != string::npos)
        {
            addition_time = stod(line.substr(line.find(":")+2));
            f_stream << addition_time << ",";
        }
        else if (line.find("Edge Additions in batch") != string::npos)
        {
            int_buffer = stoi(line.substr(line.find(":")+2));
            f_stream << int_buffer << ",";
        }
        else if (line.find("Edge Deletions in batch") != string::npos)
        {
            int_buffer = stoi(line.substr(line.find(":")+2));
            f_stream << int_buffer << "," << (addition_time + deletion_time) << ",";
        }
        else if (line.find("Finished batch") != string::npos)
        {
            time = stod(line.substr(line.find(":")+2));
            f_stream << time << ",\n";
            stream_number++;
        }
    }

    f_in.close();
    f_initial.close();
    f_stream.close();
    return 0;
}