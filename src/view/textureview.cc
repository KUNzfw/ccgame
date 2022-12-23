#include "textureview.h"

#include <stdexcept>
namespace ccgame {
TextureView::~TextureView() {
  if (texture_ != nullptr) {
    SDL_DestroyTexture(texture_);
    texture_ = nullptr;
  }
}

void TextureView::OnShow(Context &context) {
  if (blend_mode_) {
    SetBlendMode(blend_mode_.value());
  }
  if (alpha_) {
    SetAlpha(alpha_.value());
  }
}

void TextureView::OnRender(Context &context) {
  if (texture_ == nullptr) return;
  SDL_Rect rect{GetRenderX(), GetRenderY(), GetWidth(), GetHeight()};
  context.RenderCopyEx(texture_, &clip_, &rect, rotation_angle_,
                       &rotation_center_, flip_mode_);
}

void TextureView::OnHide(Context &context) {
  if (texture_ != nullptr) {
    SDL_DestroyTexture(texture_);
    texture_ = nullptr;
  }
}

void TextureView::SetRotation(int center_x, int center_y, double angle) {
  rotation_center_.x = center_x;
  rotation_center_.y = center_y;
  rotation_angle_ = angle;
}

void TextureView::SetFlip(SDL_RendererFlip flip_mode) {
  flip_mode_ = flip_mode;
}

void TextureView::SetScale(double scale_width, double scale_height) {
  scale_width_ = scale_width;
  scale_height_ = scale_height;
  SetSize(static_cast<int>(clip_.w * scale_width_),
          static_cast<int>(clip_.h * scale_height_));
}

void TextureView::SetAlpha(Uint8 alpha) {
  alpha_ = alpha;
  if (texture_ != nullptr) SDL_SetTextureAlphaMod(texture_, alpha);
}

void TextureView::SetBlendMode(SDL_BlendMode blend_mode) {
  blend_mode_ = blend_mode;
  if (texture_ != nullptr) SDL_SetTextureBlendMode(texture_, blend_mode);
}

void TextureView::SetClip(int x, int y, int width, int height) {
  clip_.x = x;
  clip_.y = y;
  clip_.w = width;
  clip_.h = height;

  SetSize(static_cast<int>(width * scale_width_),
          static_cast<int>(height * scale_height_));
}

int TextureView::GetTextureWidth() const { return texture_width_; }
int TextureView::GetTextureHeight() const { return texture_height_; }

void TextureView::LoadFromSurface(Context &context, SDL_Surface *surface) {
  if (texture_ != nullptr) {
    SDL_DestroyTexture(texture_);
  }

  texture_width_ = surface->w;
  texture_height_ = surface->h;

  SetClip(0, 0, surface->w, surface->h);

  SDL_Texture *t{context.CreateTextureFromSurface(surface)};
  SDL_FreeSurface(surface);
  if (t == nullptr) {
    throw std::runtime_error(SDL_GetError());
  }

  texture_ = t;
}
}  // namespace ccgame