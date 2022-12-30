#ifndef CCGAME_WINDOW_h_
#define CCGAME_WINDOW_h_
#include <SDL2/SDL.h>

#include <initializer_list>
#include <map>
#include <string>
#include <vector>

#include "context.h"
#include "sdl.h"
namespace ccgame {
class View;
// Manage the window and its views.
class Window {
 public:
  Window() = delete;
  // Construct the window with the given parameters
  Window(std::string title, int width, int height,
         int x = SDL_WINDOWPOS_UNDEFINED, int y = SDL_WINDOWPOS_UNDEFINED);

  // Register the view so that it can get events and deal with them.
  void RegisterView(View *view, int groupid = -1);
  // Start the window and begin event loop
  // Attention: the function will block the main thread!
  // Event order: Show -> Events -> Render -> (Show,Hide) -> Events -> Render ->
  // ... -> Hide
  // Events include KeyDown, MouseDown etc.
  void Start();

  // Show the view.
  void ShowView(View *view);
  // Hide the view.
  void HideView(View *view);

  // Show all the view in a group.
  void ShowGroup(int groupid);
  // Show all the view in groups.
  void ShowGroup(std::initializer_list<int> groups);
  // Hide all the view in a group.
  void HideGroup(int groupid);
  // Hide all the view in groups.
  void HideGroup(std::initializer_list<int> groups);
  // Hide all the view in all group.
  void HideAllGroup();

  // Quit the SDL. Must be called when the game end.
  void Quit();

  // Set the background color of the window.
  void SetBackgroundColor(Uint8 r, Uint8 g, Uint8 b, Uint8 a);

  friend void SDL::RegisterWindow(Window &window) const;

 private:
  Context context_;

  std::string title_;
  int width_;
  int height_;
  int x_;
  int y_;
  int frame_rate_{60};
  SDL_Color bgcolor_{0xFF, 0xFF, 0xFF, 0xFF};

  // the second value determine whether the view is shown or not
  std::vector<std::pair<View *, bool>> views_;
  std::map<int, std::vector<View *>> groups_;

  // true to show, false to hide
  std::vector<std::pair<std::vector<std::pair<View *, bool>>::iterator, bool>>
      views_to_hide_or_show_;

  bool registered_{false};
  bool started_{false};
  bool quit_{false};
};
}  // namespace ccgame

#endif