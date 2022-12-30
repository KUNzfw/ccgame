#ifndef CCGAME_RECTVIEW_h_
#define CCGAME_RECTVIEW_h_
#include "paintview.h"
namespace ccgame {
class RectView : public PaintView {
 public:
  RectView() = delete;
  // Construct the RectView the given parameters.
  RectView(const SDL_Rect& rect, bool solid, const SDL_Color& color);

  void OnShow(Context& context) override;

  // Set if the rect is solid.
  void SetSolid(bool solid);
  // Set the color of the rect.
  void SetColor(const SDL_Color& color);

 private:
  SDL_Rect rect_{};
  bool solid_{false};
  SDL_Color color_;
};
}  // namespace ccgame
#endif