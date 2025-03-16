#include <windows.h>
#include <bits/stdc++.h>

using namespace std;

const int NUM_PHILOSOPHERS = 5;
const int CYCLES = 3;
HANDLE forks[NUM_PHILOSOPHERS];

void philosopher(int id) {
    for (int i = 0; i < CYCLES; i++) {
        // cout << "Philosopher " << id + 1 << " is THINKING" << endl;
        printf("\033[1;31mPhilosopher %d is THINKING \033[0m\n", id + 1);
        Sleep(1000);

        WaitForSingleObject(forks[id], INFINITE);
        printf("Philosopher %d has picked up fork %d\n", id + 1, id + 1);

        WaitForSingleObject(forks[(id + 1)%NUM_PHILOSOPHERS], INFINITE);
        printf("Philosopher %d has picked up fork %d\n", id + 1, (id + 1)%NUM_PHILOSOPHERS + 1);

        printf("\033[1;32mPhilosopher %d is EATING \033[0m\n", id + 1);
        Sleep(1000);

        printf("Philosopher %d has released fork %d\n", id + 1, (id + 1)%NUM_PHILOSOPHERS + 1);
        ReleaseSemaphore(forks[(id + 1)%NUM_PHILOSOPHERS], 1, NULL);
        printf("Philosopher %d has released fork %d\n", id + 1, id + 1);
        ReleaseSemaphore(forks[id], 1, NULL);
    }
}

int main() {
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
