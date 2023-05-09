#include <vector>
#include <iostream>

template <typename T, typename W>
class Edge {
public:
    Edge(const T& src, const T& dest, const W& weight)
    {
        source = src;
        destination = dest;
        weight1 = weight;
    }
    T getSource() const 
    { 
        return source; 
    }
    T getDestination() const 
    { 
        return destination; 
    }
    W getWeight() const 
    { 
        return weight1; 
    }
private:
    T source;
    T destination;
    W weight1;
};

template <typename T, typename W>
class Graph {
public:
    bool directed = false;
    struct Node;
    struct Edge;
    struct Node
    {
        T data;
        std::vector<Edge*> edges;
    };
    struct Edge
    {
        Node * source;
        Node * destination;
        W weight1;
    };
    Graph(bool dir = false)
    {
        directed = dir;
    }
    T addNode(const T& data)
    {
        nodes.push_back(Node{data});
        return data;
    };
    W addEdge(const T& src, const T& dest, const W& weight)
    {
        Node* source_node = findNodeByData(src);
        Node* dest_node = findNodeByData(dest);
        /*
        for (Node& node : nodes) 
        {
            if (node.data == src) 
            {
                Node* source_node = &node;
            }
        }
        for (Node& node : nodes) 
        {
            if (node.data == dest) 
            {
                Node* dest_node = &node;
            }
        }*/
        if (source_node && dest_node) {
            if (!directed) 
            {
                Edge* forward_edge = new Edge{source_node, dest_node, weight};
                source_node->edges.push_back(forward_edge);
                Edge* reverse_edge = new Edge{dest_node, source_node, weight};
                dest_node->edges.push_back(reverse_edge);
            }
            else
            {
                Edge* edge = new Edge{source_node, dest_node, weight};
                source_node->edges.push_back(edge);   
            }
        }
        return weight;
    };
    bool hasCycleDFS(int node, std::vector<int>& visited, std::vector<int>& parent) const 
    {
        visited[node] = true;
        for (auto& edge : nodes[node].edges) {
            int dest = edge->destination - &nodes[0];
            if (!visited[dest]) {
                parent[dest] = node;
                if (hasCycleDFS(dest, visited, parent)) {
                    return true;
                }
            }
            else if (parent[node] != dest) {
                return true;
            }
        }
        return false;
    }
    bool hasCycle() const {
        std::vector<int> parent(nodes.size(), -1);
        std::vector<int> visited(nodes.size(), false);
        for (int i = 0; i != nodes.size(); i++) 
        {
            if (!visited[i] && hasCycleDFS(i, visited, parent)) {
                return true;
            }
        }
        return false;
    }
    /*
    std::vector<int> nodeColoring() const;
    void completeEdges();
    std::vector<std::vector<T>> connectedComponents() const;
    std::vector<double> katzCentrality(double alpha, double beta) const;
    std::vector<Edge> primMST() const;
    std::vector<Edge> kruskalMST() const;
    std::vector<T> iterativeDFS(const T& start) const;
    std::vector<T> uniformCostSearch(const T& start, const T& goal) const;
    std::vector<T> aStarSearch(const T& start, const T& goal, std::function<double(T, T)> heuristic) const;
    */
    private:
        std::vector<Node> nodes;
        Node* findNodeByData(T searchData) {
        for (auto& node : nodes) {
            if (node.data == searchData) {
                return &node;
            }
        }
        return nullptr;
        };
};
int main(){
    Graph<int, int> g;
    g.addNode(0);
    g.addNode(1);
    g.addNode(2);
    g.addNode(3);

    int x1 = g.addEdge(0, 1, 10);
    int x2 = g.addEdge(1, 2, 1);
    int x3 = g.addEdge(1, 3, 2);
    int x4 = g.addEdge(2, 3, 1);
    int x = g.hasCycle();
    std::cout<<x<<std::endl;
    return 0;
}
