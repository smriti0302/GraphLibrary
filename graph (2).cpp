#include <vector>
#include <queue>

using namespace std;

// Define the Graph class
class Graph {
    int V;
    vector<int> *adj;
public:
    Graph(int V);
    void addEdge(int u, int v);
    vector<int> BFS(int s);
};

Graph::Graph(int V) {
    this->V = V;
    adj = new vector<int>[V];
}

void Graph::addEdge(int u, int v) {
    adj[u].push_back(v);
}

vector<int> Graph::BFS(int s) {
    vector<int> visited(V, false);
    vector<int> result;

    queue<int> q;
    q.push(s);
    visited[s] = true;

    while (!q.empty()) {
        int u = q.front();
        q.pop();

        result.push_back(u);

        for (auto v : adj[u]) {
            if (!visited[v]) {
                visited[v] = true;
                q.push(v);
            }
        }
    }

    return result;
}