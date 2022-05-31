
# INTV = -DLONG
INTE = -DEDGELONG

#compilers
PCC = g++-9
PCFLAGS = -std=c++14

# dependencies
STREAM_GEN = stream_generator

.PHONY: all clean

all: $(STREAM_GEN)

% : %.cpp
	$(PCC) $(PCFLAGS) -o $@ $<

clean :
	rm -f *.o $(STREAM_GEN)
