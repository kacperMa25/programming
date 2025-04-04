#ifndef BRUDNOPIS_LISTA_H
#define BRUDNOPIS_LISTA_H

#include <functional>
#include <ostream>
#include <sstream>
#include <stdexcept>
#include <string>

template <typename T, typename Comp = std::equal_to<>> class list {
  public:
    friend std::ostream &operator<<(std::ostream &os, const list<T> &lista) {
        auto temp = lista.get_root();
        while (temp != nullptr) {
            os << temp->data << ' ';
            temp = temp->next;
        }
        return os;
    }
    struct node {
        T data;
        node *next;
        node *prev;
        explicit node(const T &var) : data(var), next(nullptr), prev(nullptr) {}
    };

    explicit list(Comp equal_ = {})
        : head(nullptr), tail(nullptr), size(0), equal(equal_) {}
    list(const list &other);
    ~list();
    void append_front(const T &var);
    void append_back(const T &var);
    bool pop_front();
    bool pop_back();
    const node *getTail() const;
    const T &get(int index) const;
    void set(int index, T &value);
    node *find(const T &key) const;
    bool pop(const T &key);
    void bubbleSort(int (*data_cmp)(T, T));
    void append_sort(const T &var, int (*data_cmp)(T, T));
    void clear();
    std::string to_string(int var);
    int get_size() const;
    const node *get_root() const;
    list &operator=(const list &other);
    T operator[](const int i) const;
    T &operator[](const int i);

    list(list &&other) noexcept;

    list &operator=(list &&other) noexcept;

    class Iterator {
      public:
        using iterator_category = std::random_access_iterator_tag;
        using value_type = T;
        using difference_type = std::ptrdiff_t;
        using pointer = T *;
        using reference = T &;
        friend class list;
        explicit Iterator(node *ptr_) : ptr(ptr_) {};
        bool operator==(Iterator it) const;
        bool operator!=(Iterator it) const;
        T &operator*();
        Iterator &operator++();
        Iterator operator++(int);

      private:
        node *ptr;
    };
    Iterator begin();
    Iterator end();

  private:
    node *head;
    node *tail;
    int size;
    Comp equal;
};

template <typename T, typename Comp>
const typename list<T, Comp>::node *list<T, Comp>::getTail() const {
    return tail;
}

template <typename T, typename Comp> T &list<T, Comp>::operator[](const int i) {
    auto temp = head;
    for (int j = 0; j < i && temp != nullptr; ++j) {
        temp = temp->next;
    }
    if (temp == nullptr) {
        throw std::out_of_range("Index got out of range\n");
    }
    return temp->data;
}

template <typename T, typename Comp>
T list<T, Comp>::operator[](const int i) const {
    auto temp = head;
    for (int j = 0; j < i && temp != nullptr; ++j) {
        temp = temp->next;
    }
    if (temp == nullptr) {
        throw std::out_of_range("Index got out of range\n");
    }
    return temp->data;
}

template <typename T, typename Comp>
const typename list<T, Comp>::node *list<T, Comp>::get_root() const {
    return head;
}

template <typename T, typename Comp>
list<T, Comp> &list<T, Comp>::operator=(const list &other) {
    if (this == &other)
        return *this;

    clear();

    node *temp = other.head;
    while (temp != nullptr) {
        append_back(temp->data);
        temp = temp->next;
    }
    return *this;
}

template <typename T, typename Comp>
list<T, Comp>::list(list<T, Comp> &&other) noexcept
    : head(other.head), tail(other.tail), size(other.size),
      equal(std::move(other.equal)) {
    other.head = nullptr;
    other.tail = nullptr;
    other.size = 0;
}

template <typename T, typename Comp>
list<T, Comp> &list<T, Comp>::operator=(list<T, Comp> &&other) noexcept {
    if (this == &other)
        return *this;

    clear();

    head = other.head;
    tail = other.tail;
    size = other.size;
    equal = std::move(other.equal);

    other.head = nullptr;
    other.tail = nullptr;
    other.size = 0;

    return *this;
}

template <typename T, typename Comp>
list<T, Comp>::list(const list &other)
    : head(nullptr), tail(nullptr), size(0), equal(other.equal) {
    auto temp = other.head;
    while (temp != nullptr) {
        append_back(temp.data);
        temp = temp->next;
    }
}

template <typename T, typename Comp> list<T, Comp>::~list() { this->clear(); }

template <typename T, typename Comp>
void list<T, Comp>::append_front(const T &var) {
    if (head == nullptr) {
        head = new node(var);
        tail = head;
        ++size;
    } else {
        auto temp = new node(var);
        temp->next = head;
        head->prev = temp;
        head = temp;
        ++size;
    }
}

template <typename T, typename Comp>
void list<T, Comp>::append_back(const T &var) {
    auto newNode = new node(var);
    if (head == nullptr) {
        head = tail = newNode;
    } else {
        newNode->prev = tail;
        tail->next = newNode;
        tail = newNode;
    }
    ++size;
}

