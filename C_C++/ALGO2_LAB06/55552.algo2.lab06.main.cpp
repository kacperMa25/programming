// ALGO2 IS1 223A LAB06
// Kacper Małeck
// mk55552@zut.edu.pl

#include "../LAB01/lista.h"
#include "../LAB02/DynamicArray.h"
#include <chrono>
#include <iostream>
#include <random>
#include <thread>
#include <utility>

template <typename K, typename V> class HashTable {
  public:
    struct Record {
        explicit Record(K key = "", V value = 0)
            : key_(std::move(key)), value_(value) {}
        Record(const Record &other) : key_(other.key_), value_(other.value_) {}
        Record(Record &&other) noexcept
            : key_(std::move(other.key_)), value_(std::move(other.value_)) {}
        bool operator==(const Record &other) const {
            return key_ == other.key_;
        }
        K key_;
        V value_;
    };
    HashTable() : table{}, size(0) {}
    ~HashTable() { clear(); }
    void push(const K &key, const V &value);
    bool pop(const K &key);
    void clear();

    static unsigned long int hash(const std::string &key);
    bool contains(const K &key) const;

    bool contains(const K &key, int hashKey) const;

    [[nodiscard]] int getSize() const;
    [[nodiscard]] int getCapacity() const;
    [[nodiscard]] int nonEmptyLists() const;
    [[nodiscard]] int longestList() const;
    [[nodiscard]] int shortestList() const;
    [[nodiscard]] float averageSize() const;
    void showStats() const;
    [[nodiscard]] std::string toString() const;

    Record &operator[](const K &key);
    const Record &operator[](const K &key) const;
    HashTable &operator=(const HashTable &other);
    void reHash();

    using List = list<Record, std::equal_to<>>;
    using Table = DynamicArray<List, std::less<>>;

  private:
    const float MAX_LOAD_FACTOR{0.75};
    Table table;
    int size;
};

template <typename K, typename V> void HashTable<K, V>::reHash() {
    Table newHashTable(table.getSize() * 2, List{});

    for (int i = 0; i < table.getSize(); ++i) {
        auto temp = table[i].get_root();
        while (temp != nullptr) {
            unsigned int hashKey =
                hash(temp->data.key_) % newHashTable.getSize();
            newHashTable[hashKey].append_back(
                Record(temp->data.key_, temp->data.value_));
            temp = temp->next;
        }
    }
    table = std::move(newHashTable);
}

template <typename K, typename V>
const typename HashTable<K, V>::Record &
HashTable<K, V>::operator[](const K &key) const {
    int hashKey = hash(key);
    auto node = table[hashKey].find(Record(key));
    if (node != nullptr)
        return node->data;
    throw std::out_of_range("Klucz nie istnieje");
}

template <typename K, typename V>
HashTable<K, V> &HashTable<K, V>::operator=(const HashTable &other) {
    table = other.table;
    return *this;
}

template <typename K, typename V>
typename HashTable<K, V>::Record &HashTable<K, V>::operator[](const K &key) {
    int hashKey = hash(key) % table.getSize();
    auto node = table[hashKey].find(Record(key));
    if (node != nullptr)
        return node->data;
    throw std::out_of_range("Klucz nie istnieje");
}

template <typename K, typename V> int HashTable<K, V>::getSize() const {
    return size;
}

template <typename K, typename V> int HashTable<K, V>::getCapacity() const {
    return table.getCapacity();
}

template <typename K, typename V>
bool HashTable<K, V>::contains(const K &key) const {
    int hashKey = hash(key) % table.getSize();
    if (table[hashKey].find(Record(key)) != nullptr)
        return true;
    return false;
}

template <typename K, typename V>
bool HashTable<K, V>::contains(const K &key, int hashKey) const {
    if (table[hashKey].find(Record(key)) != nullptr)
        return true;
    return false;
}

template <typename K, typename V> int HashTable<K, V>::nonEmptyLists() const {
    int counter = 0;
    for (int i = 0; i < table.getSize(); ++i) {
        if (table[i].get_size() != 0)
            counter++;
    }
    return counter;
}

template <typename K, typename V> int HashTable<K, V>::longestList() const {
    int maxSize = 0;
    for (int i = 0; i < table.getSize(); ++i) {
        if (table[i].get_size() > maxSize)
            maxSize = table[i].get_size();
    }
    return maxSize;
}

template <typename K, typename V> int HashTable<K, V>::shortestList() const {
    int minSize = 0;
    for (int i = 0; i < table.getSize(); ++i) {
        if (table[i].get_size() < minSize)
            minSize = table[i].get_size();
    }
    return minSize;
}

