// ALGO2 IS1
// Kacper Małecki
// mk55552@zut.edu.pl

#include "../LAB01/lista.h"
#include "../LAB02/DynamicArray.h"
#include <algorithm>
#include <chrono>
#include <fstream>
#include <functional>
#include <iostream>
#include <ostream>
#include <sstream>
#include <string>
#include <utility>

using Graph =
    DynamicArray<list<std::pair<int, float>, std::less<>>, std::less<>>;

class UnionFind {
  public:
    UnionFind(int count);
    int Find(int node, bool withCompression = true);
    int FindWithCompression(int node);
    void Union(int node1, int node2);
    void UnionByRank(int node1, int node2);
    bool Connected(int node1, int node2);
    int getCounter() { return findCounter; }

  private:
    DynamicArray<int, std::less<>> parent;
    DynamicArray<int, std::less<>> ranks;
    int findCounter;
};

UnionFind::UnionFind(int count) {
    for (int i = 0; i < count; ++i) {
        parent.pushBack(i);
    }
    for (int i = 0; i < count; ++i) {
        ranks.pushBack(1);
    }
    findCounter = 0;
}

int UnionFind::Find(int node, bool withCompression) {
    ++findCounter;
    if (withCompression) {
        if (parent[node] == node) {
            return node;
        } else {
            parent[node] = FindWithCompression(parent[node]);
            return parent[node];
        }
    } else {
        while (parent[node] != node) {
            node = parent[node];
        }
        return node;
    }
}

int UnionFind::FindWithCompression(int node) {
    ++findCounter;
    if (parent[node] == node) {
        return node;
    } else {
        parent[node] = FindWithCompression(parent[node]);
        return parent[node];
    }
}

void UnionFind::Union(int node1, int node2) {
    int u = Find(node1);
    int v = Find(node2);

    if (u != v) {
        parent[v] = u;
    }
}

void UnionFind::UnionByRank(int node1, int node2) {
    int u = Find(node1);
    int v = Find(node2);

    if (v == u)
        return;

    if (ranks[u] < ranks[v]) {
        parent[u] = v;
        ranks[v] += ranks[u];
    } else {
        parent[v] = u;
        ranks[u] += ranks[v];
    }
}

bool UnionFind::Connected(int node1, int node2) {
    int u = Find(node1);
    int v = Find(node2);

    if (u == v)
        return true;
    return false;
}

bool getFromFile(
    Graph &grapth0,
    DynamicArray<std::pair<float, float>, std::less<>> &coordinates0,
    std::string path) {
    std::ifstream file;
    std::string data;
    file.open(path);
    if (!file.is_open()) {
        std::cout << "Nie udalo sie otworzyc pliku" << std::endl;
        return false;
    }

    std::getline(file, data);
    int number = std::stoi(data);
    Graph graph(number, list<std::pair<int, float>, std::less<>>{});
    DynamicArray<std::pair<float, float>, std::less<>> coordinates(number);

    for (int i = 0; i < number; ++i) {
        std::getline(file, data, ' ');
        coordinates[i].first = std::stof(data);
        std::getline(file, data, '\n');
        coordinates[i].second = std::stof(data);
    }

    std::getline(file, data);
    number = std::stoi(data);
    for (int i = 0; i < number; ++i) {
        std::string data2;
        std::string data3;
        std::getline(file, data, ' ');
        std::getline(file, data2, ' ');
        std::getline(file, data3, '\n');
        graph[std::stoi(data)].append_back(
            std::pair(std::stoi(data2), std::stof(data3)));
    }

    grapth0 = std::move(graph);
    coordinates0 = std::move(coordinates);
    return true;
}

struct trio {
    trio() : indx1(0), indx2(0), val(0.0f) {}
    trio(int i1, int i2, float v) : indx1(i1), indx2(i2), val(v) {}
    int indx1;
    int indx2;
    float val;
    bool operator<(trio other) const { return val < other.val; }
};

