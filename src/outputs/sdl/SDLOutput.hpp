#pragma once

#include "../../core/Output.hpp"
#include <SDL.h>
#include <SDL_ttf.h>
#include <map>
#include <set>
#include <vector>

namespace webrogue {
namespace outputs {
namespace sdl {

struct NRGlyphComparator {
    bool operator()(const nr_glyph &a, const nr_glyph &b) const;
};

struct ColorPair {
    int32_t fg;
    int32_t bg;
};

class SDLOutput : public webrogue::core::Output {
    std::chrono::steady_clock::time_point lastClick;
    std::chrono::steady_clock::time_point lastPress;
    std::set<SDL_Keycode> pressed;
    std::map<SDL_FingerID, Vec2Int> fingerMap;

public:
    SDLOutput();
    int topInset = 0;
    ~SDLOutput() override;

protected:
    void pollEvents();

    bool isKeyboardAvailable() override;

    void startColor() override;

    int32_t getColorPairsCount() override;

    int32_t getColorsCount() override;

    void setColor(int32_t color, int32_t r, int32_t g, int32_t b) override;

    void setColorPair(int32_t colorPair, int32_t fg, int32_t bg) override;

    void onBegin() override;

    void onEnd() override;

    void onBeginFrame() override;

    void onEndFrame() override;

    void onLazyEnd() override;

    void drawInputToTexture();

    void refreshSize();

    void presentTexture();

    void fpsSync();

    SDL_Renderer *renderer;
    SDL_Window *window;
    bool windowSizeChanged = true;
    int previousWindowHeight = -1;
    int previousWindowWidth = -1;
    TTF_Font *font = nullptr;
    int fontWidth, fontHeight;
    int dx, dy;
    std::map<nr_glyph, SDL_Texture *, NRGlyphComparator> glyph2tex;
    std::vector<SDL_Color> colormap;
    std::vector<ColorPair> colorPairs;
    bool lastInputValid;
    Buffer2d<nr_glyph> lastInput;
    SDL_Texture *lastTexture = nullptr;
    uint32_t lastUpdateTick = 0;
    Uint64 start = SDL_GetPerformanceCounter();
};
} // namespace sdl
} // namespace outputs
} // namespace webrogue
