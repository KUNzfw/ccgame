#ifndef CCGAME_PAINTVIEW_h_
#define CCGAME_PAINTVIEW_h_
#include "textureview.h"
namespace ccgame {
class PaintView : public TextureView {
 public:
  PaintView(int x, int y, int width, int height);
  PaintView() = delete;

  void OnCreate(Context& context) override;
  void OnRender(Context& context) override;

  void SetBackgroundColor(const SDL_Color& color);

  const SDL_Color& GetBackgroundColor() const;

  void Clear();
  void DrawRect(const SDL_Rect& rect, bool solid, const SDL_Color& color);

 private:
  SDL_Color bgcolor_{0xFF, 0xFF, 0xFF, 0xFF};

  enum GEO {
    GEO_CLEAR,  // type
    GEO_RECT    // type x y w h r g b a solid
  };
  std::vector<std::vector<int>> draw_cmds_;
};
}  // namespace ccgame
#endif