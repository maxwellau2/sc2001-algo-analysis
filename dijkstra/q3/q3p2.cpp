
#include <fstream> // Include for file handling
#include <iostream>
#include <vector>
#include <climits>
#include <cstdlib>
#include <ctime>
#include <chrono>
#include <set>
#include <algorithm>

using namespace std;

// Struct to hold the return values of Dijkstra's algorithm
struct DijkstraReturn {
    vector<int> distances;  // Shortest distances from the start node
    vector<int> pi;         // Predecessor array to track paths
    uint64_t key_comps;     // Number of key comparisons performed
};

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

// Dijkstra's algorithm using adjacency matrix
DijkstraReturn dijkstraMatrix(const vector<vector<int>>& adjMatrix, int startNode) {
    int n = adjMatrix.size();
    vector<int> distances(n, INT_MAX);
    vector<int> pi(n, -1);
    vector<bool> visited(n, false);
    MinHeap minHeap;

    distances[startNode] = 0;
    minHeap.push({startNode, 0});

    while (!minHeap.empty()) {
        int u = minHeap.top().first;
        minHeap.pop();
        visited[u] = true;

        for (int v = 0; v < n; ++v) {
            int weight = adjMatrix[u][v];
            minHeap.key_comps++; // Count key comparison for distance check
            if (weight > 0 && !visited[v]) {
                minHeap.key_comps++; // Count key comparison for distance check
                int newDistance = distances[u] + weight;
                if (newDistance < distances[v]) {
                    minHeap.key_comps++; // Count key comparison for update check
                    distances[v] = newDistance;
                    pi[v] = u;
                    minHeap.push({v, newDistance});
                }
            }
        }
    }

    return {distances, pi, minHeap.key_comps};
}

// Dijkstra's algorithm using adjacency list
DijkstraReturn dijkstraList(const vector<vector<pair<int, int>>>& adjList, int startNode) {
    int n = adjList.size();
    vector<int> distances(n, INT_MAX);
    vector<int> pi(n, -1);
    vector<bool> visited(n, false);
    MinHeap minHeap;

    distances[startNode] = 0;
    minHeap.push({startNode, 0});

    while (!minHeap.empty()) {
        int u = minHeap.top().first;
        minHeap.pop();
        visited[u] = true;

        for (const auto& neighbor : adjList[u]) {
            minHeap.key_comps++; // Count key comparison for distance check
            int v = neighbor.first;
            int weight = neighbor.second;
            minHeap.key_comps++; // Count key comparison for distance check
            if (!visited[v]) {
                int newDistance = distances[u] + weight;
                minHeap.key_comps++; // Count key comparison for update check
                if (newDistance < distances[v]) {
                    distances[v] = newDistance;
                    pi[v] = u;
                    minHeap.push({v, newDistance});
                }
            }
        }
    }

    return {distances, pi, minHeap.key_comps};
}

// Function to generate a directed minimum spanning tree (MST) using Prim's algorithm
void generateMST(int n, vector<vector<int>>& adjMatrix) {
    vector<bool> inMST(n, false);
    vector<int> minEdge(n, INT_MAX); // Minimum edge cost to reach the MST
    vector<int> parent(n, -1); // Array to store constructed MST
    minEdge[0] = 0; // Start from the first node

    for (int count = 0; count < n - 1; ++count) {
        // Find the minimum edge
        int u = -1;
        for (int i = 0; i < n; ++i) {
            if (!inMST[i] && (u == -1 || minEdge[i] < minEdge[u])) {
                u = i;
            }
        }

        inMST[u] = true; // Add the selected node to the MST

        // Update the minEdge and parent
        for (int v = 0; v < n; ++v) {
            int weight = rand() % 10 + 1; // Random weight for each edge
            if (!inMST[v] && weight < minEdge[v]) {
                minEdge[v] = weight;
                parent[v] = u;
                adjMatrix[u][v] = weight; // Set the weight for the directed edge
            }
        }
    }
}

