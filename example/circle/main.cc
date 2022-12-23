#include <SDL_pixels.h>

#include <iostream>

#include "ccgame.h"

using namespace ccgame;

constexpr int WINDOW_WIDTH = 640;
constexpr int WINDOW_HEIGHT = 480;

int main([[maybe_unused]] int argc, [[maybe_unused]] char **argv) {
  try {
    SDL context{CCGAME_INIT_VIDEO, CCGAME_INIT_TTF};

    Window window("Circle", WINDOW_WIDTH, WINDOW_HEIGHT);

    PaintView paint(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);
    paint.SetBackgroundColor(SDL_Color{0xFF, 0xFF, 0xFF, 0xFF});
    // paint.DrawAACircle(WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2, 200,
    //                    SDL_Color{0, 0, 0, 0xFF});
    paint.FillAACircle(WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2, 200,
                       SDL_Color{0, 0, 0, 0xFF});
    // paint.FillCircle(WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2, 200,
    //                  SDL_Color{0, 0, 0, 0xFF});

    window.RegisterView(&paint);

    context.RegisterWindow(window);

    window.Start();

    return 0;
  } catch (std::runtime_error &e) {
    std::cout << e.what() << std::endl;
    return -1;
  }
}