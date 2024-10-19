#include "HashTable.cpp"
#include <iostream>

int main() {
    int initialSize = 5; 
    HashTable ht(initialSize);

    // Example test case
    ht.insert(1);
    ht.printTable();
    ht.insert(6);
    ht.printTable();
    ht.insert(15);
    ht.printTable(); 
    ht.insert(25);
    ht.printTable();
    ht.remove(15);
    ht.printTable();
    ht.insert(29);  
    ht.printTable(); 
    ht.insert(22);  
    ht.printTable();
    int find = ht.search(22);
    std::cout << "Found at:" << find << std::endl;

    return 0;
}