// Function to add random edges to the graph based on the desired density
void addRandomEdges(int n, vector<vector<int>>& adjMatrix, double density) {
    int initialEdges = n - 1; // Edges in the MST
    int maxEdges = n * (n - 1); // Maximum edges in a directed graph
    int targetEdges = static_cast<int>(maxEdges * density); // Target edges based on density
    int edgesToAdd = targetEdges - initialEdges; // Additional edges to add

    set<pair<int, int>> addedEdges; // To keep track of added edges

    // Add random edges until we reach the target number
    while (edgesToAdd > 0) {
        int u = rand() % n;
        int v = rand() % n;
        if (u != v && addedEdges.find({u, v}) == addedEdges.end() && adjMatrix[u][v] == 0) {
            int weight = rand() % 10 + 1; // Random weight for each new edge
            adjMatrix[u][v] = weight; // Add the edge
            addedEdges.insert({u, v}); // Mark this edge as added
            edgesToAdd--;
        }
    }
}

// Function to generate a random graph as an adjacency list from the adjacency matrix
vector<vector<pair<int, int>>> convertToAdjacencyList(const vector<vector<int>>& adjMatrix) {
    int n = adjMatrix.size();
    vector<vector<pair<int, int>>> adjList(n);
    for (int u = 0; u < n; ++u) {
        for (int v = 0; v < n; ++v) {
            if (adjMatrix[u][v] > 0) {
                adjList[u].emplace_back(v, adjMatrix[u][v]);
            }
        }
    }
    return adjList;
}


// Main function to run the density experiment
int main() {
    srand(time(0)); // Seed for randomness
    int n = 500; // Number of nodes
    double densities[] = {0.1, 0.2, 0.3, 0.4, 0.5, 0.6, 0.7, 0.8, 0.9, 0.99}; // Different densities to test
    int startNode = 0; // Starting node for Dijkstra's algorithm

    // Open a CSV file to write results
    ofstream resultsFile("density_experiment_results.csv");
    if (!resultsFile) {
        cerr << "Error opening file for writing." << endl;
        return 1;
    }

    // Write header to CSV
    resultsFile << "Density,Adjacency Matrix Key Comps,Adjacency List Key Comps,Adjacency Matrix Time (ms),Adjacency List Time (ms)\n";

    // Output console header
    cout << "Density Experiment Results:\n";
    cout << "Density\tAdjacency Matrix Key Comps\tAdjacency List Key Comps\tAdjacency Matrix Time (ms)\tAdjacency List Time (ms)\n";

    for (double density : densities) {
        vector<vector<int>> adjMatrix(n, vector<int>(n, 0)); // Adjacency matrix initialized to zero

        // Generate the minimum spanning tree
        generateMST(n, adjMatrix);

        // Add random edges based on the desired density
        addRandomEdges(n, adjMatrix, density);

        // Convert the adjacency matrix to an adjacency list
        auto adjList = convertToAdjacencyList(adjMatrix);

        // Timing Dijkstra's algorithm for adjacency matrix
        auto startMatrix = chrono::high_resolution_clock::now();
        DijkstraReturn resultMatrix = dijkstraMatrix(adjMatrix, startNode);
        auto endMatrix = chrono::high_resolution_clock::now();
        auto durationMatrix = chrono::duration_cast<chrono::milliseconds>(endMatrix - startMatrix).count();

        // Timing Dijkstra's algorithm for adjacency list
        auto startList = chrono::high_resolution_clock::now();
        DijkstraReturn resultList = dijkstraList(adjList, startNode);
        auto endList = chrono::high_resolution_clock::now();
        auto durationList = chrono::duration_cast<chrono::milliseconds>(endList - startList).count();

        // Output the key comparisons and time for both implementations
        cout << density * 100 << "%\t" << resultMatrix.key_comps << "\t\t\t" 
             << resultList.key_comps << "\t\t\t" 
             << durationMatrix << "\t\t\t\t" << durationList << "\n";

        // Write the results to the CSV file
        resultsFile << density * 100 << "," << resultMatrix.key_comps << "," 
                    << resultList.key_comps << "," << durationMatrix << "," 
                    << durationList << "\n";
    }

    // Close the results file
    resultsFile.close();

    cout << "Results written to density_experiment_results.csv" << endl;

    return 0;
}

