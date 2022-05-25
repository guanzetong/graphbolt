
# INTV = -DLONG
INTE = -DEDGELONG

#compilers
PCC = g++-5
PCFLAGS = -std=c++14

# dependencies
INIT_GEN = initial_graph_generator

.PHONY: all clean

all: $(INIT_GEN)

% : %.C $(COMMON) $(LOCAL_COMMON)
	$(PCC) $(PCFLAGS) -o $@ $<

clean :
	rm -f *.o $(INIT_GEN)
