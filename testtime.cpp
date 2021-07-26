#include <chrono>
#include <iostream>
#include <thread>

int main() {
    int i = 0;
    for (;;) {
        if (i == 10) {
            std::cout << i << std::endl;
            i = 0;
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(1000));
        i++;
    }
    return 0;
}