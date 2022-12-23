#include "textview.h"

#include <SDL2/SDL.h>

#include <stdexcept>
#include <utility>
namespace ccgame {
Font::Font(const std::string& path, int ptsize) {
  font_ = TTF_OpenFont(path.c_str(), ptsize);
  if (font_ == nullptr) {
    throw std::runtime_error(TTF_GetError());
  }
}

Font::~Font() {
  if (font_ != nullptr) {
    TTF_CloseFont(font_);
    font_ = nullptr;
  }
}

TTF_Font *Font::Get() { return font_; }

TextView::TextView(std::string text, Font *font, int x, int y, SDL_Color color)
    : text_(std::move(text)), font_(font), color_(color) {
  SetPosition(x, y);
}

void TextView::OnCreate(Context &context) {
  LoadTextureFromText(context);
  TextureView::OnCreate(context);
}

void TextView::SetText(Context &context, std::string text) {
  text_ = std::move(text);
  LoadTextureFromText(context);
}

void TextView::LoadTextureFromText(Context &context) {
  SDL_Surface *loadedText{
      TTF_RenderText_Solid(font_->Get(), text_.c_str(), color_)};
  if (loadedText == nullptr) {
    throw std::runtime_error(SDL_GetError());
  }

  LoadFromSurface(context, loadedText);
}
}  // namespace ccgame