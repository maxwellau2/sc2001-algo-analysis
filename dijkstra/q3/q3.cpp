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
    vector<int> distances;  // Shortest distances from the start node
    vector<int> pi;         // Predecessor array to track paths
    uint64_t key_comps;     // Number of key comparisons performed
};

// Mutex to prevent simultaneous file access from multiple threads
mutex fileMutex;

// MinHeap class definition
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
        return heap.front(); // Access the front without counting comparisons here
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

// Function to run Dijkstra's algorithm with an array-based priority queue
DijkstraReturn dijkstraArray(const vector<vector<int>>& graph, int startNode) {
    int n = graph.size();
    vector<int> distances(n, INT_MAX);
    vector<int> pi(n, -1);
    vector<bool> visited(n, false);
    vector<pair<int, int>> priorityQueue; // Vector-based priority queue
    uint64_t key_comps = 0; // Counter for key comparisons

    distances[startNode] = 0;
    priorityQueue.push_back({0, startNode}); // Initial entry in priority queue

    while (!priorityQueue.empty()) {
        // Extract the node with the smallest distance
        int u = -1;
        for (int i = 0; i < priorityQueue.size(); ++i) {
            key_comps++;
            if (u == -1 || priorityQueue[i].first < priorityQueue[u].first) { // Compare distances
                u = i;
            }
        }
        
        // Account for the comparison in extracting the cheapest node
        int currentNode = priorityQueue[u].second;
        priorityQueue.erase(priorityQueue.begin() + u); // Remove the extracted node

        visited[currentNode] = true;

        // Update distances for neighboring nodes
        for (int v = 0; v < n; ++v) {
            if (graph[currentNode][v] != 0 && !visited[v]) { // Check for connected unvisited nodes
                int newDistance = distances[currentNode] + graph[currentNode][v];
                key_comps++;
                if (newDistance < distances[v]) { // If a shorter path is found
                    key_comps++;
                    distances[v] = newDistance;
                    pi[v] = currentNode;
                    priorityQueue.push_back({newDistance, v}); // Add to priority queue
                }
            }
        }
    }

    return {distances, pi, key_comps}; // Return distances, predecessors, and key comparisons
}

// Function to run Dijkstra's algorithm with a min-heap-based priority queue
DijkstraReturn dijkstraMinHeap(const vector<vector<int>>& graph, int startNode) {
    int n = graph.size();
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

        // Process all neighbors of u
        for (int v = 0; v < n; ++v) {
            int weight = graph[u][v]; // Get the weight of the edge from u to v
            if (weight > 0 && !visited[v]) { // Check for connected unvisited nodes
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
    file << "Number of Nodes,Key Comparisons (Array),Key Comparisons (Min Heap),CPU Time Array (us),CPU Time MinHeap (us)\n";

    for (int num_nodes = 500; num_nodes <= 10000; num_nodes += 100) {
        vector<vector<int>> graph(num_nodes, vector<int>(num_nodes, 0));

        // Generate a random connected graph with a spanning tree
        for (int i = 0; i < num_nodes - 1; ++i) {
            int weight = rand() % 20 + 1;  // Random weight between 1 and 20
            graph[i][i + 1] = weight;
        }

        int startNode = rand() % num_nodes;

        // Test with array-based priority queue
        auto start = chrono::high_resolution_clock::now();
        DijkstraReturn resultArray = dijkstraArray(graph, startNode);
        auto end = chrono::high_resolution_clock::now();
        auto durationArray = chrono::duration_cast<chrono::nanoseconds>(end - start).count();

        // Test with min-heap-based priority queue
        start = chrono::high_resolution_clock::now();
        DijkstraReturn resultHeap = dijkstraMinHeap(graph, startNode);
        end = chrono::high_resolution_clock::now();
        auto durationHeap = chrono::duration_cast<chrono::nanoseconds>(end - start).count();

        // Write results to CSV
        lock_guard<mutex> lock(fileMutex); // Lock the mutex for thread-safe file writing
        file << num_nodes << "," << resultArray.key_comps << "," << resultHeap.key_comps << "," 
             << durationArray / 1000 << "," <<  + durationHeap /1000 << "\n"; // Convert duration to microseconds
    }

    file.close();
    cout << "Graph size test results saved to graph_size_results.csv\n";
}

// Function to test graphs of specific sizes with increasing density and record results into a CSV file
void testGraphsWithSpecificSizesAndDensity() {
    ofstream file("graph_density_results.csv");
    file << "Number of Nodes,Density (%),Key Comparisons (Array),Key Comparisons (Min Heap),CPU Time (us)\n";
    vector<int> specific_sizes = {500, 1000, 10000};

    for (int num_nodes : specific_sizes) {
        int max_possible_edges = num_nodes * (num_nodes - 1) / 2;
        for (int density = 10; density <= 100; density += 10) {
            int current_edge_count = (max_possible_edges * density) / 100;
            vector<vector<int>> graph(num_nodes, vector<int>(num_nodes, 0));

            // Generate a random connected graph with a spanning tree
            for (int i = 0; i < num_nodes - 1; ++i) {
                int weight = rand() % 20 + 1;  // Random weight between 1 and 20
                graph[i][i + 1] = weight;
            }

            // Randomly add extra edges
            for (int i = 0; i < current_edge_count; ++i) {
                int u = rand() % num_nodes;
                int v = rand() % num_nodes;
                if (u != v && graph[u][v] == 0) {
                    int weight = rand() % 20 + 1;  // Random weight between 1 and 20
                    graph[u][v] = weight;
                }
            }

            int startNode = rand() % num_nodes;

            // Test with array-based priority queue
            auto start = chrono::high_resolution_clock::now();
            DijkstraReturn resultArray = dijkstraArray(graph, startNode);
            auto end = chrono::high_resolution_clock::now();
            auto durationArray = chrono::duration_cast<chrono::nanoseconds>(end - start).count();

            // Test with min-heap-based priority queue
            start = chrono::high_resolution_clock::now();
            DijkstraReturn resultHeap = dijkstraMinHeap(graph, startNode);
            end = chrono::high_resolution_clock::now();
            auto durationHeap = chrono::duration_cast<chrono::nanoseconds>(end - start).count();

            // Write results to CSV
            lock_guard<mutex> lock(fileMutex); // Lock the mutex for thread-safe file writing
            file << num_nodes << "," << density << "," << resultArray.key_comps << "," 
                 << resultHeap.key_comps << "," << (durationArray + durationHeap) / 1000 << "\n"; // Convert duration to microseconds
        }
    }

    file.close();
    cout << "Graph density test results saved to graph_density_results.csv\n";
}

int main() {
    srand(static_cast<unsigned int>(time(0))); // Seed for random number generation

    // Run tests
    testIncreasingGraphSizes();
    testGraphsWithSpecificSizesAndDensity();

    return 0;
}