template <typename T, typename Comp> bool list<T, Comp>::pop_front() {
    if (head == nullptr) {
        return false;
    }
    auto temp2 = head->data;
    if (head->prev == nullptr && head->next == nullptr) {
        delete tail;
        tail = nullptr;
        head = nullptr;
        size = 0;
        return true;
    }
    auto temp = head;
    head = head->next;
    head->prev = nullptr;
    delete temp;
    temp = nullptr;
    --size;
    return true;
}

template <typename T, typename Comp> bool list<T, Comp>::pop_back() {
    if (head == nullptr) {
        return false;
    }
    auto temp2 = tail->data;
    if (tail->prev == nullptr && tail->next == nullptr) {
        delete tail;
        tail = nullptr;
        head = nullptr;
        size = 0;
        return true;
    }
    auto temp = tail;
    tail = tail->prev;
    tail->next = nullptr;
    delete temp;
    temp = nullptr;
    --size;
    return true;
}

template <typename T, typename Comp>
const T &list<T, Comp>::get(int index) const {
    auto temp = head;
    for (int i = 0; i != index && temp != nullptr; ++i) {
        temp = temp->next;
    }
    if (temp == nullptr) {
        throw std::out_of_range("Funkcja get wykroczyla poza zakres listy");
    } else {
        return temp->data;
    }
}

template <typename T, typename Comp>
void list<T, Comp>::set(int index, T &value) {
    auto temp = head;
    for (int i = 0; i != index && temp != nullptr; ++i) {
        temp = temp->next;
    }
    if (temp != nullptr) {
        temp->data = value;
    }
}

template <typename T, typename Comp>
typename list<T, Comp>::node *list<T, Comp>::find(const T &key) const {
    auto temp = head;
    while (temp != nullptr) {
        if (equal(temp->data, key)) {
            return temp;
        }
        temp = temp->next;
    }
    return nullptr;
}

template <typename T, typename Comp> bool list<T, Comp>::pop(const T &key) {
    auto temp = find(key);
    if (temp == nullptr) {
        return false;
    }

    if (temp == head) {
        pop_front();
        return true;
    }
    if (temp == tail) {
        pop_back();
        return true;
    }
    if (temp->next->prev != nullptr)
        temp->next->prev = temp->prev;
    if (temp->prev->next != nullptr)
        temp->prev->next = temp->next;
    delete temp;
    temp = nullptr;
    --size;
    return true;
}

template <typename T, typename Comp>
void list<T, Comp>::bubbleSort(int (*data_cmp)(T, T)) {
    auto current = head;
    for (int i = 0; i < size - 1; ++i) {
        for (int j = 1; j < size; ++j) {
            if (data_cmp(current->data, current->next->data)) {
                auto temp = current->next;
                if (current == head) {
                    current->next = temp->next;
                    temp->prev = current->prev;
                    current->prev = temp;
                    temp->next = current;
                    head = temp;
                    if (head == tail) {
                        tail = current;
                        continue;
                    }
                    current->next->prev = current;
                } else if (current == tail->prev) {
                    temp->prev = current->prev;
                    current->prev = temp;
                    current->next = nullptr;
                    temp->next = current;
                    temp->prev->next = temp;
                    tail = current;
                } else {
                    current->next = temp->next;
                    temp->prev = current->prev;
                    current->next->prev = current;
                    temp->prev->next = temp;
                    temp->next = current;
                    current->prev = temp;
                }
            } else {
                current = current->next;
            }
        }
        current = head;
    }
}

template <typename T, typename Comp>
void list<T, Comp>::append_sort(const T &var, int (*data_cmp)(T, T)) {
    append_back(var);
    bubbleSort(data_cmp);
}

template <typename T, typename Comp> void list<T, Comp>::clear() {
    while (head != nullptr) {
        auto temp = head;
        head = head->next;
        delete temp;
    }
    head = nullptr;
    tail = nullptr;
    size = 0;
}

template <typename T, typename Comp>
std::string list<T, Comp>::to_string(int var) {
    auto current = head;
    std::ostringstream temp;
    for (int i = 0; current != nullptr && i < var;
         ++i, current = current->next) {
        temp << current->data << " ";
    }
    return temp.str();
}

template <typename T, typename Comp> int list<T, Comp>::get_size() const {
    return size;
}

template <typename T, typename Comp>
bool list<T, Comp>::Iterator::operator==(const list::Iterator it) const {
    return ptr == it.ptr;
}

template <typename T, typename Comp>
bool list<T, Comp>::Iterator::operator!=(const list::Iterator it) const {
    return ptr != it.ptr;
}

template <typename T, typename Comp>
typename list<T, Comp>::Iterator &list<T, Comp>::Iterator::operator++() {
    ptr = ptr->next;
    return *this;
}

template <typename T, typename Comp>
typename list<T, Comp>::Iterator list<T, Comp>::Iterator::operator++(int) {
    auto temp = *this;
    ++(*this);
    return temp;
}

template <typename T, typename Comp> T &list<T, Comp>::Iterator::operator*() {
    return ptr->data;
}

template <typename T, typename Comp>
typename list<T, Comp>::Iterator list<T, Comp>::begin() {
    return Iterator(head);
}

template <typename T, typename Comp>
typename list<T, Comp>::Iterator list<T, Comp>::end() {
    return Iterator(tail->next);
}

#endif
