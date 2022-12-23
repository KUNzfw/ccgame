#ifndef CCGAME_SDL_h_
#define CCGAME_SDL_h_
#include <SDL2/SDL.h>

#include <initializer_list>
#include <string>
namespace ccgame {
// 用于指定需要初始化的模块
enum CCGAME_INIT { CCGAME_INIT_VIDEO, CCGAME_INIT_IMG, CCGAME_INIT_TTF };

// 防止递归调用而前置声明Window
class Window;

// 管理SDL的初始化，SDL_Window和SDL_Renderer等的创建以及SDL的正常退出
class SDL {
 public:
  // 默认初始化没有意义
  SDL() = delete;

  // 可传入任意个CCGAME_INIT类型枚举以初始化SDL
  // 初始化失败的时候会抛出std::runtime_error异常
  SDL(std::initializer_list<CCGAME_INIT>);

  // 管理SDL的正常退出
  ~SDL();

  // 注册Window，使其能够开始执行功能
  // \param window 需要注册的Window实例
  void RegisterWindow(Window &window) const;

 private:
  bool initialized_{false};
  void InitVideo() const;
  void InitImg() const;
  void InitTtf() const;
};
}  // namespace ccgame
#endif