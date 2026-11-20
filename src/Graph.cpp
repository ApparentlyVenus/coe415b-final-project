#include "../inc/Graph.hpp"
#include <stdexcept>

void Graph::check(int u) const {
    if (u < 0 || u >= size())
        throw std::out_of_range("Graph: bad vertex");
}

int Graph::size() const { return static_cast<int>(_adj.size()); }

void Graph::addEdge(int u, int v) {
    check(u);
    check(v);
    if (u == v)
        return;
    _adj[u].insert(v);
    _adj[v].insert(u);
}

bool Graph::hasEdge(int u, int v) const {
    check(u);
    return _adj[u].count(v) > 0;
}

const std::set<int>& Graph::neighbors(int u) const {
    check(u);
    return _adj[u];
}

int Graph::degree(int u) const {
    check(u);
    return static_cast<int>(_adj[u].size());
}

int Graph::edgeCount() const {
    int total = 0;
    for (size_t i = 0; i < _adj.size(); ++i)
        total += static_cast<int>(_adj[i].size());
    return total / 2;
}