#include <iostream>
#include <thread>
#include <chrono>

using namespace std;


int main(void) {
    Timer tHello;
    tHello.start(chrono::milliseconds(1000),

                 {
                         cout << "Hello!" << endl;
                 });

    thread th([&]() {
        this_thread::sleep_for(chrono::seconds(2));
        tHello.stop();
    });

    th.join();

    return 0;
}