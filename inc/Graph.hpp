#ifndef GRAPH_HPP
# define GRAPH_HPP

# include <vector>
# include <set>

class Graph {
private:

    std::vector<std::set<int> > _adj;

    void check(int u) const;

public:

    explicit Graph(int n = 0) : _adj(n) {}

    int size() const;
    void addEdge(int u, int v);
    bool hasEdge(int u, int v) const;
    const std::set<int>& neighbors(int u) const;
    int degree(int u) const;
    int edgeCount() const;
};

#endif