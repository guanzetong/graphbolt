echo "Starting Streaming Graph Generator ..."

BATCH_SIZE_ARRAY=(1000)
BATCH_NUM=100
BASIC_GRAPH="../../inputs/soc-pokec-relationships.txt"
INIT_GRAPH="../../inputs/20220624/init_graph.snap"
IDLE_GRAPH="../../inputs/20220624/idle_graph.snap"

INIT_GRAPH_ADJ="../../inputs/20220624/init_graph.adj"

for batch_size in ${BATCH_SIZE_ARRAY[@]}
do
    echo "Generating with batch size : $batch_size, Number of batches : $BATCH_NUM"

    STREAM_GRAPH="../../inputs/20220624/$batch_size/stream_graph.txt"
    SNAPSHOT_PATH="../../inputs/20220624/$batch_size/"
    if [ -f $INIT_GRAPH -a -f $IDLE_GRAPH ];
    then
        ./stream_generator -batchSize $batch_size -batchNum $BATCH_NUM \
        -initGraphFile $INIT_GRAPH -idleGraphFile $IDLE_GRAPH -streamGraphFile $STREAM_GRAPH \
        -snapshotPath $SNAPSHOT_PATH -readFile
    else
        ./stream_generator -batchSize $batch_size -batchNum $BATCH_NUM -basicGraphFile $BASIC_GRAPH \
        -initGraphFile $INIT_GRAPH -idleGraphFile $IDLE_GRAPH -streamGraphFile $STREAM_GRAPH \
        -snapshotPath $SNAPSHOT_PATH
    fi

    echo "Generated"
done

cd ../converters/
pwd
./SNAPtoAdjConverter $INIT_GRAPH $INIT_GRAPH_ADJ
for batch_size in ${BATCH_SIZE_ARRAY[@]}
do
    for batch_num in {1..100..1}
    do
        SNAP="../../inputs/20220624/$batch_size/snapshot_${batch_num}.snap"
        ADJ="../../inputs/20220624/$batch_size/snapshot_${batch_num}.adj"
        echo "Converting with batch size : $batch_size, batch number : $batch_num"
        ./SNAPtoAdjConverter $SNAP $ADJ
        echo "Converted"
    done
done

# cd ../converters/
# pwd
# batch_size=1000
# for batch_num in {1..100..1}
# do
#     SNAP="../../inputs/snapshot_pokec_1k/snapshot_${batch_size}_${batch_num}.snap"
#     ADJ="../../inputs/snapshot_pokec_1k/snapshot_${batch_size}_${batch_num}.adj"
#     echo "Converting with batch size : $batch_size, batch number : $batch_num"
#     ./SNAPtoAdjConverter $SNAP $ADJ
#     echo "Converted"
# done