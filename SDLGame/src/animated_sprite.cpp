#include "animated_sprite.h"

#include <cmath>

void AnimatedSprite::render(Renderer &renderer)
{
    auto now = std::chrono::steady_clock::now();
    if (now - m_last_frame_time >= m_frame_time)
    {
        m_current_frame   = (m_current_frame + 1) % m_total_frames;
        m_last_frame_time = now;
    }

    renderer.draw_image(m_image_id,
                        m_current_frame * m_frame_size,
                        m_sprite_set * m_frame_size,
                        static_cast<std::int32_t>(std::round(x())),
                        static_cast<std::int32_t>(std::round(y())),
                        m_flip,
                        false,
                        m_frame_size,
                        m_frame_size,
                        static_cast<std::int32_t>(std::round(m_frame_size * scale_x())),
                        static_cast<std::int32_t>(std::round(m_frame_size * scale_y())));
}

void AnimatedSprite::set_sprite_set(std::int32_t sprite_set, bool flip)
{
    m_sprite_set = sprite_set;
    m_flip       = flip;
}

void AnimatedSprite::set_total_frames(std::int32_t total_frames, std::int32_t exclude_frames)
{
    m_total_frames = total_frames - exclude_frames;
    m_frame_size   = m_width / total_frames;
}

void AnimatedSprite::set_frame_time(std::chrono::milliseconds frame_time)
{
    m_frame_time = frame_time;
}

void AnimatedSprite::reset()
{
    m_current_frame   = 0;
    m_last_frame_time = std::chrono::steady_clock::now();
}

std::int32_t AnimatedSprite::current_frame() const
{
    return m_current_frame;
}

std::int32_t AnimatedSprite::total_frames() const
{
    return m_total_frames;
}
