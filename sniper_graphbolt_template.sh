# ================================================================
# sniper_graphbolt_template.sh
# This shell script is for running GraphBolt on Sniper Simulator.
# Must be placed inside the Sniper Simulator directory.
# ================================================================

# Set the number of threads
nWorkers=64

# Set the number of logical threads on each core
nSMT=2

# Set Batch Size
nEdges=1000

# Set the ROI
section=INITIALCOMPUTE
# section=EDGEADDITION
# section=EDGEDELETION
# section=DELTACOMPUTE

# Set the path to Sniper Simulator outputs
outputDir="LiveJournal_${nWorkers}_${nSMT}_${nEdges}_${section}/"

# Set the path to streaming graph dataset
graphPath="../../../data2/ztguan/graphbolt/LiveJournal"

streamPath="${graphPath}/${nEdges}/stream_graph.txt"
initPath="${graphPath}/init_graph.adj"


# Calculate the number of cores
nCores=`expr $nWorkers / $nSMT`

# Invoke Sniper Simulator
./run-sniper -n $nCores -d $outputDir \
-g perf_model/core/logical_cpus=$nSMT \
--roi --no-cache-warming --viz -- \
/home/ztguan/graphbolt/apps/PageRank_${section} \
-nWorkers $nWorkers -numberOfUpdateBatches 1 \
-nEdges $nEdges -maxIters 1000 \
-streamPath $streamPath $initPath