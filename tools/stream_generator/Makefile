
# INTV = -DLONG
INTE = -DEDGELONG

#compilers
PCC = g++-9
PCFLAGS = -std=c++14
OMP = -fopenmp

# dependencies
STREAM_GEN = stream_generator

.PHONY: all clean

all: $(STREAM_GEN)

partition_gen: partition_gen.cpp
	$(PCC) $(PCFLAGS) $(OMP) -o $@ $<

seeds_gen: seeds_gen.cpp
	$(PCC) $(PCFLAGS) $(OMP) -o $@ $<

% : %.cpp
	$(PCC) $(PCFLAGS) -o $@ $<

clean :
	rm -f *.o $(STREAM_GEN)
