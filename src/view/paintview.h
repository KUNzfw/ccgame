#ifndef CCGAME_PAINTVIEW_h_
#define CCGAME_PAINTVIEW_h_
#include "textureview.h"
namespace ccgame {
class PaintView : public TextureView {
 public:
  // Construct the PaintView with the given parameters.
  PaintView(int x, int y, int width, int height);
  PaintView() = delete;

  void OnShow(Context& context) override;
  void OnRender(Context& context) override;

  // Set the background color of the PaintView.
  void SetBackgroundColor(const SDL_Color& color);
  // Get the background color of the PaintView.
  [[nodiscard]] const SDL_Color& GetBackgroundColor() const;

  // Clear the view.
  void Clear();
  // Draw a rectangle on the view.
  void DrawRect(const SDL_Rect& rect, bool solid, const SDL_Color& color);
  // Draw a rectangle on the view.
  void DrawRect(int x, int y, int w, int h, bool solid, const SDL_Color& color);
  // Draw a solid circle on the view.
  void FillCircle(int x, int y, int radius, const SDL_Color& color);
  // Draw an anti-aliasing circle on the view.
  void DrawAACircle(int x, int y, int radius, const SDL_Color& color);
  // Draw a solid anti-aliasing circle on the view.
  void FillAACircle(int x, int y, int radius, const SDL_Color& color);

 private:
  SDL_Color bgcolor_{0xFF, 0xFF, 0xFF, 0xFF};

  enum GEO {
    GEO_CLEAR,         // type
    GEO_RECT,          // type x y w h r g b a solid
    GEO_CIRCLE,        // type x y radius r g b a
    GEO_AACIRCLE,      // type x y radius r g b a
    GEO_AACIRCLE_FILL  // type x y radius r g b a
  };
  std::vector<std::vector<int>> draw_cmds_;
};
}  // namespace ccgame
#endif