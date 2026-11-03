#include <iostream>
#include <vector>

using namespace std;

bool isPrime(int n) {
    if (n <= 1) return false;
    for (int i = 2; i * i <= n; i++) {
        if (n % i == 0) return false;
    }
    return true;
}

int main() {
    const int num_processes = 10;
    const int interval_size = 1000;

    cout << "=== Rulare program numere prime pe Windows ===\n\n";

    for (int i = 0; i < num_processes; i++) {
        int start = i * interval_size + 1;
        int end = (i + 1) * interval_size;

        vector<int> primes;
        for (int n = start; n <= end; n++) {
            if (isPrime(n)) {
                primes.push_back(n);
            }
        }

        cout << "Intervalul [" << start << " - " << end << "]:\n";
        for (int p : primes) {
            cout << p << " ";
        }
        cout << "\n\n";
    }

    return 0;
}