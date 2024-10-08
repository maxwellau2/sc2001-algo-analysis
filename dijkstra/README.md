# Please note the following:

-   According to the lecture notes, Dijkstra's Algorithm is an algorithm to find the shortest paths from a single source vertex to all other vertices in a weighted, directed graph. All weights must be nonnegative.

-   The experiment in the lab is poorly designed. In order to fairly assess the performance of adj list vs adj mtx, we first need to identify which PQ system is better, array or min heap.

-   Metholodology for BOTH adj list and adj mtx:

1. Generate a random graph with a spanning tree.
2. Run 1 test with PQ being array, record results in csv
3. Run another test with PQ being min heap, record results in csv
4. compare the performance gains of the two PQ implementations

-   Only then can we fairly assess which data structure is better using the density experiment.
