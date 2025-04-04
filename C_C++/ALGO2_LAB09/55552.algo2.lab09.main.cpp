// ALGO2 IS1
// Kacper Małecki
// mk55552@zut.edu.pl

#include "../LAB01/lista.h"
#include "../LAB02/DynamicArray.h"
#include <algorithm>
#include <chrono>
#include <cmath>
#include <fstream>
#include <iostream>
#include <ostream>
#include <sstream>
#include <string>

struct Vector {
    double x;
    double y;

    Vector(double x0 = 0, double y0 = 0) : x(x0), y(y0) {}
    Vector(double x0, double y0, double a0, double b0)
        : x(x0 - a0), y(y0 - b0) {}

    double module() const;
    double cross(const Vector &other) const;
    Vector operator-(const Vector &other) const;
    Vector operator+(const Vector &other) const;
    bool operator<(const Vector &other) const;
    friend std::ostream &operator<<(std::ostream &os, Vector v) {
        os << v.x << ' ' << v.y;
        return os;
    }
    std::string toDot() const;
    std::string edgesToDot(const Vector &other) const;
};

struct VectorComparator {
    Vector startingPoint;

    explicit VectorComparator(Vector sp) : startingPoint(sp) {}

    bool operator()(const Vector &a, const Vector &b) const {
        Vector va = a - startingPoint;
        Vector vb = b - startingPoint;

        double crossProduct = va.cross(vb);

        if (std::abs(crossProduct) < 1e-9) {
            return va.module() < vb.module();
        }

        return crossProduct > 0;
    }
};

std::string Vector::toDot() const {
    std::ostringstream ss;
    ss << "\"" << x << "," << y << "\" " << "[" << "pos=\""
       << static_cast<int>(100 * x) << "," << static_cast<int>(100 * y)
       << "!\"];" << std::endl;
    return ss.str();
}

std::string Vector::edgesToDot(const Vector &other) const {
    std::ostringstream ss;
    ss << "\"" << x << "," << y << "\"" << " -- " << "\"" << other.x << ","
       << other.y << "\";\n";

    return ss.str();
}

double Vector::module() const { return sqrt(x * x + y * y); }

double Vector::cross(const Vector &other) const {
    return x * other.y - y * other.x;
}

Vector Vector::operator-(const Vector &other) const {
    return Vector(x - other.x, y - other.y);
}

Vector Vector::operator+(const Vector &other) const {
    return Vector(x + other.x, y + other.y);
}
bool Vector::operator<(const Vector &other) const {
    if (cross(other) < 0) {
        return false;
    } else {
        return true;
    }
}

void toDot(const DynamicArray<Vector> &array, const list<Vector> &edges,
           std::string pathname) {
    std::fstream fs;
    fs.open(pathname, std::fstream::out | std::fstream::trunc);
    if (!fs.is_open()) {
        std::cout << "Coudln't open file\n";
        return;
    }

    fs << "graph G {\n"
       << "node [shape=circle; fixedsize=true; penwidth=30; height=1.5; "
          "fontsize=16; style=\"bold\"];\n"
       << "edge [color=red; fontsize=16; penwidth=30; style=\"bold\"];\n";

    for (int i = 0; i < array.getSize(); ++i) {
        fs << array[i].toDot();
    }

    auto temp = edges.get_root();
    if (temp != nullptr) {
        while (temp->next != nullptr) {
            fs << temp->data.edgesToDot(temp->next->data);
            temp = temp->next;
        }
    }

    fs << "}";

    fs.close();
}

void degreeSort(DynamicArray<Vector, VectorComparator> &points,
                Vector startingPoint) {
    std::sort(points.begin(), points.end(), VectorComparator(startingPoint));
}

Vector getStartingPoint(const DynamicArray<Vector> &points) {
    Vector startingPoint = points[0];
    for (const auto &point : points) {
        if (point.y < startingPoint.y) {
            startingPoint = point;
        } else if (point.y == startingPoint.y) {
            if (point.x < startingPoint.x) {
                startingPoint = point;
            }
        }
    }
    return startingPoint;
}

list<Vector> grahamAlgo(const DynamicArray<Vector> &points) {
    Vector startingPoint = getStartingPoint(points);
    std::cout << "Punkt startowy: " << startingPoint << std::endl;

    DynamicArray<Vector, VectorComparator> sortedPoints(
        points, VectorComparator(startingPoint));

    auto begin = std::chrono::high_resolution_clock::now();
    degreeSort(sortedPoints, startingPoint);
    auto end = std::chrono::high_resolution_clock::now();
    auto duration =
        std::chrono::duration_cast<std::chrono::milliseconds>(end - begin);

    std::cout << "Sortowanie punktów trwało: "
              << static_cast<double>(duration.count()) / 1000 << " s"
              << std::endl;

    list<Vector> hull;
    begin = std::chrono::high_resolution_clock::now();

    hull.append_back(startingPoint);
    for (int i = 1; i < sortedPoints.getSize();) {
        if (hull.get_size() < 2) {
            hull.append_back(sortedPoints[i]);
            ++i;
        } else {
            VectorComparator comp(hull.getTail()->prev->data);
            if (comp(hull.getTail()->data, sortedPoints[i])) {
                hull.append_back(sortedPoints[i]);
                ++i;
            } else {
                hull.pop_back();
            }
        }
    }
    hull.append_back(startingPoint);

    end = std::chrono::high_resolution_clock::now();
    duration =
        std::chrono::duration_cast<std::chrono::milliseconds>(end - begin);
    std::cout << "Algorytm Grahama trwal: "
              << static_cast<double>(duration.count()) / 1000 << " s"
              << std::endl;

    return hull;
}

void readFromFile(DynamicArray<Vector> &points, const char *path) {
    std::string data;
    std::fstream fs;
    fs.open(path);
    if (!fs.is_open()) {
        std::cout << "Couldn't open file\n";
        exit(EXIT_FAILURE);
    }
    std::getline(fs, data);
    int n = std::stoi(data);
    for (int i = 0; i < n; ++i) {
        std::string data2;
        std::getline(fs, data, ' ');
        std::getline(fs, data2);
        points.pushBack(Vector(std::stod(data), std::stod(data2)));
    }
}

int main(int argc, char *argv[]) {
    if (argc != 3) {
        std::cerr << "usage: <source_file> <dest_file>" << std::endl;
        return 1;
    }
    DynamicArray<Vector> siema;
    readFromFile(siema, argv[1]);
    auto otoczka = grahamAlgo(siema);
    std::cout << "Rozmiar otoczki: " << otoczka.get_size() << std::endl;
    std::cout << "Punkty: " << std::endl;

    for (auto &points : otoczka) {
        std::cout << points << std::endl;
    }

    toDot(siema, otoczka, argv[2]);
}
