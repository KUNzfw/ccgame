#ifndef CCGMAE_TEXTURE_h_
#define CCGMAE_TEXTURE_h_
#include <optional>

#include "../view.h"
namespace ccgame {
class TextureView : public View {
 public:
  ~TextureView();
  void OnShow(Context &context) override;
  void OnRender(Context &context) override;
  void OnHide(Context &context) override;

  // Set the rotation angle of the texture content.
  void SetRotation(int center_x, int center_y, double angle);
  // Set the flip mode of the texture content.
  void SetFlip(SDL_RendererFlip flip_mode);
  // Set the blend mode of the texture content.
  void SetBlendMode(SDL_BlendMode blend_mode);
  // Set the clip rectangle of the texture.
  void SetClip(int x, int y, int width, int height);
  // Set the scale size of the texture.
  void SetScale(double scale_width, double scale_height);
  // Set the transparent value of the texture.
  void SetAlpha(Uint8 alpha);

  // Get the width of the texture.
  [[nodiscard]] int GetTextureWidth() const;
  // Get the height of the texture.
  [[nodiscard]] int GetTextureHeight() const;

  // Load texture from given surface.
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