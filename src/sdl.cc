#include "sdl.h"

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>

#include <stdexcept>

#include "window.h"
namespace ccgame {
SDL::SDL(std::initializer_list<CCGAME_INIT> list) {
  for (auto i : list) {
    switch (i) {
      case CCGAME_INIT_VIDEO:
        InitVideo();
        break;
      case CCGAME_INIT_IMG:
        InitImg();
        break;
      case CCGAME_INIT_TTF:
        InitTtf();
        break;
    }
  }

  // 当在x11系统上运行时，SDL会默认禁用compositor，从而导致非全屏sdl程序发生异常
  // 此处禁用这个默认行为
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

  SDL_Window *w{SDL_CreateWindow(window.title_.c_str(), window.x_, window.y_,
                                 window.width_, window.height_,
                                 SDL_WINDOW_SHOWN)};

  if (w == nullptr) {
    throw std::runtime_error(SDL_GetError());
  }

  // 默认启用垂直同步与硬件加速
  SDL_Renderer *r{SDL_CreateRenderer(
      w, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC)};

  if (r == nullptr) {
    throw std::runtime_error(SDL_GetError());
  }

  window.context_ = Context(w, r);
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