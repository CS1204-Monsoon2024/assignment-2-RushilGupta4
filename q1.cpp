#include <iostream>
#include <cmath>

using namespace std;

class Node {
public:
    int value;
    Node* next;
    Node(int val) : value(val), next(nullptr) {}
};

class LinkedList {
private:
    Node* head;
    long long hash;
    int size;
    const int m = 1000000001;
    const float A = 0.4; // A float between 0 and 1
    const int B = 1238123; // Random Constant Number

    int hashFunction(int value) {
        // Simple hash function, as discussed in class
        float scaled = A * value;
        int floored = floor(scaled);
        return (floored + B) % m;
    }

    long long expmod(long long base, int exp, int mod) {
        long long result = 1;
        base %= mod;
        while (exp > 0) {
            if (exp & 1) result = (result * base) % mod;
            base = (base * base) % mod;
            exp >>= 1;
        }
        return result;
    }

public:
    LinkedList() : head(nullptr), hash(0), size(0) {}

    void insert(int value) {
        Node* newNode = new Node(value);
        if (!head) {
            head = newNode;
        } else {
            Node* current = head;
            while (current->next) {
                current = current->next;
            }
            current->next = newNode;
        }

        long long h = hashFunction(value);
        hash = (hash + h * expmod(m, size, m)) % m;
        size++;
    }

    void remove(int value) {
        if (!head) return;

        if (head->value == value) {
            // Case 1: The value is at the head of the list
            Node* temp = head;
            head = head->next;
            delete temp;
            recalculateHash();
            size--;
            return;
        }

        Node* current = head;
        Node* prev = nullptr;
        while (current && current->value != value) {
            prev = current;
            current = current->next;
        }

        if (current) {
            prev->next = current->next;
            delete current;
            recalculateHash();
            size--;
        }
    }

    void recalculateHash() {
        hash = 0;
        Node* current = head;
        int index = 0;
        while (current) {
            long long h = hashFunction(current->value);
            hash = (hash + h * expmod(m, size, m)) % m;
            current = current->next;
            index++;
        }
    }

    bool isEquivalent(const LinkedList& other) const {
        if (hash != other.hash || size != other.size) {
            return false;
        }

        Node* current1 = head;
        Node* current2 = other.head;
        while (current1 && current2) {
            if (current1->value != current2->value) {
                return false;
            }
            current1 = current1->next;
            current2 = current2->next;
        }

        return true;
    }

};


