#ifndef LAB02_DYNAMICARRAY_H
#define LAB02_DYNAMICARRAY_H

#include "../ALGO2_LAB01/lista.h"
#include <functional>
#include <sstream>

#define GROWTH_FACTOR 2
#define MAX 30

class Less {
  public:
    bool operator()(const int a, const int b) const { return a < b; }
};

template <typename T, typename Comp = std::less<>> class DynamicArray {
  public:
    friend std::ostream &operator<<(std::ostream &os, const DynamicArray &obj) {
        for (int i = 0; i < obj.size; ++i) {
            os << obj[i] << std::endl;
        }
        os << std::endl;
        return os;
    }

    explicit DynamicArray(Comp comp = {})
        : array(nullptr), size(0), capacity(0), cmp(comp) {}
    explicit DynamicArray(const int capacity_, Comp cmp = {})
        : array(new T[capacity_]), size(capacity_), capacity(capacity_),
          cmp(cmp) {}

    explicit DynamicArray(const int capacity_, const T &defaultValue,
                          Comp cmp = {})
        : array(new T[capacity_]), size(capacity_), capacity(capacity_),
          cmp(cmp) {
        for (int i = 0; i < capacity_; ++i) {
            array[i] = defaultValue;
        }
    }
    DynamicArray(const DynamicArray &other, Comp comp = {});
    DynamicArray(DynamicArray &&other, Comp comp = {}) noexcept;
    /*DynamicArray(const DynamicArray<T, std::less<>> &other, Comp comp);*/
    DynamicArray(const list<T> lista, Comp comp = {});
    ~DynamicArray();
    void pushBack(const T &arg);
    T &operator[](int index);
    const T &operator[](int index) const;
    void clear();
    std::string toString() const;
    int getSize() const;
    int getCapacity() const;
    void setSize(int newSize);
    void sort();
    bool contains(const T &arg) const;
    int find(const T &arg) const;
    void decrement();
    void forgetLast();
    std::string shortRep() const;
    DynamicArray &operator=(const DynamicArray &other);
    DynamicArray &operator=(DynamicArray &&other) noexcept;
    bool operator==(const DynamicArray &other) const;
    class Iterator {
      public:
        using iterator_category = std::random_access_iterator_tag;
        using value_type = T;
        using difference_type = std::ptrdiff_t;
        using pointer = T *;
        using reference = T &;
        explicit Iterator(T *ptr_) : ptr(ptr_) {}

        bool operator==(const Iterator &other) const {
            return ptr == other.ptr;
        }
        bool operator!=(const Iterator &other) const {
            return ptr != other.ptr;
        }

        T &operator*() const { return *ptr; }
        T *operator->() const { return ptr; }

        Iterator &operator++() {
            ++ptr;
            return *this;
        }
        Iterator operator++(int) {
            Iterator temp = *this;
            ++ptr;
            return temp;
        }

        Iterator &operator--() {
            --ptr;
            return *this;
        }
        Iterator operator--(int) {
            Iterator temp = *this;
            --ptr;
            return temp;
        }

        Iterator operator+(int n) const { return Iterator(ptr + n); }
        Iterator operator-(int n) const { return Iterator(ptr - n); }

        Iterator &operator+=(int n) {
            ptr += n;
            return *this;
        }
        Iterator &operator-=(int n) {
            ptr -= n;
            return *this;
        }

        int operator-(const Iterator &other) const { return ptr - other.ptr; }

        T &operator[](int n) const { return *(ptr + n); }

        bool operator<(const Iterator &other) const { return ptr < other.ptr; }
        bool operator<=(const Iterator &other) const {
            return ptr <= other.ptr;
        }
        bool operator>(const Iterator &other) const { return ptr > other.ptr; }
        bool operator>=(const Iterator &other) const {
            return ptr >= other.ptr;
        }

      private:
        T *ptr;
    };
    Iterator begin() const { return Iterator(array); }
    Iterator end() const { return Iterator(array + size); }

  private:
    T *array;
    int size;
    int capacity;
    Comp cmp;
};

template <typename T, typename Comp>
DynamicArray<T, Comp>::DynamicArray(const list<T> lista, Comp comp)
    : array(new T[lista.get_size()]), size(lista.get_size()),
      capacity(lista.get_size()), cmp(comp) {
    auto temp = lista.get_root();
    int i = 0;
    while (temp != nullptr) {
        array[i] = temp->data;
        temp = temp->next;
        ++i;
    }
}

template <typename T, typename Comp>
bool DynamicArray<T, Comp>::operator==(const DynamicArray &other) const {
    if (size != other.size)
        return false;

    for (int i = 0; i < size; ++i) {
        if (cmp(array[i], other.array[i]) && cmp(other.array[i], array[i]))
            return false;
    }
    return true;
}

template <typename T, typename Comp>
std::string DynamicArray<T, Comp>::shortRep() const {
    std::ostringstream os;
    for (int i = 0; i < getSize(); ++i) {
        os << array[i] << " ";
        if (i > 10) {
            os << "...";
            break;
        }
    }
    return os.str();
}

