#!/bin/bash
for i in {0..51}
do
   echo "Welcome $i times"
   cp HH_all_nodes.txt HH_all_nodes_$i.txt
done
