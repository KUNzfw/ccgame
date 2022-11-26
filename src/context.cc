#include "context.h"

#include <stdexcept>
namespace ccgame {
Context::Context(SDL_Window *window, SDL_Renderer *renderer)
    : window_(window), renderer_(renderer) {}

Context::Context(Context &&rval) noexcept {
  window_ = rval.window_;
  renderer_ = rval.renderer_;
  rval.window_ = nullptr;
  rval.renderer_ = nullptr;
}

Context::~Context() {
  if (renderer_ != nullptr) {
    SDL_DestroyRenderer(renderer_);
    renderer_ = nullptr;
  }

  if (window_ != nullptr) {
    SDL_DestroyWindow(window_);
    window_ = nullptr;
  }
}

Context &Context::operator=(Context &&rval) noexcept {
  SDL_Window *w = window_;
  SDL_Renderer *r = renderer_;

  window_ = rval.window_;
  renderer_ = rval.renderer_;

  rval.window_ = w;
  rval.renderer_ = r;

  w = nullptr;
  r = nullptr;

  return *this;
}

void Context::SetRenderDrawColor(Uint8 r, Uint8 g, Uint8 b, Uint8 a) {
  if (SDL_SetRenderDrawColor(renderer_, r, g, b, a) != 0) {
    throw std::runtime_error(SDL_GetError());
  }
}
void Context::SetRenderTarget(SDL_Texture *texture) {
  if (SDL_SetRenderTarget(renderer_, texture) != 0) {
    throw std::runtime_error(SDL_GetError());
  }
}
void Context::SetRenderDrawBlendMode(SDL_BlendMode blendMode) {
  if (SDL_SetRenderDrawBlendMode(renderer_, blendMode) != 0) {
    throw std::runtime_error(SDL_GetError());
  }
}

void Context::RenderFillRect(const SDL_Rect *rect) {
  if (SDL_RenderFillRect(renderer_, rect) != 0) {
    throw std::runtime_error(SDL_GetError());
  }
}

void Context::RenderDrawRect(const SDL_Rect *rect) {
  if (SDL_RenderDrawRect(renderer_, rect) != 0) {
    throw std::runtime_error(SDL_GetError());
  }
}

void Context::RenderClear() {
  if (SDL_RenderClear(renderer_) != 0) {
    throw std::runtime_error(SDL_GetError());
  }
}

void Context::RenderCopyEx(SDL_Texture *texture, const SDL_Rect *srcrect,
                           const SDL_Rect *dstrect, const double angle,
                           const SDL_Point *center,
                           const SDL_RendererFlip flip) {
  if (SDL_RenderCopyEx(renderer_, texture, srcrect, dstrect, angle, center,
                       flip) != 0) {
    throw std::runtime_error(SDL_GetError());
  }
}

void Context::RenderPresent() { SDL_RenderPresent(renderer_); }

SDL_Texture *Context::CreateTextureFromSurface(SDL_Surface *surface) {
  auto t{SDL_CreateTextureFromSurface(renderer_, surface)};
  if (t == nullptr) {
    throw std::runtime_error(SDL_GetError());
  }
  return t;
}
SDL_Texture *Context::CreateTexture(Uint32 format, int access, int w, int h) {
  auto t{SDL_CreateTexture(renderer_, format, access, w, h)};
  if (t == nullptr) {
    throw std::runtime_error(SDL_GetError());
  }
  return t;
}
}  // namespace ccgame