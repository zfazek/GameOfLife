#include "GameOfLife.h"

#include <SDL2/SDL.h>
#include <emscripten.h>
#include <cstdlib>

static constexpr int WIDTH = 800;
static constexpr int HEIGHT = 600;
static constexpr int SIZE = 4;

SDL_Window *window;
SDL_Renderer *renderer;
GameOfLife *game_of_life;

SDL_Rect rect;

static SDL_Point arr_points[WIDTH * HEIGHT / SIZE / SIZE];

int get_rand_int(const int max) {
    return rand() % max;
}

void mainloop() {
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
    SDL_RenderClear(renderer);

    game_of_life->generation();
    const std::vector<std::vector<bool>> table = game_of_life->get_table();
    int count = 0;
    for (int y = 0; y < table.size(); ++y) {
        for (int x = 0; x < table[y].size(); ++x) {
            if (table[y][x]) {
                arr_points[count++] = {x, y};
            }
        }
    }
    for (int i = 0; i < count; ++i) {
        const int red = get_rand_int(128) + 128;
        const int green = get_rand_int(128) + 128;
        const int blue = get_rand_int(128) + 128;
        SDL_SetRenderDrawColor(renderer, red, green, blue, 0);
        rect.x = arr_points[i].x * SIZE;
        rect.y = arr_points[i].y * SIZE;
        rect.w = SIZE;
        rect.h = SIZE;
        SDL_RenderFillRect(renderer, &rect);
    }
//    SDL_RenderDrawPoints(renderer, arr_points, count);
    SDL_RenderPresent(renderer);
    emscripten_sleep(200);
}

int main() {
    srand((int)time(0));
    SDL_Init(SDL_INIT_VIDEO);
    SDL_CreateWindowAndRenderer(WIDTH, HEIGHT, 0, &window, &renderer);

    const int original_density = rand() % 100;
    game_of_life = new GameOfLife(WIDTH / SIZE, HEIGHT / SIZE, original_density);

    const int simulate_infinite_loop = 1;
    const int fps = -1;
    emscripten_set_main_loop(mainloop, fps, simulate_infinite_loop);

    delete game_of_life;
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return EXIT_SUCCESS;
}
