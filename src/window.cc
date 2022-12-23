#include "window.h"

#include <utility>

#include "context.h"
#include "view.h"
namespace ccgame {
Window::Window(std::string title, int width, int height, int x, int y)
    : title_(std::move(title)),
      width_(width),
      height_(height),
      x_(x),
      y_(y),
      context_(nullptr, nullptr) {}

void Window::RegisterView(View *view, int groupid) {
  if (view != nullptr) {
    views_.push_back(view);
  }
  groups_[groupid].push_back(view);
}

void Window::Start() {
  for (auto v : views_) {
    if (v->hidden_) continue;
    v->OnCreate(context_);
    if (v->on_create_listener_ != nullptr) v->on_create_listener_(context_);
  }

  SDL_Event e{};
  while (!quit_) {
    for (auto g : groups_to_hide_or_show_) {
      for (auto v : groups_[g.first]) {
        if (g.second) {
          v->Show();
        } else {
          v->Hide();
        }
      }
    }
    groups_to_hide_or_show_.clear();

    while (SDL_PollEvent(&e)) {
      switch (e.type) {
        case SDL_KEYDOWN:
          for (auto v : views_) {
            if (v->hidden_) continue;
            v->OnKeyDown(context_, e.key.keysym.sym);
            if (v->on_keydown_listener_ != nullptr)
              v->on_keydown_listener_(context_, e.key.keysym.sym);
          }
          break;
        case SDL_MOUSEBUTTONDOWN:
          for (auto v : views_) {
            if (v->hidden_) continue;
            v->OnMouseButtonDown(context_, e.button.x, e.button.y,
                                 e.button.button);
            if (v->on_mousebuttondown_listener_ != nullptr)
              v->on_mousebuttondown_listener_(context_, e.button.x, e.button.y,
                                              e.button.button);
          }
          break;
        case SDL_MOUSEMOTION:
          for (auto v : views_) {
            if (v->hidden_) continue;
            v->OnMouseMotion(context_, e.motion.x, e.motion.y);
            if (v->on_mousemotion_listener_ != nullptr) {
              v->on_mousemotion_listener_(context_, e.motion.x, e.motion.y);
            }
          }
          break;
        case SDL_QUIT:
          quit_ = true;
      }
    }

    context_.SetRenderDrawColor(bgcolor_.r, bgcolor_.g, bgcolor_.b, bgcolor_.a);
    context_.RenderClear();

    for (auto v : views_) {
      if (v->hidden_) continue;
      v->OnRender(context_);
      if (v->on_render_listener_ != nullptr) v->on_render_listener_(context_);
    }

    context_.RenderPresent();
  }
  for (auto v : views_) {
    if (v->hidden_) continue;
    v->OnQuit(context_);
    if (v->on_quit_listener_ != nullptr) v->on_quit_listener_(context_);
  }
}

void Window::ShowGroup(int groupid) {
  groups_to_hide_or_show_.emplace_back(groupid, true);
}
void Window::ShowGroup(std::initializer_list<int> groups) {
  for (auto i : groups) {
    ShowGroup(i);
  }
}

void Window::HideGroup(int groupid) {
  groups_to_hide_or_show_.emplace_back(groupid, false);
}
void Window::HideGroup(std::initializer_list<int> groups) {
  for (auto i : groups) {
    HideGroup(i);
  }
}
void Window::HideAllGroup() {
  for (const auto& group : groups_) {
    HideGroup(group.first);
  }
}

void Window::Quit() { quit_ = true; }

void Window::SetBackgroundColor(Uint8 r, Uint8 g, Uint8 b, Uint8 a) {
  bgcolor_.r = r;
  bgcolor_.g = g;
  bgcolor_.b = b;
  bgcolor_.a = a;
}

}  // namespace ccgame