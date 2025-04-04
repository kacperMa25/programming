//ALGO2 IS1 223A LAB03
//Kacper Małecki
//mk55552@zut.edu.pl

#include <iostream>
#include <fstream>
#include <sstream>
#include <random>
#include "C:\Users\asasy\CLionProjects\LAB01\lista.h"
#include <chrono>
#include <thread>

struct Less{
    bool operator()(int a, int b) const{
        return a < b;
    }
};

template <typename T, typename Comp>
class BST {
public:
    struct Node {
        explicit Node(T var) : value(var), left(nullptr), right(nullptr){}
        T value;
        Node* left;
        Node* right;
        Node* parent;
    };

    explicit BST(Comp cmp = {}) : root(nullptr), size(0), cmp(cmp){}
    ~BST(){
        clear();
    }
    void push(const T& var);
    Node* find(const T& var) const;
    void clear();
    void pop(const T& var);
    Node* findPredecessor(BST<T, Comp>::Node* var) const;
    const Node* getRoot() const;
    int getHeight(const Node* root_) const;
    int getSize() const;
    std::string toDot() const;
    void printToFile(const std::string& path) const;
    void inOrderTravel(const Node* node, list<T>& lista) const;
    void preOrderTravel(const Node* node, list<T>& lista) const;

    friend std::ostream& operator<<(std::ostream& os, const Node* node) {
        if (node == nullptr) return os;
        os << node->value << ";\n";
        if (node->left != nullptr) os << node->value << " -> " << node->left->value << " [label=\"L\"];\n";
        if (node->right != nullptr) os << node->value << " -> " << node->right->value << " [label=\"R\"];\n";
        os << node->left;
        os << node->right;

        return os;
    }
private:
    Node* root;
    int size;
    Comp cmp;

};

template <typename T, typename Comp>
void BST<T, Comp>::preOrderTravel(const Node* node, list<T>& lista) const{
    if(node == nullptr) return;
    preOrderTravel(node->left, lista);
    preOrderTravel(node->right, lista);
    lista.append_back(node->value);
}


template <typename T, typename Comp>
void BST<T, Comp>::inOrderTravel(const Node* node, list<T>& lista) const {
    if(node == nullptr) return;
    inOrderTravel(node->left, lista);
    lista.append_back(node->value);
    inOrderTravel(node->right, lista);
}

template <typename T, typename Comp>
void BST<T, Comp>::printToFile(const std::string& path) const {
    std::ofstream file(path);
    if(!file.is_open()){
        std::cerr << "Couldn't open file: " << path << std::endl;
        return;
    }

    file << toDot();
    file.close();
}

template <typename T, typename Comp>
std::string BST<T, Comp>::toDot() const {
    std::ostringstream os;
    os << "digraph BST {\n"
                 "label=\"size= " << getSize() <<", height=" << getHeight(getRoot()) << "\"\n"
              << "node [shape=circle; fixedsize=true];\n"
              << getRoot()
              << "}\n";
    return os.str();
}

template<typename T, typename Comp>
int BST<T, Comp>::getSize() const {
    return size;
}

template <typename T, typename Comp>
void BST<T, Comp>::push(const T &var) {
    if(root == nullptr){
        root = new Node(var);
        root->parent = nullptr;
        ++size;
    }
    else {
        auto temp = root;
        while(temp != nullptr){
            if(cmp(var, temp->value)){
                if(temp->left == nullptr){
                    temp->left = new Node(var);
                    temp->left->parent = temp;
                    ++size;
                    break;
                }
                else{
                    temp = temp->left;
                }
            }
            else if(cmp(temp->value, var)){
                if(temp->right == nullptr){
                    temp->right = new Node(var);
                    temp->right->parent = temp;
                    ++size;
                    break;
                }
                else{
                    temp = temp->right;
                }
            }
            else break;
        }
    }
}

template <typename T, typename Comp>
BST<T, Comp>::Node* BST<T, Comp>::find(const T &var) const{
    auto temp = root;
    while(temp != nullptr){
        if(cmp(var, temp->value)) temp = temp->left;
        else if(cmp(temp->value, var)) temp = temp->right;
        else return temp;
    }
    return nullptr;
}

template <typename T, typename Comp>
void BST<T, Comp>::clear() {
    while(root != nullptr){
        pop(root->value);
    }
    size = 0;
}

