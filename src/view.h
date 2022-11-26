#ifndef CCGAME_VIEW_h_
#define CCGAME_VIEW_h_
#include <SDL2/SDL.h>

#include <functional>

#include "context.h"
#include "window.h"
namespace ccgame {
enum CCGAME_ALIGN { CCGAME_ALIGN_TOPLEFT, CCGAME_ALIGN_CENTER };
class View {
 public:
  View();
  ~View();

  virtual void OnCreate(Context &context);
  virtual void OnRender(Context &context);
  virtual void OnKeyDown(Context &context, SDL_Keycode keycode);
  virtual void OnMouseButtonDown(Context &context, Sint32 x, Sint32 y,
                                 Uint8 button);
  virtual void OnMouseMotion(Context &context, Sint32 x, Sint32 y);
  virtual void OnQuit(Context &context);

  void SetOnCreateListener(std::function<void(Context &)> listener);
  void SetOnRenderListener(std::function<void(Context &)> listener);
  void SetOnKeydownListener(
      std::function<void(Context &, SDL_Keycode)> listener);
  void SetOnMouseButtonDownListener(
      std::function<void(Context &context, Sint32 x, Sint32 y, Uint8 button)> listener);
  void SetOnMouseMotion(
      std::function<void(Context &, Sint32, Sint32)> listener);
  void SetOnQuitListener(std::function<void(Context &)> listener);

  void SetPosition(int x, int y);
  void SetAlign(CCGAME_ALIGN align);

  int GetX() const;
  int GetY() const;
  int GetWidth() const;
  int GetHeight() const;

  // 判断坐标是否在View内部
  bool IsInside(Sint32 x, Sint32 y) const;

  void Hide();
  void Show();

  friend void Window::Start();

 protected:
  void SetSize(int width, int height);

  int GetRenderX() const;
  int GetRenderY() const;

 private:
  CCGAME_ALIGN align_ = CCGAME_ALIGN_TOPLEFT;

  bool hidden_{false};

  int x_;
  int y_;
  int width_;
  int height_;

  int render_x_;
  int render_y_;

  std::function<void(Context &)> on_create_listener_ = nullptr;
  std::function<void(Context &)> on_render_listener_ = nullptr;
  std::function<void(Context &, SDL_Keycode)> on_keydown_listener_ = nullptr;
  std::function<void(Context &, Sint32, Sint32, Uint8)>
      on_mousebuttondown_listener_ = nullptr;
  std::function<void(Context &, Sint32, Sint32)> on_mousemotion_listener_ =
      nullptr;
  std::function<void(Context &)> on_quit_listener_ = nullptr;

  void UpdateRenderInfo();
};
}  // namespace ccgame
#endif