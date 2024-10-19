#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>
#include <string>
#include <unordered_map>
#include <cmath>
#include <openssl/sha.h>
#include <algorithm>
#include <numeric>

// Function to read IDs from a text file with comma-separated integers
std::vector<long long> read_ids(const std::string& filename) {
    std::ifstream infile(filename);
    if (!infile.is_open()) {
        std::cerr << "Failed to open " << filename << std::endl;
        return {};
    }

    std::vector<long long> ids;
    std::string line;
    if (std::getline(infile, line)) {
        std::istringstream iss(line);
        std::string token;
        while (std::getline(iss, token, ',')) {
            try {
                ids.push_back(std::stoll(token));
            } catch (const std::exception& e) {
                std::cerr << "Error parsing integer: " << token << std::endl;
            }
        }
    }
    return ids;
}

// Hash Function 1: h(x) = x mod 50
int hash_modulo(long long x, int m = 50) {
    return x % m;
}

// Hash Function 2: h(x) = ((a * x + b) mod p) mod 50
int hash_linear(long long x, long long a, long long b, long long p, int m = 50) {
    return ((a * x + b) % p) % m;
}

// Helper function to convert SHA-256 hash to integer
long long sha256_to_int(const std::string& input) {
    unsigned char hash[SHA256_DIGEST_LENGTH];
    SHA256(reinterpret_cast<const unsigned char*>(input.c_str()), input.size(), hash);

    // Convert entire hash to a long long integer
    long long result = 0;
    for(int i = 0; i < SHA256_DIGEST_LENGTH; ++i) {
        result = (result << 8) | hash[i];
    }
    return result;
}

// Hash Function 3: h(x) = SHA-256(x) mod 50
int hash_sha256(long long x, int m = 50) {
    std::string str = std::to_string(x);
    long long hash_int = sha256_to_int(str);
    return hash_int % m;
}

// Function to evaluate uniformity
void evaluate_uniformity(const std::vector<int>& counts, const std::string& hash_name) {
    if (counts.empty()) {
        std::cerr << "No counts available for " << hash_name << std::endl;
        return;
    }

    // Calculate minimum and maximum counts
    int min_count = *std::min_element(counts.begin(), counts.end());
    int max_count = *std::max_element(counts.begin(), counts.end());

    // Calculate average count
    double average_count = std::accumulate(counts.begin(), counts.end(), 0.0) / counts.size();

    // Calculate frequencies in percent
    double min_freq = (static_cast<double>(min_count) / 1000.0) * 100.0;
    double avg_freq = (average_count / 1000.0) * 100.0;
    double max_freq = (static_cast<double>(max_count) / 1000.0) * 100.0;

    // Display the results
    std::cout << "Hash Function: " << hash_name << "\n";
    std::cout << "Min Frequency: " << min_freq << "%\n";
    std::cout << "Average Frequency: " << avg_freq << "%\n";
    std::cout << "Max Frequency: " << max_freq << "%\n\n";
}

int main() {
    // Read IDs from JSON file
    std::vector<long long> ids = read_ids("ids.txt");
    if(ids.empty()) {
        std::cerr << "No IDs to process." << std::endl;
        return 1;
    }

    // Parameters for Hash Function 2
    long long p = 1000001; // A large prime number
    long long a = 123457;   // Coefficient a
    long long b = 98765;    // Coefficient b

    // Containers for hash counts
    std::vector<int> hash1_counts(50, 0);
    std::vector<int> hash2_counts(50, 0);
    std::vector<int> hash3_counts(50, 0);

    // Containers for individual hash values (for independence check)
    std::vector<int> hash1_values;
    std::vector<int> hash2_values;
    std::vector<int> hash3_values;

    // Apply hash functions
    for(auto x : ids){
        int h1 = hash_modulo(x);
        int h2 = hash_linear(x, a, b, p);
        int h3 = hash_sha256(x);

        hash1_counts[h1]++;
        hash2_counts[h2]++;
        hash3_counts[h3]++;

        hash1_values.push_back(h1);
        hash2_values.push_back(h2);
        hash3_values.push_back(h3);
    }

    // Evaluate uniformity for each hash function
    evaluate_uniformity(hash1_counts, "h(x) = x mod 50");
    evaluate_uniformity(hash2_counts, "h(x) = ((a*x + b) mod p) mod 50");
    evaluate_uniformity(hash3_counts, "h(x) = SHA-256(x) mod 50");

    return 0;
}
