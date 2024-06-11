#!/bin/bash

task_counts=(1 2 4 8 16 32 64 128 256)

for task_count in "${task_counts[@]}"; do
  nodes=1
  if [ $task_count -gt 128 ]; then
    task_count=128
    nodes=2
  fi
  echo "Running with nodes: $nodes and tasks: $task_count"
  sbatch --ntasks-per-node $task_count --cpus-per-task 2 --nodes $nodes ./batch_rust.sh $node_count $task_count
  sbatch --ntasks-per-node $task_count --cpus-per-task 2 --nodes $nodes ./batch_c.sh $node_count $task_count
done

