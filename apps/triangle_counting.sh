BATCH_SIZE_ARRAY=(1000 10000 100000 1000000)
NUM_THREADS=64
DATASET_PATH="../../../../data2/ztguan/graphbolt/LiveJournal"

rm result.out

for batch_size in ${BATCH_SIZE_ARRAY[@]}
do
    ./triangle_counting_MVP.exe -nWorkers ${NUM_THREADS} -streamPath ${DATASET_PATH}/${batch_size}/stream_graph.txt -nEdges ${batch_size} -numberOfUpdateBatches 10 ${DATASET_PATH}/init_graph.adj >> result.out
done