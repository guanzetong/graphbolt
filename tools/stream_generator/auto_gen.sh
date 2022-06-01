echo "Starting Streaming Graph Generator ..."

BATCH_SIZE_ARRAY=(10 50 100 500 1000 5000 10000 50000 100000 500000 1000000)
BATCH_NUM=100
BASIC_GRAPH="../../inputs/soc-pokec-relationships.txt"

# for batch_size in ${BATCH_SIZE_ARRAY[@]}
# do
#     INIT_GRAPH="../../inputs/init_graph_$batch_size.snap"
#     STREAM_GRAPH="../../inputs/stream_graph_$batch_size.txt"
#     echo "Generating with batch size : $batch_size, batch number : $BATCH_NUM"
#     ./stream_generator -batchSize $batch_size -batchNum $BATCH_NUM -basicGraphFile $BASIC_GRAPH -initGraphFile $INIT_GRAPH -streamGraphFile $STREAM_GRAPH
#     echo "Generated"
# done

cd ../converters/
pwd
for batch_size in ${BATCH_SIZE_ARRAY[@]}
do
    SNAP="../../inputs/init_graph_$batch_size.snap"
    ADJ="../../inputs/init_graph_$batch_size.adj"
    echo "Converting with batch size : $batch_size, batch number : $BATCH_NUM"
    ./SNAPtoAdjConverter $SNAP $ADJ
    echo "Converted"
done