#include "imageview.h"

#include <SDL2/SDL_image.h>

#include <stdexcept>
namespace ccgame {
ImageView::ImageView(std::string path, int x, int y, SDL_Color *colorkey)
    : path_(path), colorkey_(colorkey) {
  SetPosition(x, y);
}
void ImageView::OnShow(Context &context) {
  SDL_Surface *loadedImg{IMG_Load(path_.c_str())};
  if (loadedImg == nullptr) {
    throw std::runtime_error(IMG_GetError());
  }

  if (colorkey_ != nullptr) {
    SDL_SetColorKey(loadedImg, SDL_TRUE,
                    SDL_MapRGB(loadedImg->format, colorkey_->r, colorkey_->g,
                               colorkey_->b));
  }

  LoadFromSurface(context, loadedImg);

  TextureView::OnShow(context);
}

}  // namespace ccgame