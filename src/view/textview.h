#ifndef CCGAME_TEXTVIEW_h_
#define CCGAME_TEXTVIEW_h_
#include <SDL2/SDL_ttf.h>

#include <memory>
#include <string>

#include "textureview.h"
namespace ccgame {
class Font {
 public:
  Font() = delete;
  Font(const std::string& path, int ptsize);
  ~Font();
  TTF_Font *Get();

 private:
  TTF_Font *font_{};
};

class TextView : public TextureView {
 public:
  TextView() = delete;
  TextView(std::string text, Font *font, int x, int y,
           SDL_Color color = {0, 0, 0});
  void OnShow(Context &context) override;

  void SetText(Context &context, std::string text);

 private:
  void LoadTextureFromText(Context &context);
  std::string text_{};
  Font *font_{};
  SDL_Color color_;
};
}  // namespace ccgame
#endif