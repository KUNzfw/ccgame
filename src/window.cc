#include "window.h"

#include <stdexcept>
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

void Window::RegisterView(View* view, int groupid) {
  if (view != nullptr) {
    views_.emplace_back(view, true);
  }
  groups_[groupid].push_back(view);
}

void Window::Start() {
  if (!registered_) {
    throw std::logic_error("Haven't registered this window in SDL!");
  }

  started_ = true;

  auto performance_frequency{SDL_GetPerformanceFrequency()};

  for (const auto& [view, show] : views_) {
    if (!show) continue;
    view->OnShow(context_);
    if (view->on_show_listener_) view->on_show_listener_(context_);
  }

  SDL_Event e{};
  while (!quit_) {
    auto start_time{SDL_GetPerformanceCounter()};

    for (const auto& [view, show] : views_to_hide_or_show_) {
      if (view->second == show) {
        continue;
      }

      if (show) {
        view->second = true;
        view->first->OnShow(context_);
        if (view->first->on_show_listener_) {
          view->first->on_show_listener_(context_);
        }
      } else {
        view->second = false;
        view->first->OnHide(context_);
        if (view->first->on_hide_listener_) {
          view->first->on_hide_listener_(context_);
        }
      }
    }
    views_to_hide_or_show_.clear();

    while (SDL_PollEvent(&e)) {
      switch (e.type) {
        case SDL_KEYDOWN:
          for (const auto& [view, show] : views_) {
            if (!show) continue;
            view->OnKeyDown(context_, e.key.keysym.sym);
            if (view->on_keydown_listener_ != nullptr)
              view->on_keydown_listener_(context_, e.key.keysym.sym);
          }
          break;
        case SDL_MOUSEBUTTONDOWN:
          for (const auto& [view, show] : views_) {
            if (!show) continue;
            view->OnMouseButtonDown(context_, e.button.x, e.button.y,
                                    e.button.button);
            if (view->on_mousebuttondown_listener_ != nullptr)
              view->on_mousebuttondown_listener_(context_, e.button.x,
                                                 e.button.y, e.button.button);
          }
          break;
        case SDL_MOUSEMOTION:
          for (const auto& [view, show] : views_) {
            if (!show) continue;
            view->OnMouseMotion(context_, e.motion.x, e.motion.y);
            if (view->on_mousemotion_listener_ != nullptr) {
              view->on_mousemotion_listener_(context_, e.motion.x, e.motion.y);
            }
          }
          break;
        case SDL_QUIT:
          quit_ = true;
      }
    }

    context_.SetRenderDrawColor(bgcolor_.r, bgcolor_.g, bgcolor_.b, bgcolor_.a);
    context_.RenderClear();

    for (const auto& [view, show] : views_) {
      if (!show) continue;
      view->OnRender(context_);
      if (view->on_render_listener_ != nullptr) {
        view->on_render_listener_(context_);
      }
    }

    context_.RenderPresent();

    auto end_time{SDL_GetPerformanceCounter()};

    // limit the framerate
    double elapsed_ms{static_cast<double>(end_time - start_time) /
                      static_cast<double>(performance_frequency) * 1000.0};
    double delay_ms = floor(1000.0 / frame_rate_ - elapsed_ms);
    if (delay_ms > 0) {
      SDL_Delay(static_cast<Uint32>(delay_ms));
    }
  }
  for (const auto& [view, show] : views_) {
    if (!show) continue;
    view->OnHide(context_);
    if (view->on_hide_listener_ != nullptr) {
      view->on_hide_listener_(context_);
    }
  }
}

void Window::ShowView(View* view) {
  // do nothing when the view is not registered
  auto view_iterator{std::find_if(views_.begin(), views_.end(),
                                  [&](auto val) { return val.first == view; })};

  if (view_iterator == views_.end()) {
    return;
  }
  // different before and after start
  if (started_) {
    auto p = std::find_if(views_to_hide_or_show_.begin(),
                          views_to_hide_or_show_.end(),
                          [&](auto val) { return val.first == view_iterator; });
    if (p == views_to_hide_or_show_.end()) {
      views_to_hide_or_show_.emplace_back(view_iterator, true);
    } else {
      p->second = true;
    }
  } else {
    view_iterator->second = true;
  }
}
void Window::HideView(View* view) {
  // do nothing when the view is not registered
  auto view_iterator{std::find_if(views_.begin(), views_.end(),
                                  [&](auto val) { return val.first == view; })};

  if (view_iterator == views_.end()) {
    return;
  }
  // different before and after start
  if (started_) {
    auto p = std::find_if(views_to_hide_or_show_.begin(),
                          views_to_hide_or_show_.end(),
                          [&](auto val) { return val.first == view_iterator; });
    if (p == views_to_hide_or_show_.end()) {
      views_to_hide_or_show_.emplace_back(view_iterator, false);
    } else {
      p->second = false;
    }
  } else {
    view_iterator->second = false;
  }
}

void Window::ShowGroup(int groupid) {
  for (auto i : groups_[groupid]) {
    ShowView(i);
  }
}
void Window::ShowGroup(std::initializer_list<int> groups) {
  for (auto i : groups) {
    ShowGroup(i);
  }
}

void Window::HideGroup(int groupid) {
  for (auto i : groups_[groupid]) {
    HideView(i);
  }
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

void Window::Quit() {
  started_ = false;
  quit_ = true;
}

void Window::SetBackgroundColor(Uint8 r, Uint8 g, Uint8 b, Uint8 a) {
  bgcolor_.r = r;
  bgcolor_.g = g;
  bgcolor_.b = b;
  bgcolor_.a = a;
}

}  // namespace ccgame