#include "paintview.h"
namespace ccgame {
PaintView::PaintView(int x, int y, int width, int height) {
  SetPosition(x, y);
  SetClip(0, 0, width, height);
}

void PaintView::OnShow(Context& context) {
  texture_ =
      context.CreateTexture(SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET,
                            GetWidth(), GetHeight());
  texture_width_ = GetWidth();
  texture_height_ = GetHeight();

  TextureView::OnShow(context);
}

void PaintView::OnRender(Context& context) {
  if (!draw_cmds_.empty()) {
    context.SetRenderTarget(texture_);
    context.SetRenderDrawBlendMode(SDL_BLENDMODE_BLEND);

    for (auto cmd : draw_cmds_) {
      switch (cmd[0]) {
        case GEO_CLEAR: {
          context.SetRenderDrawColor(bgcolor_.r, bgcolor_.g, bgcolor_.b,
                                     bgcolor_.a);
          context.RenderClear();
          break;
        }
        case GEO_RECT: {
          context.SetRenderDrawColor(cmd[5], cmd[6], cmd[7], cmd[8]);
          SDL_Rect rect{cmd[1], cmd[2], cmd[3], cmd[4]};
          if (cmd[9] == 0) {
            context.RenderDrawRect(&rect);
          } else {
            context.RenderFillRect(&rect);
          }
          break;
        }
        case GEO_CIRCLE: {
          context.SetRenderDrawColor(cmd[4], cmd[5], cmd[6], cmd[7]);
          int x = cmd[1], y = cmd[2], radius = cmd[3];
          for (int w = 0; w < radius * 2; w++) {
            for (int h = 0; h < radius * 2; h++) {
              int dx = radius - w;  // horizontal offset
              int dy = radius - h;  // vertical offset
              if ((dx * dx + dy * dy) <= (radius * radius)) {
                context.RenderDrawPoint(x + dx, y + dy);
              }
            }
          }
          break;
        }
        case GEO_AACIRCLE: {
          int x = cmd[1], y = cmd[2], radius = cmd[3];
          int i = 0, j = radius, last_fade_amount = 0, fade_amount = 0;

          int MAX_OPAQUE = cmd[7];

          while (i < j) {
            double height = std::sqrt(std::max(radius * radius - i * i, 0));
            double fade_amount = MAX_OPAQUE * (std::ceil(height) - height);

            if (fade_amount < last_fade_amount) {
              // Opaqueness reset so drop down a row.
              j -= 1;
            }
            last_fade_amount = fade_amount;

            // The API needs integers, so convert here now we've checked if
            // it dropped.
            int fade_amount_i = int(fade_amount);

            // We're fading out the current j row, and fading in the next one
            // down.
            context.SetRenderDrawColor(cmd[4], cmd[5], cmd[6],
                                       MAX_OPAQUE - fade_amount_i);
            context.RenderDrawPoint(x + i, y + j);
            context.RenderDrawPoint(x + i, y - j);
            context.RenderDrawPoint(x - i, y + j);
            context.RenderDrawPoint(x - i, y - j);

            if (i != j) {
              context.RenderDrawPoint(x + j, y + i);
              context.RenderDrawPoint(x + j, y - i);
              context.RenderDrawPoint(x - j, y + i);
              context.RenderDrawPoint(x - j, y - i);
            }

            context.SetRenderDrawColor(cmd[4], cmd[5], cmd[6], fade_amount_i);
            int k = j - 1;
            context.RenderDrawPoint(x + i, y + k);
            context.RenderDrawPoint(x + i, y - k);
            context.RenderDrawPoint(x - i, y + k);
            context.RenderDrawPoint(x - i, y - k);
            if (i != k) {
              context.RenderDrawPoint(x + k, y + i);
              context.RenderDrawPoint(x + k, y - i);
              context.RenderDrawPoint(x - k, y + i);
              context.RenderDrawPoint(x - k, y - i);
            }
            i += 1;
          }
        }
        case GEO_AACIRCLE_FILL: {
          int x = cmd[1], y = cmd[2], radius = cmd[3];
          int i = 0, j = radius, last_fade_amount = 0, fade_amount = 0;

          int MAX_OPAQUE = cmd[7];

          while (i < j) {
            double height = std::sqrt(std::max(radius * radius - i * i, 0));
            double fade_amount = MAX_OPAQUE * (std::ceil(height) - height);

            if (fade_amount < last_fade_amount) {
              // Opaqueness reset so drop down a row.
              j -= 1;
            }
            last_fade_amount = fade_amount;

            // The API needs integers, so convert here now we've checked if
            // it dropped.
            int fade_amount_i = int(fade_amount);

            // We're fading out the current j row, and fading in the next one
            // down.
            context.SetRenderDrawColor(cmd[4], cmd[5], cmd[6],
                                       MAX_OPAQUE - fade_amount_i);
            context.RenderDrawPoint(x + i, y + j);
            context.RenderDrawPoint(x + i, y - j);
            context.RenderDrawPoint(x - i, y + j);
            context.RenderDrawPoint(x - i, y - j);

            if (i != j) {
              context.RenderDrawPoint(x + j, y + i);
              context.RenderDrawPoint(x + j, y - i);
              context.RenderDrawPoint(x - j, y + i);
              context.RenderDrawPoint(x - j, y - i);
            }
            context.SetRenderDrawColor(cmd[4], cmd[5], cmd[6], MAX_OPAQUE);
            if (j >= 1) {
              context.RenderDrawLine(x - i, y + j - 1, x + i, y + j - 1);
              context.RenderDrawLine(x - i, y - j + 1, x + i, y - j + 1);
            }
            if (j >= 1) {
              context.RenderDrawLine(x - j + 1, y + i, x + j - 1, y + i);
              context.RenderDrawLine(x - j + 1, y - i, x + j - 1, y - i);
            }

            context.SetRenderDrawColor(cmd[4], cmd[5], cmd[6], fade_amount_i);
            int k = j - 1;
            context.RenderDrawPoint(x + i, y + k);
            context.RenderDrawPoint(x + i, y - k);
            context.RenderDrawPoint(x - i, y + k);
            context.RenderDrawPoint(x - i, y - k);
            if (i != k) {
              context.RenderDrawPoint(x + k, y + i);
              context.RenderDrawPoint(x + k, y - i);
              context.RenderDrawPoint(x - k, y + i);
              context.RenderDrawPoint(x - k, y - i);
            }
            i += 1;
          }
        }
      }
    }
    draw_cmds_.clear();

    context.SetRenderTarget(nullptr);
  }

  TextureView::OnRender(context);
}

void PaintView::SetBackgroundColor(const SDL_Color& color) {
  bgcolor_ = color;
  Clear();
}
const SDL_Color& PaintView::GetBackgroundColor() const { return bgcolor_; }

void PaintView::Clear() { draw_cmds_.push_back(std::vector<int>{GEO_CLEAR}); }

void PaintView::DrawRect(const SDL_Rect& rect, bool solid,
                         const SDL_Color& color) {
  draw_cmds_.emplace_back(std::vector<int>{GEO_RECT, rect.x, rect.y, rect.w,
                                           rect.h, color.r, color.g, color.b,
                                           color.a, solid});
}

void PaintView::DrawRect(int x, int y, int w, int h, bool solid,
                         const SDL_Color& color) {
  SDL_Rect rect{x, y, w, h};
  DrawRect(rect, solid, color);
}

void PaintView::FillCircle(int x, int y, int radius, const SDL_Color& color) {
  draw_cmds_.emplace_back(std::vector<int>{GEO_CIRCLE, x, y, radius, color.r,
                                           color.g, color.b, color.a});
}

void PaintView::DrawAACircle(int x, int y, int radius, const SDL_Color& color) {
  draw_cmds_.emplace_back(std::vector<int>{GEO_AACIRCLE, x, y, radius, color.r,
                                           color.g, color.b, color.a});
}
void PaintView::FillAACircle(int x, int y, int radius, const SDL_Color& color) {
  draw_cmds_.emplace_back(std::vector<int>{GEO_AACIRCLE_FILL, x, y, radius,
                                           color.r, color.g, color.b, color.a});
}
}  // namespace ccgame