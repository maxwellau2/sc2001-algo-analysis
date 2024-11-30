#include <iostream>
#include <vector>
#include <unordered_map>
#include <climits>
#include <cstdlib>
#include <ctime>
#include <chrono>
#include <fstream>
#include <thread>
#include <mutex>
#include <algorithm>
#include <set>
#include <utility> // for std::pair

using namespace std;

// Struct to hold the return values of Dijkstra's algorithm
struct DijkstraReturn {
    vector<int> distances;  // Shortest distances from the start node
    vector<int> pi;         // Predecessor array to track paths
    uint64_t key_comps;     // Number of key comparisons performed
};

// Mutex to prevent simultaneous file access from multiple threads
mutex fileMutex;

// Custom priority queue using a min-heap implemented with a vector
class MinHeap {
public:
    vector<pair<int, int>> heap; // Each element is a (node, distance) pair
    uint64_t key_comps = 0;      // Counter for key comparisons during heap operations

    // Insert a new node into the heap
    void push(pair<int, int> node) {
        heap.push_back(node);
        siftUp(heap.size() - 1);
    }

    // Get the node with the minimum distance (top of the heap)
    pair<int, int> top() {
        return heap.front();
    }

    // Remove the node with the minimum distance and fix the heap
    void pop() {
        swap(heap.front(), heap.back());
        heap.pop_back();
        siftDown(0);
    }

    // Check if the heap is empty
    bool empty() const {
        return heap.empty();
    }

private:
    // Move the node at index up to its correct position in the heap
    void siftUp(int idx) {
        while (idx > 0) {
            int parentIdx = (idx - 1) / 2;
            key_comps++; // Counting key comparison
            if (heap[idx].second < heap[parentIdx].second) {
                swap(heap[idx], heap[parentIdx]);
                idx = parentIdx;
            } else {
                break;
            }
        }
    }

    // Move the node at index down to its correct position in the heap
    void siftDown(int idx) {
        int leftChildIdx = 2 * idx + 1;
        int rightChildIdx = 2 * idx + 2;
        int smallestIdx = idx;

        if (leftChildIdx < heap.size()) {
            key_comps++; // Counting key comparison
            if (heap[leftChildIdx].second < heap[smallestIdx].second) {
                smallestIdx = leftChildIdx;
            }
        }

        if (rightChildIdx < heap.size()) {
            key_comps++; // Counting key comparison
            if (heap[rightChildIdx].second < heap[smallestIdx].second) {
                smallestIdx = rightChildIdx;
            }
        }

        if (smallestIdx != idx) {
            swap(heap[idx], heap[smallestIdx]);
            siftDown(smallestIdx); // Recursive call to continue fixing the heap
        }
    }
};

// Modified Dijkstra's algorithm using a vector of pairs for the adjacency list
DijkstraReturn dijkstra(const vector<vector<pair<int, int>>>& adjList, int startNode) {
    int n = adjList.size();
    vector<int> distances(n, INT_MAX); // Shortest path estimates
    vector<int> pi(n, -1);             // Predecessors of each vertex
    vector<bool> visited(n, false);    // Indicates if a node is processed
    MinHeap minHeap;                   // Custom min-heap to hold (node, distance) pairs

    distances[startNode] = 0;
    minHeap.push({startNode, 0}); // Insert the start node into the heap

    while (!minHeap.empty()) {
        int u = minHeap.top().first; // Get the node with the smallest distance
        minHeap.pop();
        visited[u] = true; // Mark the node as processed

        for (const auto& neighbor : adjList[u]) {
            int v = neighbor.first;
            int weight = neighbor.second;
            if (!visited[v]) { // Check for connected unvisited nodes
                minHeap.key_comps++; // Count key comparison for distance check
                int newDistance = distances[u] + weight;
                if (newDistance < distances[v]) { // If a shorter path is found, update the distance
                    minHeap.key_comps++; // Count key comparison for update check
                    distances[v] = newDistance;
                    pi[v] = u; // Update the predecessor
                    minHeap.push({v, newDistance}); // Push the updated distance into the heap
                }
            }
        }
    }

    return {distances, pi, minHeap.key_comps}; // Return distances, predecessors, and key comparisons
}
// Function to test graphs of increasing size and record results into a CSV file
void testIncreasingGraphSizes() {
    ofstream file;
    file.open("graph_size_results.csv", std::ios::app);
    file << "Number of Nodes,Average Key Comparisons,Average CPU Time (us)\n";
    file.close();

    const int iterations = 5;  // Number of iterations to average out anomalies

    for (int num_nodes = 500; num_nodes <= 10000; num_nodes += 100) {
        uint64_t total_key_comps = 0;
        uint64_t total_duration = 0;

        for (int iter = 0; iter < iterations; ++iter) {
            vector<vector<pair<int, int>>> adjList(num_nodes); // Adjacency list representation

            // Generate a random connected graph with a spanning tree
            for (int i = 0; i < num_nodes - 1; ++i) {
                int weight = 10;  // Use a fixed weight to reduce randomness
                adjList[i].emplace_back(i + 1, weight);  // Add directed edge
            }

            int startNode = 0;  // Fixed start node to ensure consistency across iterations

            auto start = chrono::high_resolution_clock::now();
            DijkstraReturn result = dijkstra(adjList, startNode);
            auto end = chrono::high_resolution_clock::now();
            auto duration = chrono::duration_cast<chrono::microseconds>(end - start).count();

            total_key_comps += result.key_comps;
            total_duration += duration;
        }

        // Calculate the average key comparisons and CPU time
        uint64_t average_key_comps = total_key_comps / iterations;
        uint64_t average_duration = total_duration / iterations;

        // Log the results to the file
        lock_guard<mutex> lock(fileMutex); // Lock the mutex for thread-safe file writing
        file.open("graph_size_results.csv", std::ios::app);
        file << num_nodes << "," << average_key_comps << "," << average_duration << "\n";
        file.close();
    }

    cout << "Graph size test results saved to graph_size_results.csv\n";
}


