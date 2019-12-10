#pragma once

#include <sys/ioctl.h>
#include <unistd.h>

#include <cstdio>
#include <iostream>
#include <set>
#include <vector>

void clrscr();

class GameOfLife {
  public:
    GameOfLife(const int x_, const int y_, const int original_density_) :
    x{x_}, y{y_}, _original_density{original_density_} {
        _table.reserve(x * y);
        _next_table.reserve(x * y);
        for (int i = 0; i < y; ++i) {
            std::vector<bool> v;
            for (int j = 0; j < x; ++j) {
                if ((rand() % 100) < _original_density) {
                    v.push_back(true);
                    ++_living_cells;
                } else {
                    v.push_back(false);
                }
            }
            _table.push_back(v);
            _next_table.push_back(v);
        }
        set_original_density();
        set_densities();
    }

    std::vector<std::vector<bool>> get_table() const {
        return _table;
    }

    int get_original_density() const {
        return _original_density;
    }

    int get_generations() const {
        return _generations;
    }

    int get_current_density() const {
        return _current_density;
    }

    int get_min_density() const {
        return _min_density;
    }

    int get_max_density() const {
        return _max_density;
    }

    void generation() {
        static std::set<int> staying_alive{2, 3};
        static std::set<int> born{3};
        _living_cells = 0;
        ++_generations;
        for (int i = 0; i < y; ++i) {
            for (int j = 0; j < x; ++j) {
                const int number_of_adj = get_number_of_adj(i, j);
                if (_table[i][j]) {
                    if (staying_alive.find(number_of_adj) != staying_alive.end()) {
                        _next_table[i][j] = true;
                        ++_living_cells;
                    } else {
                        _next_table[i][j] = false;
                    }
                } else {
                    if (born.find(number_of_adj) != born.end()) {
                        _next_table[i][j] = true;
                        ++_living_cells;
                    } else {
                        _next_table[i][j] = false;
                    }
                }
            }
        }
        swap(_table, _next_table);
        set_densities();
    }

    friend std::ostream& operator<<(std::ostream& os, const GameOfLife& g);

  private:
    int x;
    int y;
    int _original_density;
    int _current_density;
    int _min_density{100};
    int _max_density{0};
    int _living_cells{0};
    int long _generations{0};
    std::vector<std::vector<bool>> _table;
    std::vector<std::vector<bool>> _next_table;

    void set_original_density() {
        _original_density = 100 * _living_cells / x / y;
    }

    void set_densities() {
        _current_density = 100 * _living_cells / x / y;
        if (_current_density < _min_density) {
            _min_density = _current_density;
        }
        if (_current_density > _max_density) {
            _max_density = _current_density;
        }
    }

    int get_number_of_adj(const int i, const int j) const {
        int sum = 0;
        if (i > 0 && j > 0 && _table[i - 1][j - 1]) ++sum;
        if (i > 0 && _table[i - 1][j]) ++sum;
        if (i > 0 && j < x - 1 && _table[i - 1][j + 1]) ++sum;
        if (j > 0 && _table[i][j - 1]) ++sum;
        if (j < x - 1 && _table[i][j + 1]) ++sum;
        if (i < y - 1 && j > 0 && _table[i + 1][j - 1]) ++sum;
        if (i < y - 1 && _table[i + 1][j]) ++sum;
        if (i < y - 1 && j < x - 1 && _table[i + 1][j + 1]) ++sum;
        return sum;
    }
};

std::ostream& operator<<(std::ostream& os, const GameOfLife& g) {
    clrscr();
    for (int i = -1; i <= g.y; ++i) {
        for (int j = -1; j <= g.x; ++j) {
            if (i == -1 || i == g.y || j == -1 || j == g.x) {
                os << "#";
            } else if (g._table[i][j]) {
                os << "0";
            } else {
                os << " ";
            }
        }
        os << std::endl;
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
       << std::endl
       << "X: " << g.x
       << ", Y: " << g.y;
    return os;
}

void clrscr() {
    printf("\033[2J"); // clean screen
    printf("\033[1;1H"); // move cursor to the first line
}
