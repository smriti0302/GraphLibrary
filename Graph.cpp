#include <vector>
#include <iostream>
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include "pch.h"

namespace py = pybind11;
using namespace std;
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
        Node* source;
        Node* destination;
        W weight1;
    };
    Graph(bool dir = false)
    {
        directed = dir;
    }
    T addNode(const T& data)
    {
        nodes.push_back(Node{ data });
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
                Edge* forward_edge = new Edge{ source_node, dest_node, weight };
                source_node->edges.push_back(forward_edge);
                Edge* reverse_edge = new Edge{ dest_node, source_node, weight };
                dest_node->edges.push_back(reverse_edge);
            }
            else
            {
                Edge* edge = new Edge{ source_node, dest_node, weight };
                source_node->edges.push_back(edge);
            }
        }
        return weight;
    };
    bool hasCycleDFS(int node, std::vector<bool>& visited, std::vector<int>& parent) const
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
    std::vector<int> nodeColoring() const 
    {
        std::vector<int> nodecolors(nodes.size(), -1);
        std::queue<int> q;
        for(int i = 0; i != nodes.size(); i++){
            if(nodecolors[i] == -1)
            {
                q.push(i);
                nodecolors[i] = 0;
                while(!q.empty())
                {
                    int node_index = q.front();
                    q.pop();
                    for(auto edge_ptr : nodes[node_index].edges)
                    {
                        int next_index = edge_ptr->destination - &nodes[0];
                        if(nodecolors[next_index] == -1)
                        {
                            q.push(next_index);
                            nodecolors[next_index] = - (nodecolors[node_index] - 1);
                        } 
                        else return {};
                    }
                }
            }
        }
        return nodecolors;
    };
    
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

PYBIND11_MODULE(GraphLib, m) {
    py::class_<Edge<int, int>>(m, "Edge").def(py::init<const int&, const int&, const int&>());
    py::class_<Graph<int, int>>(m, "Graph").def(py::init<bool>()).def("addNode", &Graph<int, int>::addNode).def("addEdge", &Graph<int, int>::addEdge).def("hasCycle", &Graph<int, int>::hasCycle);
}