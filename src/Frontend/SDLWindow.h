#ifndef SDLWINDOW_H
#define SDLWINDOW_H
#include <SDL3/SDL.h>
#include <vector>

class SDLWindow
{
private:
    SDL_Window* m_window{nullptr};
    SDL_Renderer* m_renderer{nullptr};
    SDL_Texture* m_texture{nullptr};
    void initilizeSDL();
    void CreateTexture();
public:
    SDLWindow();
    ~SDLWindow();

    void updateAndRenderTexture(const std::vector<uint8_t>& TextureData );
    void clearScreen();
    void renderPresent();
};


#endif
