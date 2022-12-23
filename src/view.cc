#include "view.h"
namespace ccgame {
void View::SetOnShowListener(std::function<void(Context &)> listener) {
  on_show_listener_ = std::move(listener);
}
void View::SetOnRenderListener(std::function<void(Context &)> listener) {
  on_render_listener_ = std::move(listener);
}
void View::SetOnKeydownListener(
    std::function<void(Context &, SDL_Keycode)> listener) {
  on_keydown_listener_ = std::move(listener);
}
void View::SetOnMouseButtonDownListener(
    std::function<void(Context &, Sint32, Sint32, Uint8)> listener) {
  on_mousebuttondown_listener_ = std::move(listener);
}
void View::SetOnMouseMotion(
    std::function<void(Context &, Sint32, Sint32)> listener) {
  on_mousemotion_listener_ = std::move(listener);
}
void View::SetOnHideListener(std::function<void(Context &)> listener) {
  on_hide_listener_ = std::move(listener);
}

void View::SetPosition(int x, int y) {
  x_ = x;
  y_ = y;
  UpdateRenderInfo();
}
void View::SetSize(int width, int height) {
  width_ = width;
  height_ = height;
  UpdateRenderInfo();
}
void View::SetAlign(CCGAME_ALIGN align) {
  align_ = align;
  UpdateRenderInfo();
}

int View::GetX() const { return x_; }
int View::GetY() const { return y_; }

int View::GetWidth() const { return width_; }
int View::GetHeight() const { return height_; }

int View::GetRenderX() const { return render_x_; }
int View::GetRenderY() const { return render_y_; }

bool View::IsInside(Sint32 x, Sint32 y) const {
  return x >= render_x_ && y >= render_y_ && x < render_x_ + width_ &&
         y < render_y_ + height_;
}

void View::Hide() { hidden_ = true; }
void View::Show() { hidden_ = false; }

bool View::isHidden() const { return hidden_; }

void View::UpdateRenderInfo() {
  switch (align_) {
    case CCGAME_ALIGN_TOPLEFT:
      render_x_ = x_;
      render_y_ = y_;
      break;
    case CCGAME_ALIGN_CENTER:
      render_x_ = x_ - width_ / 2;
      render_y_ = y_ - height_ / 2;
      break;
  }
}
}  // namespace ccgame