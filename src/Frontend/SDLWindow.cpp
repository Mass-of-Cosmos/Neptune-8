#include "SDLWindow.h"
#include <array>
#include <iostream>

void SDLWindow::CreateTexture()
{
    int width{64};
    int height{32};

    m_texture = SDL_CreateTexture(m_renderer, SDL_PIXELFORMAT_ABGR32, SDL_TEXTUREACCESS_STREAMING,width,height  );
    std::cerr << "creating SDL texture\n";
}

void SDLWindow::initilizeSDL()
{
    int screenWidth{1920};
    int screenHeight{1080};

    if( !SDL_Init( SDL_INIT_VIDEO ) )
    {
        SDL_Log( "SDL could not initialize! SDL Error: %s\n", SDL_GetError() );
    }
    else
    {
        if (!SDL_CreateWindowAndRenderer("Neptune 8",screenWidth,screenHeight,0, &m_window, &m_renderer ))
        {
            SDL_Log( "Renderer could not be created! SDL Error: %s\n", SDL_GetError() );
        }
    }

}
SDLWindow::SDLWindow()
{
    initilizeSDL();
    CreateTexture();
}
SDLWindow::~SDLWindow()
{
    SDL_DestroyTexture(m_texture);
    SDL_DestroyRenderer(m_renderer);
    SDL_DestroyWindow(m_window);
    SDL_Quit();
}

void SDLWindow::updateAndRenderTexture(const std::vector<uint8_t>& TextureData )
{
    // translate each pixel's color into 32bit rgba pixels:
    std::array<uint32_t, 64 * 32> pixel_buffer{};
    for (std::size_t i{0}; i < 64 * 32; ++i)
    {
        //                                        white        black
        pixel_buffer[i] = ( (TextureData[i] == 1) ? 0xFFFFFFFF : 0x000000FF );
    }

    // now lets load the pixel buffer into the renderer
    constexpr int pitch {64 * static_cast<int>(sizeof(uint32_t))};
    SDL_UpdateTexture(m_texture,nullptr, pixel_buffer.data(), pitch);

    // render present:
    SDL_RenderClear(m_renderer);
    SDL_RenderTexture(m_renderer, m_texture, nullptr, nullptr);
    SDL_RenderPresent(m_renderer);
}

void SDLWindow::clearScreen()
{
    SDL_RenderClear(m_renderer);
}

void SDLWindow::renderPresent()
{
    SDL_RenderPresent(m_renderer);
}