template <typename T, typename Comp>
void DynamicArray<T, Comp>::setSize(int newSize) {
    if (newSize <= capacity) {
        size = newSize;
    }
}

template <typename T, typename Comp>
DynamicArray<T, Comp>::DynamicArray(DynamicArray &&other, Comp comp) noexcept
    : array(other.array), size(other.size), capacity(other.capacity),
      cmp(other.cmp) {
    other.array = nullptr;
    other.size = 0;
    other.capacity = 0;
}

/*template <typename T, typename Comp>*/
/*DynamicArray<T, Comp>::DynamicArray(const DynamicArray<T, std::less<>>
 * &other,*/
/*                                    Comp comp)*/
/*    : array(new T[other.getCapacity()]), size(other.getSize()),*/
/*      capacity(other.getCapacity()), cmp(comp) {*/
/*    for (int i = 0; i < size; ++i) {*/
/*        array[i] = other[i];*/
/*    }*/
/*}*/

template <typename T, typename Comp>
DynamicArray<T, Comp> &
DynamicArray<T, Comp>::operator=(DynamicArray &&other) noexcept {
    if (this == &other)
        return *this;
    clear();

    array = other.array;
    size = other.size;
    capacity = other.capacity;
    cmp = other.cmp;

    other.array = nullptr;
    other.size = 0;
    other.capacity = 0;

    return *this;
}

template <typename T, typename Comp>
DynamicArray<T, Comp> &
DynamicArray<T, Comp>::operator=(const DynamicArray &other) {
    if (this == &other)
        return *this;

    clear();

    for (int i = 0; i < other.getSize(); ++i) {
        pushBack(other[i]);
    }
    return *this;
}

template <typename T, typename Comp>
DynamicArray<T, Comp>::DynamicArray(const DynamicArray &other, Comp comp)
    : array(nullptr), size(0), capacity(0) {
    for (int i = 0; i < other.getSize(); ++i) {
        pushBack(other[i]);
    }
}

template <typename T, typename Comp> void DynamicArray<T, Comp>::decrement() {
    size--;
}

template <typename T, typename Comp>
bool DynamicArray<T, Comp>::contains(const T &arg) const {
    bool found = false;
    for (int i = 0; i < this->size; i++) {
        if (!cmp(arg, this->array[i]) && !cmp(this->array[i], arg)) {
            found = true;
            break;
        }
    }
    return found;
}

template <typename T, typename Comp>
int DynamicArray<T, Comp>::find(const T &arg) const {
    bool index = -1;
    for (int i = 0; i < this->size; i++) {
        if (!cmp(arg, this->array[i]) && !cmp(this->array[i], arg)) {
            index = i;
            break;
        }
    }
    return index;
}

template <typename T, typename Comp> DynamicArray<T, Comp>::~DynamicArray() {
    clear();
}

template <typename T, typename Comp>
void DynamicArray<T, Comp>::pushBack(const T &arg) {
    if (size == 0) {
        ++size;
        ++capacity;
        array = new T[1];
        array[0] = arg;
    } else {
        if (capacity > size) {
            array[size] = arg;
            ++size;
        } else {
            capacity = GROWTH_FACTOR * capacity;
            auto temp = array;
            array = new T[capacity];
            for (int i = 0; i < size; ++i) {
                array[i] = temp[i];
            }
            array[size] = arg;
            ++size;
            delete[] temp;
        }
    }
}

template <typename T, typename Comp>
T &DynamicArray<T, Comp>::operator[](int index) {
    if (index > -1 && index < size)
        return array[index];
    throw std::out_of_range("Index poza zakresem\n");
}

template <typename T, typename Comp>
const T &DynamicArray<T, Comp>::operator[](int index) const {
    if (index > -1 && index < size)
        return array[index];
    throw std::out_of_range("Indeks poza zakresem\n");
}

template <typename T, typename Comp> void DynamicArray<T, Comp>::clear() {
    if (array != nullptr) {
        delete[] array;
        array = nullptr;
    }
    size = 0;
    capacity = 0;
}

template <typename T, typename Comp>
std::string DynamicArray<T, Comp>::toString() const {
    std::ostringstream temp;
    temp << "Rozmiar: " << size << std::endl
         << "Pojemnosc: " << capacity << std::endl
         << "Typ: " << typeid(T).name() << std::endl;

    return std::string(temp.str());
}

template <typename T, typename Comp>
int DynamicArray<T, Comp>::getSize() const {
    return size;
}

template <typename T, typename Comp>
int DynamicArray<T, Comp>::getCapacity() const {
    return capacity;
}

template <typename T, typename Comp> void DynamicArray<T, Comp>::sort() {
    for (int i = 0; i < size; ++i) {
        for (int j = 0; j < size - i; ++j) {
            if (cmp(array[j], array[j + 1])) {
                std::swap(array[j], array[j + 1]);
            }
        }
    }
}

inline int less(int a, int b) { return a > b; }

#endif // LAB02_DYNAMICARRAY_H
