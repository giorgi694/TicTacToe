#include "renderer.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include <SDL_ttf.h>

Renderer::Image::Image(SDL_Renderer *renderer, const std::uint8_t *data, std::size_t size)
{
    std::int32_t channels;
    auto         pixels =
        stbi_load_from_memory(data, static_cast<std::int32_t>(size), &width, &height, &channels, 4);

    assert(pixels != nullptr);
    assert(channels == 4);

    auto *surface = SDL_CreateRGBSurfaceWithFormatFrom(pixels,
                                                       width,
                                                       height,
                                                       32,
                                                       width * 4,
                                                       SDL_PIXELFORMAT_RGBA32);
    assert(surface != nullptr);

    texture = SDL_CreateTextureFromSurface(renderer, surface);
    assert(texture != nullptr);

    SDL_FreeSurface(surface);
    stbi_image_free(pixels);
}

Renderer::Image::~Image()
{
    if (texture != nullptr)
    {
        SDL_DestroyTexture(texture);
    }
}

Renderer::Image::Image(Image &&other)
  : width{ other.width }
  , height{ other.height }
  , texture{ other.texture }
{
    other.texture = nullptr;
}

Renderer::Image &Renderer::Image::operator=(Image &&other)
{
    width   = other.width;
    height  = other.height;
    texture = other.texture;

    other.texture = nullptr;

    return *this;
}

Renderer::Renderer(SDL_Window *window)
  : m_renderer{ SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED) }
{
    SDL_SetRenderDrawBlendMode(m_renderer, SDL_BLENDMODE_BLEND);
}

Renderer::~Renderer()
{
    SDL_DestroyRenderer(m_renderer);
}

std::int32_t Renderer::width() const
{
    std::int32_t width;
    SDL_GetRendererOutputSize(m_renderer, &width, nullptr);
    return width;
}

std::int32_t Renderer::height() const
{
    std::int32_t height;
    SDL_GetRendererOutputSize(m_renderer, nullptr, &height);
    return height;
}

std::int32_t Renderer::load_image(const std::uint8_t *data,
                                  std::size_t         size,
                                  std::int32_t       *width,
                                  std::int32_t       *height)
{
    m_images.emplace_back(this->m_renderer, data, size);

    if (width != nullptr)
    {
        *width = m_images.back().width;
    }

    if (height != nullptr)
    {
        *height = m_images.back().height;
    }

    return static_cast<std::int32_t>(m_images.size()) - 1;
}

std::int32_t Renderer::load_font(const std::uint8_t *data, std::size_t size, std::int32_t ptsize)
{
    auto *font =
        TTF_OpenFontRW(SDL_RWFromConstMem(data, static_cast<std::int32_t>(size)), 1, ptsize);
    assert(font != nullptr);

    const auto id = static_cast<std::int32_t>(m_fonts.size());
    m_fonts.push_back(font);

    return id;
}

void Renderer::set_color(const Color &color)
{
    SDL_SetRenderDrawColor(m_renderer, color.r, color.g, color.b, color.a);
}

void Renderer::clear()
{
    SDL_RenderClear(m_renderer);
}

void Renderer::put_pixel(std::int32_t x, std::int32_t y)
{
    SDL_RenderDrawPoint(m_renderer, x, y);
}

void Renderer::draw_line(std::int32_t x0, std::int32_t y0, std::int32_t x1, std::int32_t y1)
{
    SDL_RenderDrawLine(m_renderer, x0, y0, x1, y1);
}

void Renderer::draw_rect(std::int32_t x, std::int32_t y, std::int32_t width, std::int32_t height)
{
    SDL_Rect rect{ x, y, width, height };
    SDL_RenderDrawRect(m_renderer, &rect);
}

void Renderer::fill_rect(std::int32_t x, std::int32_t y, std::int32_t width, std::int32_t height)
{
    SDL_Rect rect{ x, y, width, height };
    SDL_RenderFillRect(m_renderer, &rect);
}

