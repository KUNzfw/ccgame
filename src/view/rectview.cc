#include "rectview.h"
namespace ccgame {
RectView::RectView(const SDL_Rect& rect, bool solid, const SDL_Color& color)
    : solid_(solid), color_(color), PaintView(rect.x, rect.y, rect.w, rect.h) {
  rect_ = SDL_Rect{0, 0, rect.w, rect.h};
}

void RectView::OnShow(Context& context) {
  PaintView::OnShow(context);
  DrawRect(rect_, solid_, color_);
}

void RectView::SetSolid(bool solid) {
  solid_ = solid;
  Clear();
  DrawRect(rect_, solid_, color_);
}
void RectView::SetColor(const SDL_Color& color) {
  color_ = color;
  Clear();
  DrawRect(rect_, solid_, color_);
}
}  // namespace ccgame