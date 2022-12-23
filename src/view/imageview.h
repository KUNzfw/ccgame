#ifndef CCGAME_IMAGE_VIEW_h_
#define CCGAME_IMAGE_VIEW_h_
#include <string>

#include "textureview.h"
namespace ccgame {
class ImageView : public TextureView {
 public:
  ImageView() = delete;
  ImageView(std::string path, int x, int y, SDL_Color *colorkey = nullptr);
  void OnShow(Context &context) override;

 private:
  std::string path_;
  SDL_Color *colorkey_;
};
}  // namespace ccgame
#endif