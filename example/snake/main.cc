#include <SDL_filesystem.h>

#include <iostream>
#include <random>

#include "ccgame.h"

using namespace ccgame;

constexpr int WINDOW_WIDTH = 640;
constexpr int WINDOW_HEIGHT = 480;

enum PAGE { PAGE_MENU, PAGE_GAME, PAGE_PAUSE, PAGE_END };
std::random_device rd;

class Snake : public PaintView {
 public:
  Snake(int x, int y, int map_size, int unit_size, int game_tick_)
      : PaintView(x, y, map_size * unit_size, map_size * unit_size),
        unit_(unit_size),
        map_size_(map_size),
        game_tick_(game_tick_),
        fruit_(Coord{0, 0}),
        random_engine_(rd()) {
    distribution = std::uniform_int_distribution<int>(0, map_size_ - 1);
    game_status_ = STATUS_STOP;
  }

  void OnRender(Context &context) override {
    if (game_status_ == STATUS_RUN) {
      if (game_timer_.GetTicks() >= game_tick_) {
        OnGameTick();
        game_timer_.Restart();
        RenderGame();
      }
    }
    PaintView::OnRender(context);
  }

  void OnKeyDown(Context &context, SDL_Keycode keycode) override {
    switch (keycode) {
      case SDLK_w:
        if (direction_ != SOUTH) next_direction_ = NORTH;
        break;
      case SDLK_a:
        if (direction_ != EAST) next_direction_ = WEST;
        break;
      case SDLK_d:
        if (direction_ != WEST) next_direction_ = EAST;
        break;
      case SDLK_s:
        if (direction_ != NORTH) next_direction_ = SOUTH;
        break;
      default:
        break;
    }
  }

  void Start() {
    end_ = false;
    if (game_status_ == STATUS_STOP) {
      game_timer_.Start();
      game_status_ = STATUS_RUN;
      snake_.push_back(RandomCoord());
      do {
        fruit_ = RandomCoord();
      } while (InSnake(fruit_));
      RenderGame();
    }
  }
  void Pause() {
    if (game_status_ == STATUS_RUN) {
      game_timer_.Pause();
      game_status_ = STATUS_PAUSE;
    }
  }
  void UnPause() {
    if (game_status_ == STATUS_PAUSE) {
      game_timer_.Unpause();
      game_status_ = STATUS_RUN;
    }
  }
  void Stop() {
    if (game_status_ == STATUS_RUN || game_status_ == STATUS_PAUSE) {
      game_timer_.Stop();
      game_status_ = STATUS_STOP;

      snake_.clear();
    }
  }
  [[nodiscard]] bool IsEnd() const { return end_; }
  std::size_t GetScore() { return snake_.size() - 1; }

 private:
  struct Coord {
    int x;
    int y;
    bool operator==(const Coord &rval) const {
      return rval.x == x && rval.y == y;
    }
  };
  enum STATUS { STATUS_RUN, STATUS_STOP, STATUS_PAUSE };
  enum DIRECTION { NORTH, SOUTH, WEST, EAST };

  SDL_Color body_color_{0, 0xFF, 0, 0xFF};
  SDL_Color fruit_color_{0xFF, 0, 0, 0xFF};

  int unit_;
  int map_size_;
  int game_tick_;
  int game_status_;
  DIRECTION direction_{NORTH};
  DIRECTION next_direction_{NORTH};
  std::mt19937 random_engine_;
  std::uniform_int_distribution<int> distribution;
  Timer game_timer_;
  std::vector<Coord> snake_;
  Coord fruit_;
  bool end_{false};

  bool InSnake(const Coord &coord) {
    return std::any_of(snake_.begin(), snake_.end(),
                       [&](const auto &item) { return coord == item; });
  }

