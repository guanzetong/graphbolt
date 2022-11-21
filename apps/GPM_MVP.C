#ifndef GPM_MVP
#define GPM_MVP

#include "../core/common/utils.h"
#include "../core/graph/IO.h"
#include "../core/graph/graph.h"
#include "../core/graph/vertex.h"
using namespace std;

int main (int argc, char *argv[]) {
    // Initialization
    commandLine P(argc, argv);
    char *graph_struct_file = P.getArgument(0);
    graph<asymmetricVertex> G =
        readGraph<asymmetricVertex>(graph_struct_file, 0, 0, 0);
    int *vertex_prop = newA(int, G.n);

    // Vertex property
    // 0: △, 1: ○, 2: □
    vertex_prop[0] = 1;
    vertex_prop[1] = 0;
    vertex_prop[2] = 2;
    vertex_prop[3] = 0;
    vertex_prop[4] = 1;

    // Result vector
    vector<vector<int>> result;

    // Pattern Mining
    // △ <- ○ -> □

    // Iterate over all the vertices and look for △
    for (int triangle_id = 0; triangle_id < G.n; triangle_id++) {
        cout << "1st loop : " << triangle_id << " : " << vertex_prop[triangle_id] << '\n';
        if (vertex_prop[triangle_id] == 0) {
            int in_degree = G.V[triangle_id].getInDegree();
            // cout << "in degree " << in_degree << '\n';
            // Iterate over all the in-neighbors of a △ and look for ○
            for (int j = 0; j < in_degree; j++) {
                int circle_id = G.V[triangle_id].getInNeighbor(j);
                cout << "2nd loop : " << circle_id << " : " << vertex_prop[circle_id] << "\n";
                if (vertex_prop[circle_id] == 1) {
                    int out_degree = G.V[circle_id].getOutDegree();
                    // cout << "out degree " << out_degree << '\n';
                    // Iterate over all the out-neighbors of a ○ and look for □
                    for (int k = 0; k < out_degree; k++) {
                        int square_id = G.V[circle_id].getOutNeighbor(k);
                        cout << "3rd loop : " << square_id << " : " << vertex_prop[square_id] << "\n";
                        if (vertex_prop[square_id] == 2) {
                            vector<int> temp;
                            temp.push_back(triangle_id);
                            temp.push_back(circle_id);
                            temp.push_back(square_id);
                            result.push_back(temp);
                        }
                    }
                }
            }
        }
    }

    // Print out results
    cout << '\n' << "Patterns found: \n";
    for (int i = 0; i < result.size(); i++) {
        cout << "#" << i+1 << " : " << result[i][0] 
        << " <- " << result[i][1] << " -> " << result[i][2] << '\n';        
    }

    return 0;
}

#endif