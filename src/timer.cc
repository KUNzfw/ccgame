#include "timer.h"

namespace ccgame {
Timer::Timer()
    : start_ticks_(0), pause_ticks_(0), start_(false), pause_(false) {}

void Timer::Start() {
  if (!start_) {
    start_ = true;
    start_ticks_ = SDL_GetTicks();
  }
}
void Timer::Stop() {
  start_ = false;
  pause_ = false;
  start_ticks_ = 0;
  pause_ticks_ = 0;
}
void Timer::Restart() {
  Stop();
  Start();
}
void Timer::Pause() {
  if (start_ && !pause_) {
    pause_ = true;
    pause_ticks_ = SDL_GetTicks();
  }
}
void Timer::Unpause() {
  if (start_ && pause_) {
    pause_ = false;
    start_ticks_ = start_ticks_ + (SDL_GetTicks() - pause_ticks_);
    pause_ticks_ = 0;
  }
}

Uint32 Timer::GetTicks() const {
  if (start_) {
    if (pause_) {
      return pause_ticks_ - start_ticks_;
    } else {
      return SDL_GetTicks() - start_ticks_;
    }
  } else {
    return 0;
  }
}
bool Timer::IsStart() const { return start_; }
bool Timer::IsPause() const { return pause_; }
}  // namespace ccgame