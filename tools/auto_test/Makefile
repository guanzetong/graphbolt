
# INTV = -DLONG
INTE = -DEDGELONG

#compilers
PCC = g++-9
PCFLAGS = -std=c++14

# dependencies
TIME_ANALYSIS = time_analysis

.PHONY: all clean

all: $(TIME_ANALYSIS)

% : %.cpp
	$(PCC) $(PCFLAGS) -o $@ $<

clean :
	rm -f *.o $(TIME_ANALYSIS)
