# Enable this if the input graph has weights. Disable this if you want to use synthetically generated weights.
ifdef WEIGHTED
EDGEDATA = -DEDGEDATA
endif

# Enable this if number of vertices > 2^32 
ifdef LONGVERTEXCOUNT
INTV = -DLONG
endif

INTE = -DEDGELONG

# Compilers
PCC = g++-5

# Parallel Programming API
$(info ************  Using CILK ************)
PARFLAGS = -fcilkplus -lcilkrts -DCILK
$(info ************  Using OpenMP ************)
PARFLAGS = -fopenmp -DOPENMP

# Sniper path
SNIPER = -I/home/ztguan/sniper/include

# Flags
PCFLAGS = -std=c++14 -g -O3 $(INTV) $(INTE) $(EDGEDATA)

# mimalloc
$(info ************  Dynamic mimalloc, local machine only ************)
LDFLAGS = -L../lib/mimalloc/out/release -lmimalloc 
$(info ************  Static mimalloc, local machine or Sniper ************)
LDFLAGS = ../lib/mimalloc/out/release/libmimalloc.a

# dependencies
COMMON=../core/common/binary_search.h ../core/common/bitsetscheduler.h ../core/common/blockRadixSort.h ../core/common/densebitset.h ../core/common/gettime.h ../core/common/index_map.h ../core/common/matrix.h ../core/common/maybe.h ../core/common/parallel.h ../core/common/parseCommandLine.h ../core/common/transpose.h ../core/common/quickSort.h ../core/common/rwlock.h ../core/common/sequence.h ../core/common/utils.h

GRAPH= ../core/graph/graph.h ../core/graph/graphUtils.h ../core/graph/IO.h ../core/graph/vertex.h ../core/graph/vertexSubset.h

GRAPHBOLT=../core/graphBolt/AdaptiveExecutor.h ../core/graphBolt/GraphBoltEngine_complex.h ../core/graphBolt/GraphBoltEngine.h ../core/graphBolt/GraphBoltEngine_simple.h ../core/graphBolt/ingestor.h ../core/graphBolt/KickStarterEngine.h

OTHERS=../core/main.h

ALL=PageRank LabelPropagation CF COEM SSSP BFS

# Make

all: $(ALL)

# Define the Region-Of-Interest (ROI) for Sniper. Have no effect when running on local machine
# - *_INITIALCOMPUTE : ROI is the initial computation
# - *_DELTACOMPUTE: ROI is the incremental computation
# e.g. "make PageRank_INITIALCOMPUTE" produce an executable that mark the initial (before applying any batch) computation of PageRank

PageRank_% : PageRank.C $(COMMON) $(GRAPH) $(GRAPHBOLT) $(OTHERS)
	$(PCC) $(PCFLAGS) $(SNIPER) $(PARFLAGS) $(LDFLAGS) $< $(LDFLAGS) -D$* -o $@

CF_% : CF.C $(COMMON) $(GRAPH) $(GRAPHBOLT) $(OTHERS)
	$(PCC) $(PCFLAGS) $(SNIPER) $(PARFLAGS) $(LDFLAGS) $< $(LDFLAGS) -D$* -o $@

SSSP_%: SSSP.C $(COMMON) $(GRAPH) $(GRAPHBOLT) $(OTHERS)
	$(PCC) $(PCFLAGS) $(SNIPER) $(PARFLAGS) $(LDFLAGS) $< $(LDFLAGS) -D$* -o $@

BFS_%: BFS.C $(COMMON) $(GRAPH) $(GRAPHBOLT) $(OTHERS)
	$(PCC) $(PCFLAGS) $(SNIPER) $(PARFLAGS) $(LDFLAGS) $< $(LDFLAGS) -D$* -o $@

# Normal compilation. No ROI.
% : %.C $(COMMON) $(GRAPH) $(GRAPHBOLT) $(OTHERS)
	$(PCC) $(PCFLAGS) $(SNIPER) $(PARFLAGS) $(LDFLAGS) $< $(LDFLAGS) -o $@

.PHONY : clean

clean :
	rm -f *.o *.obj $(ALL)

