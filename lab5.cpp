#include <iostream>
#include <vector>
#include <unistd.h>
#include <sys/wait.h>

using namespace std;

// Funcție pentru verificarea numerelor prime
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

    for (int i = 0; i < num_processes; i++) {
        int pipe_to_child[2];   // Părinte -> Copil
        int pipe_to_parent[2];  // Copil -> Părinte

        if (pipe(pipe_to_child) == -1 || pipe(pipe_to_parent) == -1) {
            perror("Eroare la crearea pipe-ului");
            return 1;
        }

        pid_t pid = fork();
        if (pid < 0) {
            perror("Eroare la fork");
            return 1;
        }

        if (pid == 0) {
            // === PROCESUL COPIL ===
            close(pipe_to_child[1]);
            close(pipe_to_parent[0]);

            int start, end;
            read(pipe_to_child[0], &start, sizeof(start));
            read(pipe_to_child[0], &end, sizeof(end));
            close(pipe_to_child[0]);

            vector<int> primes;
            for (int n = start; n <= end; n++) {
                if (isPrime(n)) {
                    primes.push_back(n);
                }
            }

            int count = primes.size();
            write(pipe_to_parent[1], &count, sizeof(count));
            if (count > 0) {
                write(pipe_to_parent[1], primes.data(), count * sizeof(int));
            }
            close(pipe_to_parent[1]);
            _exit(0);
        } else {
            // === PROCESUL PĂRINTE ===
            close(pipe_to_child[0]);
            close(pipe_to_parent[1]);

            int start = i * interval_size + 1;
            int end = (i + 1) * interval_size;

            write(pipe_to_child[1], &start, sizeof(start));
            write(pipe_to_child[1], &end, sizeof(end));
            close(pipe_to_child[1]);

            int count;
            read(pipe_to_parent[0], &count, sizeof(count));
            vector<int> primes(count);
            if (count > 0) {
                read(pipe_to_parent[0], primes.data(), count * sizeof(int));
            }
            close(pipe_to_parent[0]);

            cout << "Intervalul [" << start << " - " << end << "]:\n";
            for (int p : primes) {
                cout << p << " ";
            }
            cout << "\n\n";

            wait(NULL);
        }
    }

    return 0;
}
