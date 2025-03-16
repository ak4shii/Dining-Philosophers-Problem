#include <windows.h>
#include <bits/stdc++.h>

using namespace std;

const int NUM_PHILOSOPHERS = 5;
const int THINKING = 0;
const int HUNGRY = 1;
const int EATING = 2;
const int CYCLES = 6;
int state[NUM_PHILOSOPHERS]={0};
HANDLE s[NUM_PHILOSOPHERS];
mutex mtx;

void test(int id) {
    if (state[id] == HUNGRY && state[(id - 1 + NUM_PHILOSOPHERS)%NUM_PHILOSOPHERS] != EATING && state[(id + 1)%NUM_PHILOSOPHERS] != EATING) {
        state[id] = EATING;
        ReleaseSemaphore(s[id], 1, NULL);
    }
}

void pick_forks(int id) {
    mtx.lock();
    state[id] = HUNGRY;
    printf("\033[1;33mPhilosopher %d is HUNGRY \033[0m\n", id + 1);
    test(id);
    mtx.unlock();
    WaitForSingleObject(s[id], INFINITE);
    printf("Philosopher %d has picked up fork %d and %d \n", id + 1, id + 1, (id + 1)%NUM_PHILOSOPHERS + 1);
}

void put_forks(int id) {
    mtx.lock();
    state[id] = THINKING;
    test((id - 1 + NUM_PHILOSOPHERS)%NUM_PHILOSOPHERS);
    test((id + 1)%NUM_PHILOSOPHERS);
    mtx.unlock();
}

void philosopher(int id) {
    for (int i = 0; i < CYCLES; i++) {
        // cout << "Philosopher " << id + 1 << " is THINKING" << endl;
        printf("\033[1;31mPhilosopher %d is THINKING \033[0m\n", id + 1);
        Sleep(100);

        pick_forks(id);

        printf("\033[1;32mPhilosopher %d is EATING \033[0m\n", id + 1);
        Sleep(100);

        put_forks(id);
    }
}

int main() {
    vector<thread> philosophers;

    for (int i = 0; i < NUM_PHILOSOPHERS; ++i) {
        s[i] = CreateSemaphore(NULL, 0, 1, NULL);
        if (s[i] == NULL) {
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
        CloseHandle(s[i]);
    }

    return 0;
}
