#ifndef CCGAME_CONTEXT_h_
#define CCGAME_CONTEXT_h_
#include <SDL2/SDL.h>
namespace ccgame {
// 封装SDL的函数以避免SDL_Window等指针的暴露
class Context {
 public:
  Context() = default;
  // 禁止复制
  Context(const Context &rval) = delete;
  Context(SDL_Window *window, SDL_Renderer *renderer);
  Context(Context &&rval) noexcept;
  ~Context();

  Context &operator=(Context &&rval) noexcept;
  Context &operator=(const Context &rval) = delete;

  void SetRenderDrawColor(Uint8 r, Uint8 g, Uint8 b, Uint8 a);
  void SetRenderTarget(SDL_Texture *texture);
  void SetRenderDrawBlendMode(SDL_BlendMode blendMode);

  void RenderFillRect(const SDL_Rect *rect);
  void RenderDrawRect(const SDL_Rect *rect);
  void RenderDrawPoint(int x, int y);
  void RenderDrawLine(int x1, int y1, int x2, int y2);

  void RenderClear();
  void RenderCopyEx(SDL_Texture *texture, const SDL_Rect *srcrect,
                    const SDL_Rect *dstrect, double angle,
                    const SDL_Point *center, SDL_RendererFlip flip);

  void RenderPresent();

  SDL_Texture *CreateTextureFromSurface(SDL_Surface *surface);
  SDL_Texture *CreateTexture(Uint32 format, int access, int w, int h);

 private:
  SDL_Window *window_;
  SDL_Renderer *renderer_;
};
}  // namespace ccgame
#endif