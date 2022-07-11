MAX_EDGE_NUM=1468364884
BATCH_SIZE=1000
BATCH_NUM=10
BASIC_GRAPH_FILE="../../inputs/twitter-2010.txt"
INIT_GRAPH_FILE="../../inputs/20220701/init_graph.snap"
IDLE_GRAPH_FILE="../../inputs/20220701/idle_graph.snap"
STREAM_GRAPH_FILE="../../inputs/20220701/${BATCH_SIZE}/stream_graph.txt"
SNAPSHOT_PATH="../../inputs/20220701/${BATCH_SIZE}/"
INIT_GRAPH_FILE_ADJ="../../inputs/20220701/init_graph.adj"

# ./stream_generator -maxEdgeNum ${MAX_EDGE_NUM} -batchSize ${BATCH_SIZE} \
# -batchNum ${BATCH_NUM} -basicGraphFile ${BASIC_GRAPH_FILE} \
# -initGraphFile ${INIT_GRAPH_FILE} -idleGraphFile ${IDLE_GRAPH_FILE} \
# -streamGraphFile ${STREAM_GRAPH_FILE} -snapshotPath ${SNAPSHOT_PATH}

cd ../converters/
pwd
./SNAPtoAdjConverter ${INIT_GRAPH_FILE} ${INIT_GRAPH_FILE_ADJ}
for batch_num in {1..10..1}
do
    SNAP="../../inputs/20220701/${BATCH_SIZE}/snapshot_${batch_num}.snap"
    ADJ="../../inputs/20220701/${BATCH_SIZE}/snapshot_${batch_num}.adj"
    echo "Converting with batch size : ${BATCH_SIZE}, batch number : $batch_num"
    ./SNAPtoAdjConverter $SNAP $ADJ
    echo "Converted"
done