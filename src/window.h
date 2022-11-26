#ifndef CCGAME_WINDOW_h_
#define CCGAME_WINDOW_h_
#include <SDL2/SDL.h>

#include <map>
#include <string>
#include <vector>

#include "context.h"
#include "sdl.h"
namespace ccgame {
class View;
// 管理窗口相关信息以及View
class Window {
 public:
  Window() = delete;
  Window(std::string title, int width, int height,
         int x = SDL_WINDOWPOS_UNDEFINED, int y = SDL_WINDOWPOS_UNDEFINED);

  // 注册View使其能够渲染并接受事件
  void RegisterView(View *view, int groupid = -1);
  // 启动窗口并开始事件循环
  // !这个方法会进入死循环，在窗口退出前无法继续执行后续程序
  // 事件顺序: Create -> Events -> Render -> Events -> Render -> ... -> Quit
  // Events 包括Keydown等
  void Start();

  void ShowGroup(int groupid);
  void HideGroup(int groupid);
  void HideAllGroup();

  void Quit();

  // 传入rgba值以设置窗口的背景颜色
  void SetBackgroundColor(Uint8 r, Uint8 g, Uint8 b, Uint8 a);

  friend void SDL::RegisterWindow(Window &window) const;

 private:
  Context context_;

  std::string title_;
  int width_;
  int height_;
  int x_;
  int y_;
  SDL_Color bgcolor_{0xFF, 0xFF, 0xFF, 0xFF};

  std::vector<View *> views_;
  std::map<int, std::vector<View *>> groups_;

  // true to show, false to hide
  std::vector<std::pair<int, bool>> gruops_to_hide_or_show_;

  bool quit_{false};
};
}  // namespace ccgame

#endif