template <typename T, typename Comp>
void BST<T, Comp>::pop(const T &var) {
    auto temp = find(var);
    if (temp == nullptr) {
        return;
    }

    if (temp == root) { // If the removed element is root
        if (!root->left && !root->right) { // No children
            root = nullptr;
        } else if (!root->left) { // Only right child
            root = root->right;
            root->parent = nullptr;
        } else if (!root->right) { // Only left child
            root = root->left;
            root->parent = nullptr;
        } else { // Both children
            auto predecessor = findPredecessor(temp);
            if (predecessor->parent == temp) { // Predecessor is a child of root
                predecessor->right = root->right;
                root->right->parent = predecessor;
            } else { // Predecessor is not a direct child
                predecessor->parent->right = predecessor->left;
                if (predecessor->left) {
                    predecessor->left->parent = predecessor->parent;
                }
                predecessor->left = root->left;
                root->left->parent = predecessor;
                predecessor->right = root->right;
                root->right->parent = predecessor;
            }
            root = predecessor;
            root->parent = nullptr;
        }
    } else { // If removed node is not the root
        if (!temp->left && !temp->right) { // No children
            if (temp->parent->left == temp) temp->parent->left = nullptr;
            else temp->parent->right = nullptr;
        } else if (!temp->left) { // Only right child
            if (temp->parent->left == temp) temp->parent->left = temp->right;
            else temp->parent->right = temp->right;
            temp->right->parent = temp->parent;
        } else if (!temp->right) { // Only left child
            if (temp->parent->left == temp) temp->parent->left = temp->left;
            else temp->parent->right = temp->left;
            temp->left->parent = temp->parent;
        } else { // Both children
            auto predecessor = findPredecessor(temp);
            if (predecessor == temp->left) { // Predecessor is a direct child
                if (temp->parent->left == temp) temp->parent->left = predecessor;
                else temp->parent->right = predecessor;
                predecessor->parent = temp->parent;
                predecessor->right = temp->right;
                temp->right->parent = predecessor;
            } else { // Predecessor is not a direct child
                predecessor->parent->right = predecessor->left;
                if (predecessor->left) {
                    predecessor->left->parent = predecessor->parent;
                }
                predecessor->left = temp->left;
                temp->left->parent = predecessor;
                predecessor->right = temp->right;
                temp->right->parent = predecessor;

                if (temp->parent->left == temp) temp->parent->left = predecessor;
                else temp->parent->right = predecessor;
                predecessor->parent = temp->parent;
            }
        }
    }
    delete temp;
    --size;
}


template <typename T, typename Comp>
BST<T, Comp>::Node* BST<T, Comp>::findPredecessor(BST<T, Comp>::Node* var) const {
    auto temp = var;
    if(temp == nullptr || temp->left == nullptr) return nullptr;
    temp = temp->left;
    while(temp->right != nullptr){
        temp = temp->right;
    }
    return temp;
}


template <typename T, typename Comp>
const BST<T, Comp>::Node* BST<T,Comp>::getRoot() const{
    return root;
}


template <typename T, typename Comp>
int BST<T, Comp>::getHeight(const BST::Node *root_) const {
    if(root_ == nullptr) return 0;

    int leftHeight = getHeight(root_->left);
    int rightHeight = getHeight(root_->right);

    return std::max(leftHeight, rightHeight) + 1;
}


bool less(int a, int b){
    return a < b;
}

void test1(){
    BST<int, Less> tree;
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dist(0, INT32_MAX);

    using namespace std::chrono;

    const int MAX_ORDER = 100;
    for(int i = 2; i < MAX_ORDER; ++i){
        const int n = std::pow(2, i);

        auto begin = high_resolution_clock::now();
        for(int j = 0; j < n; ++j){
            tree.push(dist(gen));
        }
        auto end = high_resolution_clock::now();
        //tree.printToFile("output.dot");

        auto duration = duration_cast<milliseconds>(end - begin);
        std::cout << "Size: " << tree.getSize() << std::endl
                  << "Height: " << tree.getHeight(tree.getRoot()) << std::endl
                  << "Creating tree took: " << static_cast<double>(duration.count()) / 1000 << " s" << std::endl
                  << "Avg. time of inserting single element: " << static_cast<double>(duration.count()) / n << " ms" << std::endl;

        begin = high_resolution_clock::now();
        for(int j = 0; j < 100000000; ++j){
            tree.find(dist(gen));
        }
        end = high_resolution_clock::now();

        duration = duration_cast<milliseconds>(end - begin);
        std::cout << "Finding random element took: " << static_cast<double>(duration.count()) / 1000 << " s" << std::endl
                  << "Avg. time of looking for random element: " << static_cast<double>(duration.count()) / 10000000 << " ms" << std::endl;

        std::cout << "====================" << std::endl;
        tree.clear();
        //std::this_thread::sleep_for(seconds(5));
    }
}

int main() {
    test1();
}