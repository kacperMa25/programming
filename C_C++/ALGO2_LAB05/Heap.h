#ifndef HEAP_H
#define HEAP_H

#include <chrono>
#include "../LAB02/DynamicArray.h"
#include <fstream>
#include <random>
#include <thread>

template <typename T, typename Comp>
class Heap{
public:
    friend std::ostream& operator<<(std::ostream& os, const Heap<T, Comp>& obj) {
        for(int i = 0; i < obj.getSize(); ++i){
            int leftChild = 2 * i + 1;
            int rightChild = 2 * i + 2;
            os << obj.arr[i] << ";" << std::endl;
            if(leftChild < obj.getSize()){
                os << obj.arr[i] << " ->" << obj.arr[leftChild] << ";" << std::endl;
            }
            if(rightChild < obj.getSize()){
                os << obj.arr[i] << " ->" << obj.arr[rightChild] << ";" << std::endl;
            }
        }
        return os;
    }

    Heap() : arr(){}
    explicit Heap(DynamicArray<T, Comp>& arr) : arr(std::move(arr)){
        repair();
    }
    ~Heap() {
      arr.clear();
    }
    void insert(T value);

    void insertWithRepeat(T value);

    T pop();
    void clear();
    [[nodiscard]] int getSize() const;
    [[nodiscard]] std::string toDot() const;
    void printToFile(const std::string& path) const;
    void repair(bool bottomUp = true);
    void heapSort();

    std::string shortRep() const;

    bool operator==(const DynamicArray<T, Comp>& obj) const;

private:
    DynamicArray<T, Comp> arr;
    Comp cmp;

    void swim(int index);
    void swimRec(int index);
    void sinkRec(int index);
};

template <typename T, typename Comp>
std::string Heap<T, Comp>::shortRep() const{
    std::ostringstream os;
    for (int i = 0; i < getSize(); ++i){
        os << arr[i] << " ";
        if (i > 10){
            os << "...";
            break;
        }
    }
    return os.str();
}


template <typename T, typename Comp>
bool Heap<T, Comp>::operator==(const DynamicArray<T, Comp>& obj) const{
    return arr == obj;
}

template <typename T, typename Comp>
void Heap<T, Comp>::heapSort(){
    int size = getSize();
    for (int lastEl = arr.getSize() - 1; lastEl >= 0; --lastEl){
        std::swap(arr[0], arr[lastEl]);
        arr.decrement();
        sinkRec(0);
    }
    arr.setSize(size);
}

template <typename T, typename Comp>
void Heap<T, Comp>::repair(bool bottomUp){
    if (bottomUp){
        for (int i = arr.getSize() - 1; i >= 0; --i){
            sinkRec(i);
        }
    } else {
        for (int i = 0; i < arr.getSize(); ++i){
            sinkRec(i);
        }
    }
}

template <typename T, typename Comp>
void Heap<T, Comp>::clear() {
    arr.clear();
}

template<typename T, typename Comp>
T Heap<T, Comp>::pop() {
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
void Heap<T, Comp>::printToFile(const std::string& path) const {
    std::ofstream file(path);
    if(!file.is_open()){
        std::cerr << "Couldn't open file: " << path << std::endl;
        return;
    }

    file << toDot();
    file.close();
}


template<typename T, typename Comp>
std::string Heap<T, Comp>::toDot() const {
    std::ostringstream ss;
    ss << "digraph Heap {" << std::endl
    << (*this)
    << "}";

    return ss.str();
}


template<typename T, typename Comp>
int Heap<T, Comp>::getSize() const {
    return arr.getSize();
}


template <typename T, typename Comp>
void Heap<T, Comp>::insert(T value) {
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

template <typename T, typename Comp>
void Heap<T, Comp>::sinkRec(int index) {
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


template <typename T, typename Comp>
void Heap<T, Comp>::swimRec(int index) {
    if (index <= 0) return;

    int parent = (index - 1) / 2;
    if (cmp(arr[index], arr[parent])) return;

    std::swap(arr[index], arr[parent]);
    swimRec(parent);
}

template <typename T, typename Comp>
void Heap<T, Comp>::swim(int index) {
    while(index > 0){
        if (cmp(arr[index / 2], arr[index])){
            std::swap(arr[index], arr[index / 2]);
        }
        else
            break;
        index = index / 2;
    }
}

#endif //HEAP_H
