#ifndef CCGAME_SDL_h_
#define CCGAME_SDL_h_
#include <SDL2/SDL.h>

#include <initializer_list>
#include <string>
namespace ccgame {
// To specify the feature which should be initialized.
enum CCGAME_INIT { CCGAME_INIT_VIDEO, CCGAME_INIT_IMG, CCGAME_INIT_TTF };

// Forward decorate to avoid including recursively.
class Window;

// Manage the relevant resources of the SDL2.
class SDL {
 public:
  // Will initialize all the features by default.
  // Throw std::runtime_error when failed to initialize.
  SDL();

  // Passed in several CCGAME_INIT enum and initialize them.
  // Throw std::invalid_argument when not passed in CCGAME_INIT_VIDEO
  // And throw std::runtime_error when failed to initialize.
  SDL(std::initializer_list<CCGAME_INIT>);

  // Manage the deconstruction of the resources.
  ~SDL();

  // Register the window and make it be able to start working.
  // Attention: this function must be called before window.Start();
  void RegisterWindow(Window &window) const;

 private:
  bool initialized_{false};
  void InitVideo() const;
  void InitImg() const;
  void InitTtf() const;
};
}  // namespace ccgame
#endif