  void OnGameTick() {
    Coord head_next{snake_[0].x, snake_[0].y};
    direction_ = next_direction_;
    switch (direction_) {
      case NORTH:
        head_next.y--;
        break;
      case SOUTH:
        head_next.y++;
        break;
      case WEST:
        head_next.x--;
        break;
      case EAST:
        head_next.x++;
        break;
    }

    if (head_next.x < 0 || head_next.y < 0 || head_next.x >= map_size_ ||
        head_next.y >= map_size_ || InSnake(head_next)) {
      Pause();
      end_ = true;
      return;
    }

    if (head_next == fruit_) {
      do {
        fruit_ = RandomCoord();
      } while (InSnake(fruit_) || fruit_ == head_next);
      snake_.push_back({});
    }

    for (std::size_t i = snake_.size() - 1; i > 0; i--) {
      snake_[i] = snake_[i - 1];
    }
    snake_[0] = head_next;
  }

  void RenderGame() {
    Clear();
    for (const auto &c : snake_) {
      DrawRect(SDL_Rect{c.x * unit_, c.y * unit_, unit_, unit_}, true,
               body_color_);
    }
    int fruit_radius = unit_ / 2;
    FillAACircle(fruit_.x * unit_ + fruit_radius,
                 fruit_.y * unit_ + fruit_radius, fruit_radius, fruit_color_);
    // FillCircle(fruit_.x * unit_ + fruit_radius, fruit_.y * unit_ +
    // fruit_radius,
    //            fruit_radius, fruit_color_);
  }

  Coord RandomCoord() {
    return Coord{distribution(random_engine_), distribution(random_engine_)};
  }
};

