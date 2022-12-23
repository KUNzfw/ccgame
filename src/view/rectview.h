#ifndef CCGAME_RECTVIEW_h_
#define CCGAME_RECTVIEW_h_
#include "paintview.h"
namespace ccgame {
class RectView : public PaintView {
 public:
  RectView() = delete;
  RectView(const SDL_Rect& rect, bool solid, const SDL_Color& color);

  void SetSolid(bool solid);
  void SetColor(const SDL_Color& color);

 private:
  SDL_Rect rect_{};
  bool solid_{false};
  SDL_Color color_;
};
}  // namespace ccgame
#endif