Graph KruskalMST(Graph graph0) {
    UnionFind unia(graph0.getSize());
    DynamicArray<trio, std::less<>> tab;

    for (int i = 0; i < graph0.getSize(); ++i) {
        auto temp = graph0[i].get_root();
        while (temp != nullptr) {
            tab.pushBack(trio(i, temp->data.first, temp->data.second));
            temp = temp->next;
        }
    }

    auto begin = std::chrono::high_resolution_clock::now();
    std::sort(tab.begin(), tab.end());
    auto end = std::chrono::high_resolution_clock::now();

    auto diff =
        std::chrono::duration_cast<std::chrono::milliseconds>(end - begin);

    std::cout << "Czas sortowania krawedzi trwal: "
              << static_cast<double>(diff.count()) / 1000 << " s" << std::endl;

    Graph MST(graph0.getSize(), list<std::pair<int, float>, std::less<>>{});

    int edgeCounter = 0;
    double valCounter = 0;
    begin = std::chrono::high_resolution_clock::now();
    for (const auto &edge : tab) {
        if (!unia.Connected(edge.indx1, edge.indx2)) {
            unia.UnionByRank(edge.indx1, edge.indx2);
            MST[edge.indx1].append_back(std::pair(edge.indx2, edge.val));
            ++edgeCounter;
            valCounter += edge.val;
        }
    }
    end = std::chrono::high_resolution_clock::now();
    diff = std::chrono::duration_cast<std::chrono::milliseconds>(end - begin);

    std::cout << "Czas wykonania głownej pętli: "
              << static_cast<double>(diff.count()) / 1000 << " s" << std::endl;
    std::cout << "Liczba operacji Find: " << unia.getCounter() << std::endl;
    std::cout << "Liczba krawedzi: " << edgeCounter << std::endl;
    std::cout << "Suma wartosci krawedzi: " << valCounter << std::endl;

    return MST;
}

void drawGraph(Graph &graph,
               DynamicArray<std::pair<float, float>, std::less<>> &coordinates,
               std::string path) {
    std::ostringstream oss;
    oss << "Graph G {\n"
        << "node [shape=circle; fixedsize=true; color=red; height=2.0; "
           "fontsize=64; style=\"bold\"];\n"
        << "edge [color=red; fontsize=64; style=\"bold\"];\n";

    for (int i = 0; i < graph.getSize(); ++i) {
        oss << i << " [pos=\"" << static_cast<int>(coordinates[i].first * 100)
            << "," << static_cast<int>(coordinates[i].second * 100)
            << "!\"];\n";

        auto temp = graph[i].get_root();
        while (temp != nullptr) {
            oss << i << " -- " << temp->data.first << " [label=\""
                << temp->data.second << "\"];\n";
            temp = temp->next;
        }
    }
    oss << "}";

    std::ofstream file(path);

    if (file.is_open()) {
        file << oss.str();
        file.close();
    }
}

int main() {
    Graph graph;
    DynamicArray<std::pair<float, float>, std::less<>> coordinates;

    std::cout << "G1: " << std::endl;
    getFromFile(graph, coordinates, "g1.txt");
    Graph MST = KruskalMST(graph);
    drawGraph(MST, coordinates, "graph1.dot");
    graph.clear();
    MST.clear();
    coordinates.clear();
    std::cout << "================================" << std::endl;

    std::cout << "G2: " << std::endl;
    getFromFile(graph, coordinates, "g2.txt");
    MST = KruskalMST(graph);
    drawGraph(MST, coordinates, "graph2.dot");
    graph.clear();
    MST.clear();
    coordinates.clear();
    std::cout << "================================" << std::endl;

    std::cout << "G3: " << std::endl;
    getFromFile(graph, coordinates, "g3.txt");
    MST = KruskalMST(graph);
    drawGraph(MST, coordinates, "graph3.dot");
}
