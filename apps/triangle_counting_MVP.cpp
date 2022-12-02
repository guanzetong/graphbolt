#include "../core/common/utils.h"
#include "../core/graph/IO.h"
#include "../core/graph/graph.h"
#include "../core/graph/vertex.h"
#include "../core/graphBolt/ingestor.h"

using namespace std;

unsigned long search_triangles(graph<asymmetricVertex> &G);

int main (int argc, char *argv[]) {
    // Initialization
    commandLine P(argc, argv);                                      //  Command line input arguments
    char *graph_struct_file = P.getArgument(0);                     //  Get the data graph file path
    graph<asymmetricVertex> G =
        readGraph<asymmetricVertex>(graph_struct_file, 0, 0, 0);     //  Read the data graph from file
    int *vertex_prop = newA(int, G.n);                              //  Allocate vertex property array

    // Do initial computation here
    unsigned long count = search_triangles(G);
    std::cout << "Num Matches " << count << '\n';

    Ingestor<asymmetricVertex> ingestor(G, P);                       //  Create an ingestor object for streaming
    ingestor.validateAndOpenFifo();                                 //  Open the streaming file
    while (ingestor.processNextBatch()) {
        edgeArray &edge_additions = ingestor.getEdgeAdditions();    //  Return added edges, useful for pruning computation
        edgeArray &edge_deletions = ingestor.getEdgeDeletions();    //  Return deleted edges, useful for pruning computation

        // Do delta-based computation or traditional computation here
        unsigned long count = search_triangles(G);
        std::cout << "Num Matches " << count << '\n';
    }

}

unsigned long search_triangles(graph<asymmetricVertex> &G) {
    unsigned long count = 0;

    for (int node1 = 0; node1 < G.n; ++node1) {
        const int node1_degree = G.V[node1].getOutDegree();
        for (int node1_neighbour = 0; node1_neighbour < node1_degree; ++node1_neighbour) {
            const int node2 = G.V[node1].getOutNeighbor(node1_neighbour);
            if (node2 < node1) {
                continue;
            }
            const int node2_degree = G.V[node2].getOutDegree();
            for (int node2_neighbour = 0; node2_neighbour < node2_degree; ++node2_neighbour) {
                const int node3 = G.V[node2].getOutNeighbor(node2_neighbour);
                if (node3 < node2) {
                    continue;
                }
                const int node3_degree = G.V[node3].getInDegree();
                for (int node3_neighbour = 0; node3_neighbour < node3_degree; ++node3_neighbour) {
                    if(G.V[node3].getInNeighbor(node3_neighbour) == node1) {
                        std::cout << "Match:" << node1 << "->" << node2 << "->" << node3 << '\n'; 
                        ++count;
                    }
                }
            }
        }
        
    }

    return count;
}
