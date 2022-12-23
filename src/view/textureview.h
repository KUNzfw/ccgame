#ifndef CCGMAE_TEXTURE_h_
#define CCGMAE_TEXTURE_h_
#include <optional>

#include "../view.h"
namespace ccgame {
class TextureView : public View {
 public:
  ~TextureView();
  void OnCreate(Context &context) override;
  void OnRender(Context &context) override;

  void SetRotation(int center_x, int center_y, double angle);
  void SetFlip(SDL_RendererFlip flip_mode);
  void SetBlendMode(SDL_BlendMode blend_mode);
  void SetClip(int x, int y, int width, int height);
  void SetScale(double scale_width, double scale_height);
  void SetAlpha(Uint8 alpha);

  [[nodiscard]] int GetTextureWidth() const;
  [[nodiscard]] int GetTextureHeight() const;

  void LoadFromSurface(Context &context, SDL_Surface *surface);

 protected:
  SDL_Texture *texture_ = nullptr;

  int texture_width_{0};
  int texture_height_{0};

 private:
  SDL_Point rotation_center_{0, 0};
  double rotation_angle_{0.0};
  SDL_RendererFlip flip_mode_{SDL_FLIP_NONE};

  std::optional<SDL_BlendMode> blend_mode_{};
  std::optional<Uint8> alpha_{};

  SDL_Rect clip_{0, 0, 0, 0};
  double scale_width_{1.0};
  double scale_height_{1.0};
};
}  // namespace ccgame
#endif