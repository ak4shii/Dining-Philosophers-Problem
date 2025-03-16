#include <windows.h>
#include <bits/stdc++.h>

using namespace std;

const int NUM_PHILOSOPHERS = 5;
const int CYCLES = 10;

HANDLE forks[NUM_PHILOSOPHERS];
int first[NUM_PHILOSOPHERS];
int second[NUM_PHILOSOPHERS];

void philosopher(int id) {
    for (int i = 0; i < CYCLES; i++) {
        // cout << "Philosopher " << id + 1 << " is THINKING" << endl;
        printf("\033[1;31mPhilosopher %d is THINKING \033[0m\n", id + 1);
        Sleep(100);

        WaitForSingleObject(forks[first[id]], INFINITE);
        printf("Philosopher %d has picked up fork %d\n", id + 1, first[id] + 1);

        WaitForSingleObject(forks[second[id]], INFINITE);
        printf("Philosopher %d has picked up fork %d\n", id + 1, second[id] + 1);

        printf("\033[1;32mPhilosopher %d is EATING \033[0m\n", id + 1);
        Sleep(100);

        printf("Philosopher %d has released fork %d\n", id + 1, second[id] + 1);
        ReleaseSemaphore(forks[second[id]], 1, NULL);
        printf("Philosopher %d has released fork %d\n", id + 1, first[id] + 1);
        ReleaseSemaphore(forks[first[id]], 1, NULL);
    }
}

int main() {
    for(int i = 0; i < NUM_PHILOSOPHERS - 1; i++) {
        first[i] = i;
        second[i] = i + 1;
    }
    first[NUM_PHILOSOPHERS - 1] = 0;
    second[NUM_PHILOSOPHERS - 1] = NUM_PHILOSOPHERS - 1;

    vector<thread> philosophers;

    for (int i = 0; i < NUM_PHILOSOPHERS; ++i) {
        forks[i] = CreateSemaphore(NULL, 1, 1, NULL);
        if (forks[i] == NULL) {
            std::cerr << "Semaphore Creation Failed!" << std::endl;
            return 1;
        }
    }

    for (int i = 0; i < NUM_PHILOSOPHERS; ++i) {
        philosophers.emplace_back(philosopher, i);
    }

    for (auto& philosopher : philosophers) {
        philosopher.join();
    }

    for (int i = 0; i < NUM_PHILOSOPHERS; ++i) {
        CloseHandle(forks[i]);
    }

    return 0;
}
