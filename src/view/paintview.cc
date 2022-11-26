#include "paintview.h"
namespace ccgame {
PaintView::PaintView(int x, int y, int width, int height) {
  SetPosition(x, y);
  SetClip(0, 0, width, height);
}

void PaintView::OnCreate(Context& context) {
  texture_ =
      context.CreateTexture(SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET,
                            GetWidth(), GetHeight());
  texture_width_ = GetWidth();
  texture_height_ = GetHeight();

  TextureView::OnCreate(context);
}

void PaintView::OnRender(Context& context) {
  if (!draw_cmds_.empty()) {
    context.SetRenderTarget(texture_);
    context.SetRenderDrawBlendMode(SDL_BLENDMODE_BLEND);

    for (auto cmd : draw_cmds_) {
      switch (cmd[0]) {
        case GEO_CLEAR:
          context.SetRenderDrawColor(bgcolor_.r, bgcolor_.g, bgcolor_.b,
                                     bgcolor_.a);
          context.RenderClear();
          break;
        case GEO_RECT:
          context.SetRenderDrawColor(cmd[5], cmd[6], cmd[7], cmd[8]);
          SDL_Rect rect{cmd[1], cmd[2], cmd[3], cmd[4]};
          if (cmd[9] == 0) {
            context.RenderDrawRect(&rect);
          } else {
            context.RenderFillRect(&rect);
          }
          break;
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
  draw_cmds_.push_back(std::vector<int>{GEO_RECT, rect.x, rect.y, rect.w,
                                        rect.h, color.r, color.g, color.b,
                                        color.a, solid});
}
}  // namespace ccgame