template <typename K, typename V> float HashTable<K, V>::averageSize() const {
    float avg = 0;
    for (int i = 0; i < table.getSize(); ++i) {
        avg += table[i].get_size();
    }
    return avg / table.getSize();
}

template <typename K, typename V> void HashTable<K, V>::showStats() const {
    std::cout << "Size: " << size << std::endl
              << "Capacity: " << table.getCapacity() << std::endl
              << "Non-empty lists: " << nonEmptyLists() << std::endl
              << "Longest list: " << longestList() << std::endl
              << "Shortest list: " << shortestList() << std::endl
              << "Average size: " << averageSize() << std::endl;
}

template <typename K, typename V>
std::string HashTable<K, V>::toString() const {
    std::ostringstream os;
    os << "Hash table: {" << std::endl;
    for (int i = 0, visited = 0; i < table.getSize(); ++i) {
        const typename List::node *current = table[i].get_root();
        if (table[i].get_size() > 0)
            os << "\t" << i << ": ";
        while (current != nullptr) {
            os << current->data.key_ << " " << current->data.value_ << ", ";
            current = current->next;
            visited++;
        }
        if (table[i].get_size() > 0)
            os << std::endl;
        if (visited > 10) {
            os << "\t" << "..." << std::endl;
            break;
        }
    }
    os << "}" << std::endl;
    return os.str();
}

template <typename K, typename V>
unsigned long int HashTable<K, V>::hash(const std::string &key) {
    unsigned long long int hashKey = 0;
    int power = 1;
    for (int i = key.length() - 1; i >= 0; --i) {
        hashKey += key[i] * power;
        power *= 31;
    }
    return hashKey;
}

template <typename K, typename V>
void HashTable<K, V>::push(const K &key, const V &value) {
    if (table.getSize() == 0) {
        table.pushBack(List{});
        table.pushBack(List{});
    }
    if (static_cast<double>(getSize() + 1) / table.getSize() >
        MAX_LOAD_FACTOR) {
        reHash();
    }
    unsigned int hashKey = hash(key) % table.getSize();
    if (contains(key, hashKey))
        return;
    table[hashKey].append_back(Record(key, value));
    ++size;
}

template <typename K, typename V> bool HashTable<K, V>::pop(const K &key) {
    int hashKey = hash(key) % table.getSize();
    bool hasBeenDeleted = table[hashKey].pop(Record(key));
    if (hasBeenDeleted == true) {
        --size;
    }
    return hasBeenDeleted;
}

template <typename K, typename V> void HashTable<K, V>::clear() {
    table.clear();
    size = 0;
}

std::random_device rd;
std::mt19937 gen(rd());
std::uniform_int_distribution<> dist(0, std::numeric_limits<int>::max());

static char alphaNum[] = "abcdefghijklmnopqrstuvwxyz";

int alphaNumLength = sizeof(alphaNum) - 1;

std::string getRandomKey() {
    std::string randomKey;
    for (int i = 0; i < 6; ++i) {
        randomKey += alphaNum[dist(gen) % alphaNumLength];
    }
    return randomKey;
}

int main() {
    HashTable<std::string, int> hashTable;

    const int MAX_ORDER = 30;
    for (int i = 0; i < MAX_ORDER; ++i) {
        int n = std::pow(2, i);
        int hits = 0;

        auto begin = std::chrono::high_resolution_clock::now();
        for (int j = 0; j < n; ++j) {
            hashTable.push(getRandomKey(), dist(gen));
        }
        auto end = std::chrono::high_resolution_clock::now();
        auto duration =
            std::chrono::duration_cast<std::chrono::milliseconds>(end - begin);
        std::cout << hashTable.toString() << std::endl;

        hashTable.showStats();

        std::cout << "Creating Hash Table took: "
                  << static_cast<double>(duration.count()) / n << " ms"
                  << std::endl;

        begin = std::chrono::high_resolution_clock::now();
        int m = std::pow(10, 5);
        for (int j = 0; j < m; ++j) {
            if (hashTable.contains(getRandomKey()))
                hits++;
        }
        end = std::chrono::high_resolution_clock::now();
        duration =
            std::chrono::duration_cast<std::chrono::milliseconds>(end - begin);

        std::cout << "Finding lasted: "
                  << static_cast<double>(duration.count()) / 1000 << " s"
                  << std::endl
                  << "Number of hits: " << hits << std::endl
                  << "==========================================\n\n";
        hashTable.clear();
        std::this_thread::sleep_for(std::chrono::seconds(3));
    }
}
