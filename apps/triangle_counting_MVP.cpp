#ifndef TRIANGLE_COUNTING_MVP_C
#define TRIANGLE_COUNTING_MVP_C

#include "../core/common/utils.h"
#include "../core/graph/IO.h"
#include "../core/graph/graph.h"
#include "../core/graph/vertex.h"
#include "../core/graphBolt/ingestor.h"

using namespace std;

unsigned long search_triangles(graph<symmetricVertex> G);

int main (int argc, char *argv[]) {
    // Initialization
    commandLine P(argc, argv);
    char *graph_struct_file = P.getArgument(0);
    graph<symmetricVertex> G =
        readGraph<symmetricVertex>(graph_struct_file, 0, 0, 0);
    int *vertex_prop = newA(int, G.n);

    Ingestor<symmetricVertex> ingestor(G, P);

    

}

unsigned long search_triangles(graph<symmetricVertex> G) {
    unsigned long count = 0;

    for (int node1 = 0; node1 < G.n; ++node1) {
        const int node1_degree = G.V[node1].getInDegree();
        for (int node1_neighbour = 0; node1_neighbour < node1_degree; ++node1_neighbour) {
            const int node2 = G.V[node1].getInNeighbor(node1_neighbour);
            if (node2 > node1) {
                continue;
            }
            const int node2_degree = G.V[node2].getInDegree();
            for (int node2_neighbour = 0; node2_neighbour < node2_degree; ++node2_neighbour) {
                const int node3 = G.V[node2].getInNeighbor(node2_neighbour);
                if (node3 > node2) {
                    continue;
                }
                const int node3_degree = G.V[node3].getInDegree();
                for (int node3_neighbour = 0; node3_neighbour < node3_degree; ++node3_neighbour) {
                    if(G.V[node3].getInNeighbor(node3_neighbour) == node1) {
                        ++count;
                    }
                }
            }
        }
        
    }

    return count;
}

#endif
