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
    ofstream file("graph_size_results.csv");
    file << "Number of Nodes,Key Comparisons,CPU Time (us)\n";

    for (int num_nodes = 500; num_nodes <= 10000; num_nodes += 100) {
        vector<vector<pair<int, int>>> adjList(num_nodes); // Changed to vector<pair<int, int>>

        // Generate a random connected graph with a spanning tree
        for (int i = 0; i < num_nodes - 1; ++i) {
            int weight = rand() % 20 + 1;  // Random weight between 1 and 20
            adjList[i].emplace_back(i + 1, weight); // Use emplace_back to add edges
            // adjList[i + 1].emplace_back(i, weight); // Undirected graph
        }

        int startNode = rand() % num_nodes;
        auto start = chrono::high_resolution_clock::now();
        DijkstraReturn result = dijkstra(adjList, startNode);
        auto end = chrono::high_resolution_clock::now();
        auto duration = chrono::duration_cast<chrono::nanoseconds>(end - start).count();

        lock_guard<mutex> lock(fileMutex); // Lock the mutex for thread-safe file writing
        file << num_nodes << "," << result.key_comps << "," << duration << "\n";
    }

    file.close();
    cout << "Graph size test results saved to graph_size_results.csv\n";
}

// Function to test graphs of specific sizes with increasing density and record results into a CSV file
// Function to test graphs of specific sizes with increasing density and record results into a CSV file
void testGraphsWithSpecificSizesAndDensity() {
    ofstream file;
    file.open("graph_density_results.csv", std::ios::app);
    file << "Number of Nodes,Density (%),Key Comparisons,CPU Time (us)\n";
    file.close();
    vector<int> specific_sizes = {500, 1000, 10000};

    for (int num_nodes : specific_sizes) {
        int max_possible_edges = num_nodes * (num_nodes - 1) / 2;  // Maximum number of edges for full connectivity

        // Generate graphs with increasing density
        for (int interval = 1; interval <= 20; ++interval) {
            int current_edge_count = (interval * max_possible_edges) / 20;
            vector<vector<pair<int, int>>> adjList(num_nodes); // Adjacency list representation

            // Ensure the graph is connected by creating a spanning tree
            for (int i = 0; i < num_nodes - 1; ++i) {
                int weight = rand() % 20 + 1;  // Random weight between 1 and 20
                adjList[i].emplace_back(i + 1, weight);
                // adjList[i + 1].emplace_back(i, weight); // Undirected graph
            }

            // Add additional random edges to achieve the desired edge count
            int added_edges = num_nodes - 1; // Already added by the spanning tree
            while (added_edges < current_edge_count) {
                int node1 = rand() % num_nodes;
                int node2 = rand() % num_nodes;

                // Check for existing edge or self-loop
                while (node1 == node2 || 
                       any_of(adjList[node1].begin(), adjList[node1].end(), [&](const auto& p) { return p.first == node2; })) {
                    node1 = rand() % num_nodes;
                    node2 = rand() % num_nodes;
                }

                int weight = rand() % 20 + 1;
                adjList[node1].emplace_back(node2, weight);
                // adjList[node2].emplace_back(node1, weight); // Undirected graph
                added_edges++;
            }

            // Run Dijkstra's algorithm
            int startNode = rand() % num_nodes;
            auto start = chrono::high_resolution_clock::now();
            cout << "doing shitt" << endl;
            DijkstraReturn result = dijkstra(adjList, startNode);
            auto end = chrono::high_resolution_clock::now();
            auto duration = chrono::duration_cast<chrono::nanoseconds>(end - start).count();

            // Log the results
            lock_guard<mutex> lock(fileMutex); // Lock the mutex for thread-safe file writing
            file.open("graph_density_results.csv", std::ios::app);
            file << num_nodes << "," << (interval * 5) << "," << result.key_comps << "," << duration << "\n";
            file.close();
        }
    }

    file.close();
    cout << "Graph density test results saved to graph_density_results.csv\n";
}


int main() {
    srand(static_cast<unsigned int>(time(0)));  // Initialize random seed

    // Create threads for the two tests
    // thread sizeTestThread(testIncreasingGraphSizes);
    // thread densityTestThread(testGraphsWithSpecificSizesAndDensity);

    // // Wait for both threads to complete
    // sizeTestThread.join();
    // densityTestThread.join();
    testGraphsWithSpecificSizesAndDensity();

    cout << "All tests completed and results saved to CSV files.\n";

    return 0;
}
