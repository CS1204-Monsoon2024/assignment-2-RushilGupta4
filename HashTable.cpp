#include <iostream>
#include <vector>
#include <cmath>
#include <random>
#include <cstdlib>

using namespace std;

class HashTable {
private:
    vector<int*> table;
    int size;
    int count;
    float alpha = 0.8; // Load Factor

    int expmod(int base, int exp, int mod) {
        int result = 1;
        base %= mod;
        while (exp > 0) {
            if (exp & 1) result = (result * base) % mod;
            base = (base * base) % mod;
            exp >>= 1;
        }
        return result;
    }

    // Helper function for Rabin-Miller test
    bool millerTest(int d, int n) {
        int a = 2 + rand() % (n - 4);
        int x = expmod(a, d, n);
        if (x == 1 || x == n - 1) return true;
        while (d != n - 1) {
            x = (x * x) % n;
            d *= 2;
            if (x == 1) return false;
            if (x == n - 1) return true;
        }
        return false;
    }

    bool isPrime(int n, int k = 5) {
        if (n <= 1 || n == 4) return false;
        if (n <= 3) return true;

        int d = n - 1;
        while (d % 2 == 0) d /= 2;

        for (int i = 0; i < k; i++) {
            if (!millerTest(d, n)) return false;
        }

        return true;
    }

    int getNextPrime(int n) {
        while (!isPrime(n)) {
            n++;
        }

        return n;
    }

    int hash(int key) {
        return key % size;
    }

    void resize() {
        int newSize = getNextPrime(size * 2);
        vector<int*> oldTable = table;
        table = vector<int*>(newSize, nullptr);
        size = newSize;
        count = 0;

        for (int* item : oldTable) {
            if (item != nullptr) {
                insert(*item);
                delete item;
            }
        }
    }

public:
    HashTable(int initialSize) {
        size = getNextPrime(initialSize);
        table = vector<int*>(size, nullptr);
        count = 0;
    }

    void insert(int key) {
        if (count >= alpha * size) {
            resize();
        }

        int index = hash(key);
        int i = 0;

        while (i < size) {
            int probeIndex = (index + i * i) % size;
            if (table[probeIndex] == nullptr) {
                table[probeIndex] = new int(key);
                count++;
                return;
            } else if (*table[probeIndex] == key) {
                cout << "Duplicate key insertion is not allowed" << endl;
                return;
            }
            i++;
        }

        cout << "Max probing limit reached!" << endl;
    }

    void remove(int key) {
        int index = search(key);
        if (index != -1) {
            delete table[index];
            table[index] = nullptr;
            count--;
        } else {
            cout << "Element not found" << endl;
        }
    }

    int search(int key) {
        int index = hash(key);
        int i = 0;

        while (i < size) {
            int probeIndex = (index + i * i) % size;
            if (table[probeIndex] == nullptr) {
                return -1;
            }
            if (*table[probeIndex] == key) {
                return probeIndex;
            }
            i++;
        }

        return -1;
    }

    void printTable() {
        for (int i = 0; i < size; i++) {
            if (table[i] == nullptr) {
                cout << "- ";
            } else {
                cout << *table[i] << " ";
            }
        }
        cout << endl;
    }

    // Add destructor to free allocated memory
    ~HashTable() {
        for (int* item : table) {
            delete item;
        }
    }
};
