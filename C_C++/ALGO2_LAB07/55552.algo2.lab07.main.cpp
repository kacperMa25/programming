// ALGO2 IS1 223A LAB07
// Kacper Małeck
// mk55552@zut.edu.pl

#include "../LAB01/lista.h"
#include "../LAB05/Heap.h"
#include <chrono>
#include <cmath>
#include <iostream>
#include <random>
#include <thread>

void countingSort(DynamicArray<int, std::less<>> &arr, int domainMax) {
  DynamicArray<int, std::less<>> counter(domainMax, 0);
  for (int i = 0; i < arr.getSize(); i++) {
    counter[arr[i]]++;
  }
  for (int i = 1; i < domainMax; i++) {
    counter[i] += counter[i - 1];
  }
  DynamicArray<int, std::less<>> sorted(arr.getSize());
  for (int i = arr.getSize() - 1; i >= 0; i--) {
    sorted[(counter[arr[i]]--) - 1] = arr[i];
  }
  arr = std::move(sorted);
}

template <typename T, typename Comp>
void bucketSort(DynamicArray<T, std::less<>> &arr, int domainMax,
                int (*cmp)(T, T) = less) {
  int width = std::ceil(static_cast<double>(domainMax) / arr.getSize());
  DynamicArray<list<int, std::equal_to<>>, Comp> buckets(
      domainMax / width, list<int, std::equal_to<>>{});
  for (int i = 0; i < arr.getSize(); i++) {
    buckets[arr[i] / width].append_sort(arr[i], cmp);
  }

  for (int i = 0, j = 0; i < arr.getSize(); ++j) {
    auto temp = buckets[j].get_root();
    while (temp != nullptr) {
      arr[i] = temp->data;
      temp = temp->next;
      i++;
    }
  }
}

void bucketSort(DynamicArray<int, std::less<>> &arr, int domainMax) {
  int width = std::ceil(static_cast<double>(domainMax) / arr.getSize());
  DynamicArray<list<int, std::equal_to<>>, std::less<>> buckets(
      std::ceil(static_cast<double>(domainMax) / width),
      list<int, std::equal_to<>>{});
  for (int i = 0; i < arr.getSize(); i++) {
    buckets[arr[i] / width].append_sort(arr[i], less);
  }

  for (int i = 0, j = 0; i < arr.getSize(); ++j) {
    auto temp = buckets[j].get_root();
    while (temp != nullptr) {
      arr[i] = temp->data;
      temp = temp->next;
      i++;
    }
  }
}

constexpr int m = static_cast<int>(std::pow(10, 7));
constexpr int MAX_ORDER = 30;

std::random_device rd;
std::mt19937 gen(rd());
std::uniform_int_distribution<> integers(0, m - 1);
std::uniform_real_distribution<> reals(0, m - 1);

static char alphaNum[] = "abcdefghijklmnopqrstuvwxyz";

int alphaNumLength = sizeof(alphaNum) - 1;

std::string getRandomKey() {
  std::string randomKey;
  for (int i = 0; i < 6; ++i) {
    randomKey += alphaNum[integers(gen) % alphaNumLength];
  }
  return randomKey;
}

void test_ints() {

  for (int i = 10; i < MAX_ORDER; i++) {
    int n = static_cast<int>(std::pow(2, i));

    std::cout << "Size: " << n << std::endl;

    DynamicArray<int, std::less<>> arr1(n);
    for (int j = 0; j < n; j++) {
      arr1[j] = integers(gen);
    }

    DynamicArray arr2(arr1);
    DynamicArray arr3(arr1);

    std::cout << "Unsorted: " << arr1.shortRep() << std::endl;

    auto begin = std::chrono::high_resolution_clock::now();
    countingSort(arr1, m);
    auto end = std::chrono::high_resolution_clock::now();
    auto duration =
        std::chrono::duration_cast<std::chrono::milliseconds>(end - begin);

    std::cout << "Counting sort: "
              << static_cast<double>(duration.count()) / 1000 << " seconds"
              << std::endl;

    begin = std::chrono::high_resolution_clock::now();
    Heap heap(arr2);
    heap.heapSort();
    end = std::chrono::high_resolution_clock::now();
    duration =
        std::chrono::duration_cast<std::chrono::milliseconds>(end - begin);

    std::cout << "Heap sort: " << static_cast<double>(duration.count()) / 1000
              << " seconds" << std::endl;

    begin = std::chrono::high_resolution_clock::now();
    bucketSort(arr3, m);
    end = std::chrono::high_resolution_clock::now();
    duration =
        std::chrono::duration_cast<std::chrono::milliseconds>(end - begin);

    std::cout << "Bucket sort: " << static_cast<double>(duration.count()) / 1000
              << " seconds" << std::endl;
    std::cout << "Same?: " << (arr1 == arr3) << std::endl;
    std::cout << "Sorted: " << heap.shortRep() << std::endl;

    std::cout << "==============================================" << std::endl;
    std::this_thread::sleep_for(std::chrono::seconds(3));
  }
}

struct Person {
  friend std::ostream &operator<<(std::ostream &os, const Person &person) {
    os << "Name: " << person.name << std::endl;
    os << "Age: " << person.age << std::endl;
    return os;
  }

  Person(std::string name_, int age_) {
    name = name_;
    age = age_;
  }
  std::string name;
  int age;
};

// void test_obj() {
//
//     for (int i = 10; i < MAX_ORDER; i++) {
//         int n = static_cast<int>(std::pow(2, i));
//
//         std::cout << "Size: " << n << std::endl;
//
//         DynamicArray<Person, std::less<>> arr1(n);
//         for (int j = 0; j < n; j++) {
//             arr1[j] = Person(getRandomKey() ,integers(gen));
//         }
//
//         DynamicArray arr2(arr1);
//         DynamicArray arr3(arr1);
//
//         auto begin = std::chrono::high_resolution_clock::now();
//         Heap heap(arr2);
//         heap.heapSort();
//         auto end = std::chrono::high_resolution_clock::now();
//         auto duration =
//         std::chrono::duration_cast<std::chrono::milliseconds>(end - begin);
//
//         std::cout << "Heap sort: " << static_cast<double>(duration.count()) /
//         1000 << " seconds" << std::endl;
//
//         begin = std::chrono::high_resolution_clock::now();
//         bucketSort(arr3, m);
//         end = std::chrono::high_resolution_clock::now();
//         duration = std::chrono::duration_cast<std::chrono::milliseconds>(end
//         - begin);
//
//         std::cout << "Bucket sort: " << static_cast<double>(duration.count())
//         / 1000 << " seconds" << std::endl; std::cout << "Same?: " << (arr1 ==
//         arr3) << std::endl; std::cout << "Sorted: " << heap.shortRep() <<
//         std::endl;
//
//         std::cout << "==============================================" <<
//         std::endl; std::this_thread::sleep_for(std::chrono::seconds(3));
//     }
// }

int main() { test_ints(); }