int main([[maybe_unused]] int argc, [[maybe_unused]] char **argv) {
  try {
    SDL context{CCGAME_INIT_VIDEO, CCGAME_INIT_TTF};

    Window window("Snake", WINDOW_WIDTH, WINDOW_HEIGHT);

    Font joystix28(std::string(SDL_GetBasePath()) + "assets/joystix.ttf", 28);

    Snake game(WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2, 16, WINDOW_HEIGHT / 20,
               100);

    window.SetBackgroundColor(0xAA, 0xAA, 0xAA, 0xFF);

    // PAGE_MENU start
    RectView button1(SDL_Rect{WINDOW_WIDTH / 2, WINDOW_HEIGHT / 3,
                              WINDOW_WIDTH / 3, WINDOW_HEIGHT / 6},
                     true, SDL_Color{0x33, 0x33, 0xBB, 0xFF});
    button1.SetAlign(CCGAME_ALIGN_CENTER);

    button1.SetOnMouseButtonDownListener(
        [&](Context &context, Sint32 x, Sint32 y, Uint8 button) {
          if (button1.IsInside(x, y)) {
            window.HideAllGroup();
            window.ShowGroup(PAGE_GAME);
            game.Start();
          }
        });
    TextView text1("START", &joystix28, button1.GetX(), button1.GetY());
    text1.SetAlign(CCGAME_ALIGN_CENTER);

    RectView button2(SDL_Rect{WINDOW_WIDTH / 2, WINDOW_HEIGHT / 3 * 2,
                              WINDOW_WIDTH / 3, WINDOW_HEIGHT / 6},
                     true, SDL_Color{0xCC, 0x22, 0x22, 0xFF});
    button2.SetAlign(CCGAME_ALIGN_CENTER);
    button2.SetOnMouseButtonDownListener(
        [&](Context &context, Sint32 x, Sint32 y, Uint8 button) {
          if (button2.IsInside(x, y)) {
            window.Quit();
          }
        });

    TextView text2("QUIT", &joystix28, button2.GetX(), button2.GetY());
    text2.SetAlign(CCGAME_ALIGN_CENTER);

    window.RegisterView(&button1, PAGE_MENU);
    window.RegisterView(&text1, PAGE_MENU);
    window.RegisterView(&button2, PAGE_MENU);
    window.RegisterView(&text2, PAGE_MENU);
    // PAGE_MENU end

    // PAGE_PAUSE start
    game.SetAlign(CCGAME_ALIGN_CENTER);
    game.SetOnRenderListener([&](Context &context) {
      if (game.IsEnd()) {
        window.ShowGroup(PAGE_END);
      }
    });

    Font joystix14(std::string(SDL_GetBasePath()) + "assets/joystix.ttf", 14);
    TextView button3("PAUSE", &joystix14, 10, 10);

    button3.SetOnMouseButtonDownListener(
        [&](Context &context, Sint32 x, Sint32 y, Uint8 button) {
          if (button3.IsInside(x, y) && !game.IsEnd()) {
            window.ShowGroup(PAGE_PAUSE);
            game.Pause();
          }
        });

    TextView text_score("Score: 0", &joystix14, 10, 100);
    text_score.SetOnRenderListener([&](Context &context) {
      text_score.SetText(context, "Score: " + std::to_string(game.GetScore()));
    });

    window.RegisterView(&game, PAGE_GAME);
    window.RegisterView(&button3, PAGE_GAME);
    window.RegisterView(&text_score, PAGE_GAME);
    // PAGE_GAME end

    // PAGE_PAUSE start
    RectView rect1(SDL_Rect{0, 0, WINDOW_WIDTH, WINDOW_HEIGHT}, true,
                   SDL_Color{0xFF, 0xFF, 0xFF, 0xFF});
    rect1.SetBlendMode(SDL_BLENDMODE_BLEND);
    rect1.SetAlpha(0xCC);

    TextView button4("CONTINUE", &joystix28, WINDOW_WIDTH / 2,
                     WINDOW_HEIGHT / 3);
    button4.SetAlign(CCGAME_ALIGN_CENTER);
    button4.SetOnMouseButtonDownListener(
        [&](Context &context, Sint32 x, Sint32 y, Uint8 button) {
          if (button4.IsInside(x, y)) {
            window.HideGroup(PAGE_PAUSE);
            game.UnPause();
          }
        });

    TextView button5("EXIT", &joystix28, WINDOW_WIDTH / 2,
                     WINDOW_HEIGHT / 3 * 2);
    button5.SetAlign(CCGAME_ALIGN_CENTER);
    button5.SetOnMouseButtonDownListener(
        [&](Context &context, Sint32 x, Sint32 y, Uint8 button) {
          if (button5.IsInside(x, y)) {
            window.HideAllGroup();
            window.ShowGroup(PAGE_MENU);
            game.Stop();
          }
        });

    window.RegisterView(&rect1, PAGE_PAUSE);
    window.RegisterView(&button4, PAGE_PAUSE);
    window.RegisterView(&button5, PAGE_PAUSE);
    // PAGE_PAUSE end

    // PAGE_END start
    RectView rect2(SDL_Rect{0, 0, WINDOW_WIDTH, WINDOW_HEIGHT}, true,
                   SDL_Color{0xFF, 0xFF, 0xFF, 0xFF});
    rect2.SetBlendMode(SDL_BLENDMODE_BLEND);
    rect2.SetAlpha(0xCC);

    Font joystix20(std::string(SDL_GetBasePath()) + "assets/joystix.ttf", 20);
    TextView text3("YOU DIED!PRESS ANY KEY TO RESTART!", &joystix20,
                   WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2);
    text3.SetAlign(CCGAME_ALIGN_CENTER);
    text3.SetOnKeydownListener([&](Context &context, SDL_Keycode keycode) {
      game.Stop();
      game.Start();
      window.HideGroup(PAGE_END);
    });

    window.RegisterView(&rect2, PAGE_END);
    window.RegisterView(&text3, PAGE_END);
    // PAGE_END end

    // init status
    window.HideAllGroup();
    window.ShowGroup(PAGE_MENU);

    context.RegisterWindow(window);

    window.Start();

    return 0;
  } catch (std::runtime_error &e) {
    std::cout << e.what() << std::endl;
    return -1;
  }
}