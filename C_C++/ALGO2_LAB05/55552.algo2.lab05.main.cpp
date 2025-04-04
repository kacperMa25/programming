// ALGO2 IS1 223A LAB05
// Kacper Małecki
// mk55552@zut.edu.pl

#include "../ALGO2_LAB02/DynamicArray.h"
#include <chrono>
#include <fstream>
#include <random>
#include <thread>

template <typename T, typename Comp> class Heap {
  public:
    friend std::ostream &operator<<(std::ostream &os,
                                    const Heap<T, Comp> &obj) {
        for (int i = 0; i < obj.getSize(); ++i) {
            int leftChild = 2 * i + 1;
            int rightChild = 2 * i + 2;
            os << obj.arr[i] << ";" << std::endl;
            if (leftChild < obj.getSize()) {
                os << obj.arr[i] << " ->" << obj.arr[leftChild] << ";"
                   << std::endl;
            }
            if (rightChild < obj.getSize()) {
                os << obj.arr[i] << " ->" << obj.arr[rightChild] << ";"
                   << std::endl;
            }
        }
        return os;
    }
    Heap() : arr() {}
    ~Heap() { arr.clear(); }
    void insert(T value);

    void insertWithRepeat(T value);

    T pop();
    void clear();
    [[nodiscard]] int getSize() const;
    [[nodiscard]] std::string toDot() const;
    void printToFile(const std::string &path) const;
    void heapSort();

  private:
    DynamicArray<T, Comp> arr;
    Comp cmp;

    void swim(int index);
    void swimRec(int index);
    void sinkRec(int index);
};

template <typename T, typename Comp> void Heap<T, Comp>::clear() {
    arr.clear();
}

template <typename T, typename Comp> T Heap<T, Comp>::pop() {
    if (arr.getSize() == 0) {
        throw std::out_of_range("Heap is empty\n");
    }
    T max = arr[0];
    arr[0] = arr[arr.getSize() - 1];
    arr.decrement();
    sinkRec(0);
    return max;
}

template <typename T, typename Comp>
void Heap<T, Comp>::printToFile(const std::string &path) const {
    std::ofstream file(path);
    if (!file.is_open()) {
        std::cerr << "Couldn't open file: " << path << std::endl;
        return;
    }

    file << toDot();
    file.close();
}

template <typename T, typename Comp> std::string Heap<T, Comp>::toDot() const {
    std::ostringstream ss;
    ss << "digraph Heap {" << std::endl << (*this) << "}";

    return ss.str();
}

template <typename T, typename Comp> int Heap<T, Comp>::getSize() const {
    return arr.getSize();
}

template <typename T, typename Comp> void Heap<T, Comp>::insert(T value) {
    if (!arr.contains(value)) {
        arr.pushBack(value);
        swimRec(arr.getSize() - 1);
    }
}

template <typename T, typename Comp>
void Heap<T, Comp>::insertWithRepeat(T value) {
    arr.pushBack(value);
    swimRec(arr.getSize() - 1);
}

template <typename T, typename Comp> void Heap<T, Comp>::sinkRec(int index) {
    int leftChild = index * 2 + 1;
    int rightChild = index * 2 + 2;
    if (arr.getSize() <= leftChild) {
        return;
    }

    int biggerChild = leftChild;
    if (rightChild < arr.getSize() && cmp(arr[leftChild], arr[rightChild])) {
        biggerChild = rightChild;
    }

    if (cmp(arr[biggerChild], arr[index])) {
        return;
    }
    std::swap(arr[index], arr[biggerChild]);
    sinkRec(biggerChild);
}

template <typename T, typename Comp> void Heap<T, Comp>::swimRec(int index) {
    if (index <= 0)
        return;

    int parent = (index - 1) / 2;
    if (cmp(arr[index], arr[parent]))
        return;

    std::swap(arr[index], arr[parent]);
    swimRec(parent);
}

template <typename T, typename Comp> void Heap<T, Comp>::swim(int index) {
    while (index > 0) {
        if (cmp(arr[index / 2], arr[index])) {
            std::swap(arr[index], arr[index / 2]);
        } else
            break;
        index = index / 2;
    }
}

void timeResults() {
    using namespace std::chrono;
    Heap<int, Less> kopiec;
    std::random_device rd;
    std::mt19937 generator(rd());
    std::uniform_int_distribution<int> distribution(0, 10000000);

    const int MAX_ORDER = 20;
    for (int i = 2; i < MAX_ORDER; ++i) {

        const int n = std::pow(4, i);

        auto begin = high_resolution_clock::now();
        for (int j = 0; j < n; ++j) {
            kopiec.insertWithRepeat(distribution(generator));
        }
        auto end = high_resolution_clock::now();
        auto duration = duration_cast<milliseconds>(end - begin);

        int size = kopiec.getSize();
        std::cout << "Rozmiar: " << size << std::endl
                  << "Dodawanie do kopca trwalo: "
                  << static_cast<double>(duration.count()) / size << " ms"
                  << std::endl;

        begin = high_resolution_clock::now();
        while (kopiec.getSize() > 0) {
            kopiec.pop();
        }
        end = high_resolution_clock::now();
        duration = duration_cast<milliseconds>(end - begin);

        std::cout << "Usuwanie maksymalnego elementu trwalo "
                  << static_cast<double>(duration.count()) / size << " ms"
                  << std::endl
                  << "===============================\n";
        kopiec.clear();
    }
}

void generateHeap(int order) {
    Heap<int, Less> kopiec;
    std::random_device rd;
    std::mt19937 generator(rd());
    std::uniform_int_distribution<int> distribution(0, 100);

    for (int i = 0; i < order; ++i) {
        kopiec.insert(distribution(generator));
    }
    kopiec.printToFile("../graph.dot");
}

int main() { timeResults(); }
