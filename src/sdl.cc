#include "sdl.h"

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>

#include <algorithm>
#include <stdexcept>

#include "window.h"
namespace ccgame {
SDL::SDL() { SDL({CCGAME_INIT_VIDEO, CCGAME_INIT_IMG, CCGAME_INIT_TTF}); }
SDL::SDL(std::initializer_list<CCGAME_INIT> list) {
  if (std::find(list.begin(), list.end(), CCGAME_INIT_VIDEO) != list.end()) {
    InitVideo();
  } else {
    throw std::invalid_argument("Haven't passed in CCGAME_INIT_VIDEO");
  }
  for (auto i : list) {
    switch (i) {
      case CCGAME_INIT_VIDEO:
        break;
      case CCGAME_INIT_IMG:
        InitImg();
        break;
      case CCGAME_INIT_TTF:
        InitTtf();
        break;
    }
  }

  // When program based on SDL2 runs on x11, it will disable compositor by
  // default. This will lead to strange behavior, so the ccgame disable it.
  SDL_SetHint(SDL_HINT_VIDEO_X11_NET_WM_BYPASS_COMPOSITOR, "0");

  initialized_ = true;
}

SDL::~SDL() {
  SDL_Quit();
  IMG_Quit();
}

void SDL::RegisterWindow(Window &window) const {
  if (!initialized_) {
    return;
  }

  SDL_Window *sdl_window{SDL_CreateWindow(window.title_.c_str(), window.x_,
                                          window.y_, window.width_,
                                          window.height_, SDL_WINDOW_SHOWN)};

  if (sdl_window == nullptr) {
    throw std::runtime_error(SDL_GetError());
  }

  // Enable the renderer accelerated.
  SDL_Renderer *sdl_renderer{
      SDL_CreateRenderer(sdl_window, -1, SDL_RENDERER_ACCELERATED)};

  if (sdl_renderer == nullptr) {
    throw std::runtime_error(SDL_GetError());
  }

  window.context_ = Context(sdl_window, sdl_renderer);
  window.registered_ = true;
}

void SDL::InitVideo() const {
  if (initialized_) {
    return;
  }
  if (SDL_Init(SDL_INIT_VIDEO) < 0) {
    throw std::runtime_error(SDL_GetError());
  }
}

void SDL::InitImg() const {
  if (initialized_) {
    return;
  }
  if (!(IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG)) {
    throw std::runtime_error(IMG_GetError());
  }
}

void SDL::InitTtf() const {
  if (initialized_) {
    return;
  }
  if (TTF_Init() == -1) {
    throw std::runtime_error(TTF_GetError());
  }
}
}  // namespace ccgame