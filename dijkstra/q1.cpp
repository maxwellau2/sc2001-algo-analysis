#include <iostream>
#include <vector>
#include <climits>
#include <cstdlib>
#include <ctime>

using namespace std;

// Struct to hold the return values of Dijkstra's algorithm
struct DijkstraReturn {
    vector<int> distances;  // Shortest distances from the start node (equivalent to 'd' in lecture notes)
    vector<int> pi;         // Predecessor array to track paths (equivalent to 'pi' in lecture notes)
    uint64_t key_comps;     // Number of key comparisons performed
};

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
            key_comps++; // Increment key comparisons
            if (graph[u][v] != 0 && !visited[v]) { // Check for connected unvisited nodes
                int newDistance = distances[u] + graph[u][v];
                key_comps++; // Increment key comparisons
                if (newDistance < distances[v]) { // If a shorter path is found, update the distance
                    key_comps++; // Increment key comparisons   
                    distances[v] = newDistance;   // Update the 'd' array
                    pi[v] = u;                   // Update the 'pi' array with predecessor
                }
            }
        }
    }

    return {distances, pi, key_comps};  // Return distances, predecessors, and key comparisons
}

// Function to generate and test a dense graph using Dijkstra's algorithm
void testDense(int num_nodes, bool verbose = false) {
    // Initialize random seed
    srand(static_cast<unsigned int>(time(0)));

    // Create an empty adjacency matrix for the graph
    vector<vector<int>> graph(num_nodes, vector<int>(num_nodes, 0));

    // Ensure the graph is connected by creating a spanning tree
    for (int i = 0; i < num_nodes - 1; ++i) {
        int weight = rand() % 20 + 1;  // Random weight between 1 and 20
        graph[i][i + 1] = weight;
        // graph[i + 1][i] = weight;      // Undirected graph
    }

    // Add additional random edges to create a dense graph
    int max_possible_edges = num_nodes * (num_nodes - 1) / 2;
    for (int edge_count = num_nodes - 1; edge_count < max_possible_edges; ++edge_count) {
        int node1 = rand() % num_nodes;
        int node2 = rand() % num_nodes;
        while (node1 == node2 || graph[node1][node2] != 0) {
            node1 = rand() % num_nodes;
            node2 = rand() % num_nodes;
        }
        int weight = rand() % 20 + 1;
        graph[node1][node2] = weight;
        graph[node2][node1] = weight;  // Undirected graph
    }

    // Run Dijkstra's algorithm on the dense graph
    int startNode = rand() % num_nodes;
    DijkstraReturn result = dijkstra(graph, startNode);

    if (verbose) {
        // Display the results
        cout << "\nDense Graph - Shortest distances from node " << startNode << ":\n";
        for (int i = 0; i < result.distances.size(); ++i) {
            cout << "To node " << i << ": " << result.distances[i] << " (predecessor: " << result.pi[i] << ")\n";
        }
    }
    cout << "Number of key comparisons performed: " << result.key_comps << endl;
}

// Function to generate and test a sparse graph using Dijkstra's algorithm
void testSparse(int num_nodes, bool verbose = false) {
    // Initialize random seed
    srand(static_cast<unsigned int>(time(0)));

    // Create an empty adjacency matrix for the graph
    vector<vector<int>> graph(num_nodes, vector<int>(num_nodes, 0));

    // Ensure the graph is connected by creating a spanning tree
    for (int i = 0; i < num_nodes - 1; ++i) {
        int weight = rand() % 20 + 1;  // Random weight between 1 and 20
        graph[i][i + 1] = weight;
        graph[i + 1][i] = weight;      // Undirected graph
    }

    // Add a small number of additional random edges to keep the graph sparse
    int additional_edges = num_nodes / 2;  // Sparsely add extra edges
    for (int edge_count = 0; edge_count < additional_edges; ++edge_count) {
        int node1 = rand() % num_nodes;
        int node2 = rand() % num_nodes;
        while (node1 == node2 || graph[node1][node2] != 0) {
            node1 = rand() % num_nodes;
            node2 = rand() % num_nodes;
        }
        int weight = rand() % 20 + 1;
        graph[node1][node2] = weight;
        graph[node2][node1] = weight;  // Undirected graph
    }

    // Run Dijkstra's algorithm on the sparse graph
    int startNode = rand() % num_nodes;
    DijkstraReturn result = dijkstra(graph, startNode);

    if (verbose) {
        // Display the results
        cout << "\nSparse Graph - Shortest distances from node " << startNode << ":\n";
        for (int i = 0; i < result.distances.size(); ++i) {
            cout << "To node " << i << ": " << result.distances[i] << " (predecessor: " << result.pi[i] << ")\n";
        }
    }
    cout << "Number of key comparisons performed: " << result.key_comps << endl;
}

// Function to generate and test a mediumly connected graph using Dijkstra's algorithm
void testMedium(int num_nodes, bool verbose = false) {
    // Initialize random seed
    srand(static_cast<unsigned int>(time(0)));

    // Create an empty adjacency matrix for the graph
    vector<vector<int>> graph(num_nodes, vector<int>(num_nodes, 0));

    // Ensure the graph is connected by creating a spanning tree
    for (int i = 0; i < num_nodes - 1; ++i) {
        int weight = rand() % 20 + 1;  // Random weight between 1 and 20
        graph[i][i + 1] = weight;
        graph[i + 1][i] = weight;      // Undirected graph
    }

    // Add a moderate number of additional random edges to make it mediumly connected
    int additional_edges = num_nodes * (num_nodes - 1) / 4;  // Add more edges for medium connectivity
    for (int edge_count = 0; edge_count < additional_edges; ++edge_count) {
        int node1 = rand() % num_nodes;
        int node2 = rand() % num_nodes;
        while (node1 == node2 || graph[node1][node2] != 0) {
            node1 = rand() % num_nodes;
            node2 = rand() % num_nodes;
        }
        int weight = rand() % 20 + 1;
        graph[node1][node2] = weight;
        graph[node2][node1] = weight;  // Undirected graph
    }

    // Run Dijkstra's algorithm on the mediumly connected graph
    int startNode = rand() % num_nodes;
    DijkstraReturn result = dijkstra(graph, startNode);

    if (verbose){
        cout << "\nMediumly Connected Graph - Shortest distances from node " << startNode << ":\n";
        for (int i = 0; i < result.distances.size(); ++i) {
            cout << "To node " << i << ": " << result.distances[i] << " (predecessor: " << result.pi[i] << ")\n";
        }
    }
    cout << "Number of key comparisons performed: " << result.key_comps << endl;
    // Display the results
}

int main() {
    int num_nodes = 60;  // You can change the number of nodes here
    testDense(num_nodes);
    testSparse(num_nodes);
    testMedium(num_nodes);
    return 0;
}
