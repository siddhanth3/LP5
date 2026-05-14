// ================================================================
// INCLUDES: These libraries give us different tools to use
// ================================================================
#include <iostream>  // For input/output (cin, cout)
#include <vector>    // For dynamic arrays (list of lists)
#include <queue>     // For BFS - stores nodes waiting to be processed
#include <chrono>    // For measuring execution time
#include <omp.h>     // For parallel processing (multi-threading)
#include <cmath>     // For mathematical functions

using namespace std;

// ================================================================
// TREE CLASS: Blueprint for creating a tree data structure
// ================================================================
class Tree {

    // Private variables - hidden from outside
    int V;                      // V = Number of nodes in tree
    vector<vector<int>> adj;    // Adjacency list - stores which nodes connect to each other

public:

    // ===== CONSTRUCTOR: Initializes the tree when created =====
    Tree(int V) {

        this->V = V;           // Store the number of nodes
        adj.resize(V);         // Create empty list for each node (0 to V-1)
    }

    // ===== ADDEDGE FUNCTION: Connects two nodes together =====
    void addEdge(int parent, int child) {

        adj[parent].push_back(child);  // Add child to parent's list
        adj[child].push_back(parent);  // Add parent to child's list (bidirectional - like a road both ways)
    }

    // ===== DISPLAYTREE FUNCTION: Shows the tree structure =====
    void displayTree() {

        cout << "\nTree Structure:\n";

        // Loop through each node (0 to V-1)
        for (int i = 0; i < V; i++) {

            cout << i << " -> ";  // Print current node

            // Loop through all neighbors of this node
            for (int child : adj[i]) {

                cout << child << " ";  // Print each neighbor
            }

            cout << endl;  // Move to next line
        }
    }

    // ===== PARALLEL BFS FUNCTION: Breadth-First Search with parallel processing =====
    // BFS explores level by level (like layers of an onion)
    double parallelBFS(int start) {

        // Create a tracker for visited nodes (false = not visited, true = visited)
        vector<bool> visited(V, false);

        // Create a queue - like a line in a store (first come, first served)
        queue<int> q;

        // Mark starting node as visited and add to queue
        visited[start] = true;
        q.push(start);

        cout << "\nParallel BFS Traversal: ";

        // Start the timer to measure execution time
        auto startTime = chrono::high_resolution_clock::now();

        // Keep processing while queue has nodes
        while (!q.empty()) {

            int node;

            // Critical section - only one thread processes at a time (prevents conflicts)
            #pragma omp critical
            {
                node = q.front();   // Get node from front of queue
                q.pop();            // Remove it from queue

                cout << node << " ";  // Print the node we're visiting
            }

            // Parallel for - multiple threads process neighbors simultaneously
            #pragma omp parallel for
            for (int i = 0; i < adj[node].size(); i++) {

                int neighbor = adj[node][i];  // Get each neighbor of current node

                // If neighbor not yet visited
                if (!visited[neighbor]) {

                    // Critical section - ensure thread safety
                    #pragma omp critical
                    {
                        // Double-check if still not visited (another thread might have visited it)
                        if (!visited[neighbor]) {

                            visited[neighbor] = true;  // Mark as visited
                            q.push(neighbor);         // Add to queue for processing
                        }
                    }
                }
            }
        }

        // Stop the timer
        auto endTime = chrono::high_resolution_clock::now();
        
        // Calculate elapsed time in milliseconds
        double duration = chrono::duration<double, milli>(endTime - startTime).count();

        cout << "\nBFS Execution Time: " << duration << " ms" << endl;

        return duration;  // Return the time taken
    }

    // ===== PARALLEL DFS UTILITY FUNCTION: Helper function for DFS =====
    // DFS explores deeply first (like going down a tunnel before exploring other tunnels)
    void parallelDFSUtil(int node, vector<bool>& visited) {

        bool alreadyVisited;  // Flag to check if node was already visited

        // Critical section - ensure thread safety
        #pragma omp critical
        {
            alreadyVisited = visited[node];  // Check current status

            // If this node hasn't been visited yet
            if (!visited[node]) {

                visited[node] = true;     // Mark it as visited

                cout << node << " ";      // Print the node
            }
        }

        // If already visited in another thread, stop processing this branch
        if (alreadyVisited)
            return;

        // Parallel for - multiple threads process neighbors simultaneously
        #pragma omp parallel for
        for (int i = 0; i < adj[node].size(); i++) {

            int neighbor = adj[node][i];  // Get each neighbor

            // If neighbor not visited
            if (!visited[neighbor]) {

                // Recursively visit this neighbor (go deeper)
                parallelDFSUtil(neighbor, visited);
            }
        }
    }

    // ===== PARALLEL DFS FUNCTION: Main DFS traversal function =====
    double parallelDFS(int start) {

        // Create a tracker for visited nodes
        vector<bool> visited(V, false);

        cout << "\nParallel DFS Traversal: ";

        // Start the timer
        auto startTime = chrono::high_resolution_clock::now();

        // Call the DFS utility function to do the actual work
        parallelDFSUtil(start, visited);

        // Stop the timer
        auto endTime = chrono::high_resolution_clock::now();
        
        // Calculate elapsed time in milliseconds
        double duration = chrono::duration<double, milli>(endTime - startTime).count();

        cout << "\nDFS Execution Time: " << duration << " ms" << endl;

        return duration;  // Return the time taken
    }
};

// ================================================================
// MAIN FUNCTION: Entry point of the program
// ================================================================
int main() {

    // STEP 1: Get number of nodes from user
    int V;  // V = number of nodes/vertices

    cout << "Enter number of nodes in tree: ";
    cin >> V;

    // STEP 2: Create a tree object with V nodes
    Tree t(V);

    // STEP 3: Get edges from user (connections between nodes)
    // Note: A tree with V nodes has exactly V-1 edges
    cout << "Enter " << V - 1 << " edges:\n";

    for (int i = 0; i < V - 1; i++) {

        int u, v;  // u and v are two nodes to connect

        cin >> u >> v;

        t.addEdge(u, v);  // Connect these two nodes
    }

    // STEP 4: Get starting node (root) from user
    int root;

    cout << "Enter root node: ";
    cin >> root;

    // STEP 5: Display the tree structure (adjacency list)
    t.displayTree();

    // STEP 6: Perform Parallel BFS and record time
    double bfsTime = t.parallelBFS(root);

    // STEP 7: Perform Parallel DFS and record time
    double dfsTime = t.parallelDFS(root);

    // STEP 8: Show comparison of performance
    cout << "\n===== Performance Summary =====\n";
    cout << "BFS Time: " << bfsTime << " ms\n";
    cout << "DFS Time: " << dfsTime << " ms\n";
    cout << "Faster: " << (bfsTime < dfsTime ? "BFS" : "DFS") << " by " 
         << fabs(bfsTime - dfsTime) << " ms\n";
    cout << "================================\n";

    return 0;  // Program ends successfully
}

// Enter number of nodes in tree: 6
// Enter 5 edges:
// 0 1
// 0 2
// 1 3
// 1 4
// 2 5
// Enter root node: 0

// Tree Structure:
// 0 -> 1 2 
// 1 -> 0 3 4 
// 2 -> 0 5 
// 3 -> 1 
// 4 -> 1 
// 5 -> 2 
// g++ -fopenmp -o dbfs dbfs.cpp
// ./dbfs.exe

