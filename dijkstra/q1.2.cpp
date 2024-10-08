#include <iostream>
#include <vector>
#include <climits>
#include <cstdlib>
#include <ctime>
#include <chrono>
#include <fstream>
#include <thread>
#include <mutex>

using namespace std;

// Struct to hold the return values of Dijkstra's algorithm
struct DijkstraReturn {
    vector<int> distances;  // Shortest distances from the start node (equivalent to 'd' in lecture notes)
    vector<int> pi;         // Predecessor array to track paths (equivalent to 'pi' in lecture notes)
    uint64_t key_comps;     // Number of key comparisons performed
};

// Mutex to prevent simultaneous file access from multiple threads
mutex fileMutex;

// Function to extract the node with the minimum distance from the priority queue
int extractCheapest(vector<pair<int, int>>& priorityQueue, uint64_t& key_comps) {
    int minIndex = 0;
    for (int i = 1; i < priorityQueue.size(); ++i) {
        key_comps++; // Increment key comparisons
        if (priorityQueue[i].second < priorityQueue[minIndex].second) {
            minIndex = i;
        }
    }

    int minNode = priorityQueue[minIndex].first;
    priorityQueue.erase(priorityQueue.begin() + minIndex); // Remove the extracted node
    return minNode;
}

// Modified Dijkstra's algorithm to track key comparisons and predecessor array using a vector-based priority queue
DijkstraReturn dijkstra(const vector<vector<int>>& graph, int startNode) {
    int n = graph.size();
    vector<int> distances(n, INT_MAX); // 'd' array in lecture notes: shortest path estimates
    vector<int> pi(n, -1);             // 'pi' array in lecture notes: predecessors of each vertex
    vector<bool> visited(n, false);    // 'S' in lecture notes: indicates if a node is processed
    vector<pair<int, int>> priorityQueue; // Vector-based priority queue to hold (node, distance) pairs
    uint64_t key_comps = 0;            // Counter for key comparisons

    distances[startNode] = 0; // Distance from the source node to itself is always 0

    // Initialize the priority queue with all nodes
    for (int i = 0; i < n; ++i) {
        priorityQueue.push_back({i, distances[i]});
    }

    while (!priorityQueue.empty()) {
        int u = extractCheapest(priorityQueue, key_comps); // Get the node with the smallest distance
        visited[u] = true; // Mark the node as processed (add to set 'S')

        // Update distances for neighboring nodes
        for (int v = 0; v < n; ++v) {
            if (graph[u][v] != 0 && !visited[v]) { // Check for connected unvisited nodes
                key_comps++;
                int newDistance = distances[u] + graph[u][v];
                if (newDistance < distances[v]) { // If a shorter path is found, update the distance
                    key_comps ++;
                    distances[v] = newDistance;   // Update the 'd' array
                    pi[v] = u;                   // Update the 'pi' array with predecessor
                }
            }
        }
    }

    return {distances, pi, key_comps};  // Return distances, predecessors, and key comparisons
}

// Function to test graphs of increasing size and record results into a CSV file
void testIncreasingGraphSizes() {
    ofstream file("graph_size_results.csv");
    file << "Number of Nodes,Key Comparisons,CPU Time (us)\n";

    for (int num_nodes = 500; num_nodes <= 10000; num_nodes += 100) {
        vector<vector<int>> graph(num_nodes, vector<int>(num_nodes, 0));

        // Generate a random connected graph with a spanning tree
        for (int i = 0; i < num_nodes - 1; ++i) {
            int weight = rand() % 20 + 1;  // Random weight between 1 and 20
            graph[i][i + 1] = weight;
            // graph[i + 1][i] = weight;      // Undirected graph
        }

        int startNode = rand() % num_nodes;
        auto start = chrono::high_resolution_clock::now();
        DijkstraReturn result = dijkstra(graph, startNode);
        auto end = chrono::high_resolution_clock::now();
        auto duration = chrono::duration_cast<chrono::nanoseconds>(end - start).count();

        lock_guard<mutex> lock(fileMutex); // Lock the mutex for thread-safe file writing
        file << num_nodes << "," << result.key_comps << "," << duration << "\n";
    }

    file.close();
    cout << "Graph size test results saved to graph_size_results.csv\n";
}

// Function to test graphs of specific sizes with increasing density and record results into a CSV file
void testGraphsWithSpecificSizesAndDensity() {
    ofstream file("graph_density_results.csv");
    file << "Number of Nodes,Density (%),Key Comparisons,CPU Time (us)\n";
    vector<int> specific_sizes = {500, 1000, 10000};

    for (int num_nodes : specific_sizes) {
        int max_possible_edges = num_nodes * (num_nodes - 1) / 2;  // Maximum number of edges for full connectivity

        for (int interval = 1; interval <= 20; ++interval) {
            int current_edge_count = (interval * max_possible_edges) / 20;
            vector<vector<int>> graph(num_nodes, vector<int>(num_nodes, 0));

            // Ensure the graph is connected by creating a spanning tree
            for (int i = 0; i < num_nodes - 1; ++i) {
                int weight = rand() % 20 + 1;  // Random weight between 1 and 20
                graph[i][i + 1] = weight;
                // graph[i + 1][i] = weight;      // Undirected graph
            }

            // Add additional random edges to achieve the desired edge count
            int added_edges = num_nodes - 1; // Already added by the spanning tree
            while (added_edges < current_edge_count) {
                int node1 = rand() % num_nodes;
                int node2 = rand() % num_nodes;
                while (node1 == node2 || graph[node1][node2] != 0) {
                    node1 = rand() % num_nodes;
                    node2 = rand() % num_nodes;
                }
                int weight = rand() % 20 + 1;
                graph[node1][node2] = weight;
                // graph[node2][node1] = weight;  // Undirected graph
                added_edges++;
            }

            int startNode = rand() % num_nodes;
            auto start = chrono::high_resolution_clock::now();
            DijkstraReturn result = dijkstra(graph, startNode);
            auto end = chrono::high_resolution_clock::now();
            auto duration = chrono::duration_cast<chrono::nanoseconds>(end - start).count();

            lock_guard<mutex> lock(fileMutex); // Lock the mutex for thread-safe file writing
            file << num_nodes << "," << (interval * 10) << "," << result.key_comps << "," << duration << "\n";
        }
    }

    file.close();
    cout << "Graph density test results saved to graph_density_results.csv\n";
}

int main() {
    srand(static_cast<unsigned int>(time(0)));  // Initialize random seed

    // Create threads for the two tests
    thread sizeTestThread(testIncreasingGraphSizes);
    thread densityTestThread(testGraphsWithSpecificSizesAndDensity);

    // Wait for both threads to complete
    sizeTestThread.join();
    densityTestThread.join();

    cout << "All tests completed and results saved to CSV files.\n";

    return 0;
}
