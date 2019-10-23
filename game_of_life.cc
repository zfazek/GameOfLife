#include <chrono>
#include <cstdio>
#include <ctime>
#include <iostream>
#include <random>
#include <thread>
#include <vector>

void clrscr();

using namespace std;

static constexpr int X = 140;
static constexpr int Y = 70;

class GameOfLife {
  public:
    GameOfLife() {
        const int limit = rand() % 100;
        for (unsigned i = 0; i < Y; ++i) {
            for (unsigned j = 0; j < X; ++j) {
                if ((rand() % 100) < limit) {
                    _table[i][j] = true;
                } else {
                    _table[i][j] = false;
                }
            }
        }
    }

    void step() {
        bool next_table[Y + 2][X + 2]{false};
        for (unsigned i = 1; i <= Y; ++i) {
            for (unsigned j = 1; j <= X; ++j) {
                const unsigned number_of_adj = get_number_of_adj(i, j);
                if (_table[i][j]) {
                    if (number_of_adj == 2 || number_of_adj == 3) {
                        next_table[i][j] = true;
                    } else {
                        next_table[i][j] = false;
                    }
                } else {
                    if (number_of_adj == 3) {
                        next_table[i][j] = true;
                    } else {
                        next_table[i][j] = false;
                    }
                }
            }
        }
        for (unsigned i = 1; i <= Y; ++i) {
            for (unsigned j = 1; j <= X; ++j) {
                _table[i][j] = next_table[i][j];
            }
        }
    }

    friend ostream& operator<<(ostream& os, const GameOfLife& g);

  private:
    bool _table[Y + 2][X + 2]{false};

    unsigned get_number_of_adj(const int i, const int j) {
        unsigned sum = 0;
        if (_table[i - 1][j - 1]) ++sum;
        if (_table[i - 1][j    ]) ++sum;
        if (_table[i - 1][j + 1]) ++sum;
        if (_table[i    ][j - 1]) ++sum;
        if (_table[i    ][j + 1]) ++sum;
        if (_table[i + 1][j - 1]) ++sum;
        if (_table[i + 1][j    ]) ++sum;
        if (_table[i + 1][j + 1]) ++sum;
        return sum;
    }
};

ostream& operator<<(ostream& os, const GameOfLife& g) {
    clrscr();
    for (unsigned i = 1; i <= Y; ++i) {
        for (unsigned j = 1; j <= X; ++j) {
            if (g._table[i][j]) {
                os << "0";
            } else {
                os << " ";
            }
        }
        os << endl;
    }
    os << endl;
    return os;
}

void clrscr() {
   fprintf(stdout, "\033[2J"); // clean screen
   fprintf(stdout, "\033[1;1H"); // move cursor to the first line
}

int main() {
    srand((unsigned)time(0));
    GameOfLife game_of_life;
    cout << game_of_life << endl;
    this_thread::sleep_for(chrono::milliseconds(2000));
    for (;;) {
        cout << game_of_life << endl;
        game_of_life.step();
        this_thread::sleep_for(chrono::milliseconds(200));
    }
    cout << game_of_life << endl;
}