void Renderer::draw_image(std::int32_t image_id,
                          std::int32_t src_x,
                          std::int32_t src_y,
                          std::int32_t dest_x,
                          std::int32_t dest_y,
                          bool         flip_x,
                          bool         flip_y,
                          std::int32_t src_width,
                          std::int32_t src_height,
                          std::int32_t dest_width,
                          std::int32_t dest_height)
{
    const Image &image = m_images.at(image_id);

    if (src_width == -1)
    {
        src_width = image.width - src_x;
    }
    if (src_height == -1)
    {
        src_height = image.height - src_y;
    }

    if (dest_width == -1)
    {
        dest_width = src_width;
    }
    if (dest_height == -1)
    {
        dest_height = src_height;
    }

    SDL_RendererFlip flip = SDL_FLIP_NONE;

    if (flip_x)
    {
        flip = SDL_FLIP_HORIZONTAL;
    }

    if (flip_y)
    {
        flip = static_cast<SDL_RendererFlip>(flip | SDL_FLIP_VERTICAL);
    }

    const auto srcRect  = SDL_Rect{ src_x, src_y, src_width, src_height };
    const auto destRect = SDL_Rect{ dest_x, dest_y, dest_width, dest_height };
    SDL_RenderCopyEx(m_renderer, image.texture, &srcRect, &destRect, 0, nullptr, flip);
}

void Renderer::draw_text(std::int32_t     font_id,
                         std::string_view text,
                         std::int32_t     x,
                         std::int32_t     y,
                         const Color     &color,
                         std::int32_t     pt_size)
{
    if (text.empty())
    {
        return;
    }

    SDL_Texture *texture = nullptr;
    std::int32_t width;
    std::int32_t height;

    auto cached_text = m_text_cache.find(text.data());
    if (cached_text == m_text_cache.end())
    {
        auto *font = m_fonts.at(font_id);
        TTF_SetFontSize(font, pt_size);

        SDL_Color sdl_color{ color.r, color.g, color.b, color.a };
        auto     *surface =
            TTF_RenderUTF8_Blended(font, text.data(), { color.r, color.g, color.b, color.a });
        assert(surface != nullptr);

        texture = SDL_CreateTextureFromSurface(m_renderer, surface);
        assert(texture != nullptr);

        width  = surface->w;
        height = surface->h;

        m_text_cache.emplace(text.data(),
                             TextCacheEntry{ font_id,
                                             color,
                                             pt_size,

                                             surface->w,
                                             surface->h,

                                             std::unique_ptr<SDL_Texture, void (*)(SDL_Texture *)>{
                                                 texture,
                                                 SDL_DestroyTexture },

                                             std::chrono::steady_clock::now() });

        SDL_FreeSurface(surface);
    }
    else
    {
        if (cached_text->second.font_id != font_id || cached_text->second.color != color ||
            cached_text->second.pt_size != pt_size)
        {
            m_text_cache.erase(cached_text);
            draw_text(font_id, text, x, y, color, pt_size);

            return;
        }

        texture                         = cached_text->second.texture.get();
        width                           = cached_text->second.width;
        height                          = cached_text->second.height;
        cached_text->second.last_access = std::chrono::steady_clock::now();

        std::vector<std::string_view> keys_to_erase;
        keys_to_erase.reserve(m_text_cache.size());
        for (const auto &[key, value] : m_text_cache)
        {
            if (std::chrono::steady_clock::now() - value.last_access > std::chrono::seconds{ 60 })
            {
                keys_to_erase.push_back(key);
            }
        }

        for (const auto &key : keys_to_erase)
        {
            m_text_cache.erase(key.data());
        }
    }

    SDL_Rect rect{ x, y, width, height };
    SDL_RenderCopy(m_renderer, texture, nullptr, &rect);
}

void Renderer::present()
{
    SDL_RenderPresent(m_renderer);
}
