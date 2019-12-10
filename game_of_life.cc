#include "GameOfLife.h"

#include <chrono>
#include <iostream>
#include <random>
#include <thread>

using namespace std;

int main() {
    srand((int)time(0));
    struct winsize win;
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &win);
    const int original_density = rand() % 100;
    GameOfLife game_of_life(win.ws_col - 2, win.ws_row - 5, original_density);
    cout << game_of_life << endl;
    this_thread::sleep_for(chrono::milliseconds(2000));
    while (true) {
        cout << game_of_life << endl;
        game_of_life.generation();
        this_thread::sleep_for(chrono::milliseconds(200));
    }
}