// Function to test graphs of specific sizes with increasing density and record results into a CSV file
void testGraphsWithSpecificSizesAndDensity() {
    ofstream file;
    file.open("graph_density_results.csv", std::ios::app);
    file << "Number of Nodes,Density (%),Average Key Comparisons,Average CPU Time (us),Edges\n";
    file.close();

    vector<int> specific_sizes = {500, 1000, 10000};
    const int iterations = 5; // Number of iterations to average out anomalies

    for (int num_nodes : specific_sizes) {
        int max_possible_edges = num_nodes * (num_nodes - 1);  // Maximum number of edges for a directed graph

        // Generate graphs with increasing density
        for (int interval = 1; interval <= 20; ++interval) {
            int target_edge_count = (interval * max_possible_edges) / 20;
            uint64_t total_key_comps = 0;
            uint64_t total_duration = 0;

            for (int iter = 0; iter < iterations; ++iter) {
                vector<vector<pair<int, int>>> adjList(num_nodes); // Adjacency list representation
                set<pair<int, int>> edgeSet;

                // Ensure the graph is connected by creating a spanning tree
                for (int i = 0; i < num_nodes - 1; ++i) {
                    int weight = 1;  // Use a fixed weight to reduce randomness
                    adjList[i].emplace_back(i + 1, weight);
                    edgeSet.insert({i, i + 1});  // Track the edge to avoid duplication
                }

                // Add additional edges to achieve the desired edge count
                int added_edges = num_nodes - 1;  // Already added by the spanning tree
                for (int i = 0; i < num_nodes && added_edges < target_edge_count; ++i) {
                    for (int j = (i + 2) % num_nodes; j != i && added_edges < target_edge_count; j = (j + 1) % num_nodes) {
                        if (edgeSet.find({i, j}) == edgeSet.end()) {
                            int fixed_weight = 1;  // Use a fixed weight for all additional edges
                            adjList[i].emplace_back(j, fixed_weight);
                            edgeSet.insert({i, j});  // Add the edge to the set to mark it as existing
                            added_edges++;
                        }
                    }
                }

                // Run Dijkstra's algorithm
                int startNode = 0;  // Start from a fixed node to ensure consistent paths
                auto start = chrono::high_resolution_clock::now();
                DijkstraReturn result = dijkstra(adjList, startNode);
                auto end = chrono::high_resolution_clock::now();
                auto duration = chrono::duration_cast<chrono::microseconds>(end - start).count();

                total_key_comps += result.key_comps;
                total_duration += duration;
            }

            // Calculate the average key comparisons and CPU time
            uint64_t average_key_comps = total_key_comps / iterations;
            uint64_t average_duration = total_duration / iterations;

            // Log the results with edge count
            lock_guard<mutex> lock(fileMutex); // Lock the mutex for thread-safe file writing
            file.open("graph_density_results.csv", std::ios::app);
            file << num_nodes << "," << (interval * 5) << "," << average_key_comps << "," 
                 << average_duration << "," << target_edge_count << "\n";
            file.close();
        }
    }
}



int main() {
    srand(static_cast<unsigned int>(time(0)));  // Initialize random seed

    // Create threads for the two tests
    thread sizeTestThread(testIncreasingGraphSizes);
    thread densityTestThread(testGraphsWithSpecificSizesAndDensity);

    // Wait for both threads to complete
    sizeTestThread.join();
    densityTestThread.join();
    // testGraphsWithSpecificSizesAndDensity();

    cout << "All tests completed and results saved to CSV files.\n";

    return 0;
}
