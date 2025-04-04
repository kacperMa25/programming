//ALGO2 IS1 223A LAB01
//Kacper Małecki
//mk55552@zut.edu.pl

#include <iostream>
#include <cmath>
#include <chrono>
#include <random>
#include <string>
#include <sstream>

template <typename T>
class list {
private:
    class node{
    public:
        T data;
        node* next;
        node* prev;
        explicit node(T var) : data(var), next(nullptr), prev(nullptr) {}
    };
    node* head;
    node* tail;
    int size;

public:
    list() : head(nullptr), tail(nullptr), size(0){}
    ~list();
    void append_front(const T& var);
    void append_back(const T& var);
    T pop_front();
    T pop_back();
    const T& get(int index) const;
    void set(int index, T& value);
    node* find(const T& key, int (*cmp)(T, T)) const;
    bool pop(const T& key, int (*cmp)(T, T));
    void sort(int (*comp)(T, T));
    void append_sort(const T& var, int (*comp)(T, T));
    void clear();
    std::string to_string(int var);
    int get_size() const;

    class Iterator {
    public:
        friend class list<T>;
        explicit Iterator(node* ptr_) : ptr(ptr_){};
        bool operator==(Iterator it) const;
        bool operator!=(Iterator it) const;
        T& operator*();
        Iterator& operator++();
        Iterator operator++(int);
    private:
        node* ptr;
    };
    Iterator begin();
    Iterator end();
};


template <typename T>
list<T>::~list() {
    this->clear();
}

template <typename T>
void list<T>::append_front(const T& var) {
    if (this->head == nullptr){
        head = new node(var);
        tail = head;
        ++size;
    }
    else {
        auto temp = new node(var);
        temp->next = head;
        head->prev = temp;
        head = temp;
        ++size;
    }
}

template <typename T>
void list<T>::append_back(const T& var) {
    if(head == nullptr){
        head = new node(var);
        tail = head;
        ++size;
    }
    else{
        auto temp = new node(var);
        temp->prev = tail;
        tail->next = temp;
        tail = temp;
        ++size;
    }
}

template <typename T>
T list<T>::pop_front(){
    if(head == nullptr){
        return false;
    }
    auto temp2 = head->data;
    if (head->prev == nullptr && head->next == nullptr){
        delete tail;
        tail = nullptr;
        head = nullptr;
        size = 0;
        return temp2;
    }
    auto temp = head;
    head = head->next;
    head->prev = nullptr;
    delete temp;
    temp = nullptr;
    --size;
    return temp2;
}

template <typename T>
T list<T>::pop_back(){
    if(head == nullptr){
        return false;
    }
    auto temp2 = tail->data;
    if (tail->prev == nullptr && tail->next == nullptr){
        delete tail;
        tail = nullptr;
        head = nullptr;
        size = 0;
        return temp2;
    }
    auto temp = tail;
    tail = tail->prev;
    tail->next = nullptr;
    delete temp;
    temp = nullptr;
    --size;
    return temp2;
}

template <typename T>
const T& list<T>::get(int index) const{
    auto temp = head;
    for(int i = 0; i != index && temp != nullptr; ++i){
        temp = temp->next;
    }
    if(temp == nullptr){
        throw std::out_of_range("Funkcja get wykroczyla poza zakres listy");
    }
    else{
        return temp->data;
    }
}

template <typename T>
void list<T>::set(int index, T& value){
    auto temp = head;
    for(int i = 0; i != index && temp != nullptr; ++i){
        temp = temp->next;
    }
    if(temp != nullptr){
        temp->data = value;
    }
}

template <typename T>
typename list<T>::node* list<T>::find(const T& key, int (*cmp)(T, T)) const{
    auto temp = head;
    while(temp != nullptr){
        if(cmp(temp->data, key)){
            return temp;
        }
        temp = temp->next;
    }
    return NULL;
}

