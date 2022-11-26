#ifndef CCGAME_TIMER_h_
#define CCGAME_TIMER_h_
#include "SDL2/SDL.h"
namespace ccgame {
class Timer {
 public:
  Timer();

  void Start();
  void Stop();
  void Restart();
  void Pause();
  void Unpause();

  Uint32 GetTicks();

  bool IsStart();
  bool IsPause();

 private:
  Uint32 start_ticks_;
  Uint32 pause_ticks_;

  bool start_;
  bool pause_;
};
}  // namespace ccgame
#endif