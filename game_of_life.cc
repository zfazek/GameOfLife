#include <chrono>
#include <cstdio>
#include <ctime>
#include <iostream>
#include <random>
#include <thread>
#include <vector>

void clrscr();

using namespace std;

class GameOfLife {
  public:
    GameOfLife() {
        _original_density = rand() % 100;
        for (unsigned i = 1; i <= Y; ++i) {
            for (unsigned j = 1; j <= X; ++j) {
                if ((rand() % 100) < _original_density) {
                    _table[i][j] = true;
                } else {
                    _table[i][j] = false;
                }
            }
        }
    }

    int get_original_density() const {
        return _original_density;
    }

    int get_generations() const {
        return _generations;
    }

    unsigned get_current_density() const {
        return _current_density;
    }

    unsigned get_min_density() const {
        return _min_density;
    }

    unsigned get_max_density() const {
        return _max_density;
    }

    int get_x() const {
        return X;
    }

    int get_y() const {
        return Y;
    }

    void generation() {
        _living_cells = 0;
        ++_generations;
        bool next_table[Y + 2][X + 2]{false};
        for (unsigned i = 1; i <= Y; ++i) {
            for (unsigned j = 1; j <= X; ++j) {
                const unsigned number_of_adj = get_number_of_adj(i, j);
                if (_table[i][j]) {
                    if (number_of_adj == 2 || number_of_adj == 3) {
                        next_table[i][j] = true;
                        ++_living_cells;
                    } else {
                        next_table[i][j] = false;
                    }
                } else {
                    if (number_of_adj == 3) {
                        next_table[i][j] = true;
                        ++_living_cells;
                    } else {
                        next_table[i][j] = false;
                    }
                }
            }
        }
        _current_density = 100 * _living_cells / X / Y;
        if (_current_density < _min_density) {
            _min_density = _current_density;
        }
        if (_current_density > _max_density) {
            _max_density = _current_density;
        }
        for (unsigned i = 1; i <= Y; ++i) {
            for (unsigned j = 1; j <= X; ++j) {
                _table[i][j] = next_table[i][j];
            }
        }
    }

    friend ostream& operator<<(ostream& os, const GameOfLife& g);

  private:
    static constexpr int X = 200;
    static constexpr int Y = 48;

    int _original_density;
    int _current_density;
    int _min_density{100};
    int _max_density{0};
    unsigned _living_cells{0};
    unsigned long _generations = 0;
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
    for (unsigned i = 0; i < g.Y + 2; ++i) {
        for (unsigned j = 0; j < g.X + 2; ++j) {
            if (i == 0 || i == g.Y + 1 || j == 0 || j == g.X + 1) {
                os << "#";
            } else if (g._table[i][j]) {
                os << "0";
            } else {
                os << " ";
            }
        }
        os << endl;
    }
    os << "Original density: "
       << g.get_original_density()
       << "%, Current density: "
       << g.get_current_density()
       << "%, Minimum density: "
       << g.get_min_density()
       << "%, Maximum density: "
       << g.get_max_density()
       << "%, Generations: "
       << g.get_generations()
       << endl
       << "X: " << g.get_x()
       << ", Y: " << g.get_y()
       << endl;
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
        game_of_life.generation();
        this_thread::sleep_for(chrono::milliseconds(200));
    }
    cout << game_of_life << endl;
}