template <typename T>
bool list<T>::pop(const T& key, int (*cmp)(T, T)){
    auto temp = find(key, cmp);
    if(temp == head){
        pop_front();
        return true;
    }
    else if(temp == tail){
        pop_back();
        return true;
    }
    if(temp != nullptr) {
        temp->next->prev = temp->prev;
        temp->prev->next = temp->next;
        delete temp;
        temp = nullptr;
        --size;
        return true;
    }
    return false;
}

template<typename T>
void list<T>::sort(int (*comp)(T, T)){
    auto current = head;
    for(int i = 0; i < size - 1; ++i){
        for(int j = 1; j < size; ++j ){
            if(comp(current->data, current->next->data)){
                auto temp = current->next;
                if(current == head) {
                    current->next = temp->next;
                    temp->prev = current->prev;
                    current->prev = temp;
                    temp->next = current;
                    head = temp;
                    if(head == tail) {
                        tail = current;
                        continue;
                    }
                    current->next->prev = current;
                }
                else if(current == tail->prev){
                    temp->prev = current->prev;
                    current->prev = temp;
                    current->next = nullptr;
                    temp->next = current;
                    temp->prev->next = temp;
                    tail = current;
                }
                else {
                    current->next = temp->next;
                    temp->prev = current->prev;
                    current->next->prev = current;
                    temp->prev->next = temp;
                    temp->next = current;
                    current->prev = temp;
                }
            }
            else{
                current = current->next;
            }
        }
        current = head;
    }
}

template <typename T>
void list<T>::append_sort(const T& var, int (*comp)(T, T)){
    append_back(var);
    sort(comp);
}

template <typename T>
void list<T>::clear(){
    while(this->head != nullptr) {
        pop_front();
    }
}

template <typename T>
std::string list<T>::to_string(int var){
    auto current = head;
    std::string output;
    std::ostringstream temp;
    for(int i = 0; current != nullptr && i < var; ++i, current = current->next){
        temp << current->data << " ";
    }
    output = temp.str();
    return output;
}

template <typename T>
int list<T>::get_size() const{
    return size;
}


template <typename T>
bool list<T>::Iterator::operator==(const list::Iterator it) const {
    return ptr == it.ptr;
}

template <typename T>
bool list<T>::Iterator::operator!=(const list::Iterator it) const {
    return ptr != it.ptr;
}

template <typename T>
typename list<T>::Iterator& list<T>::Iterator::operator++() {
    ptr = ptr->next;
    return *this;
}

template<typename T>
typename list<T>::Iterator list<T>::Iterator::operator++(int) {
    auto temp = *this;
    ++(*this);
    return temp;
}


template<typename T>
T& list<T>::Iterator::operator*() {
    return ptr->data;
}

template<typename T>
typename list<T>::Iterator list<T>::begin() {
    return Iterator(head);
}

template<typename T>
typename list<T>::Iterator list<T>::end() {
    return Iterator(tail->next);
}

int less(int a, int b){
    return a > b;
}

int equal(int a, int b){
    return a == b;
}

int main() {
    const int MAX_ORDER = 100;
    list<int> lista;

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(0, 9999);

    for(int i = 0; i < MAX_ORDER; ++i){
        const int n = pow(2, i);

        auto begin = std::chrono::steady_clock::now();
        for(int j = 0; j < n; ++j){
            lista.append_back(dis(gen));
        }
        auto end = std::chrono::steady_clock::now();

        auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - begin);
        std::cout << "Rozmiar listy: " << lista.get_size() << std::endl
                  << "Czas trwania dodawania: " << static_cast<double>(duration.count()) / 1000 << " sekund(y)" << std::endl;

        begin = std::chrono::steady_clock::now();
        const int m = pow(2, i);
        for(int j = 0; j < m; ++j){
            lista.find(dis(gen), equal);
        }
        end = std::chrono::steady_clock::now();

        duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - begin);
        std::cout << "Czas trwania wyszukania: " << static_cast<double>(duration.count()) / 1000 << " sekund(y)" << std::endl << std::endl;

        lista.clear();
    }
}