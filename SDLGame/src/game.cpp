#include "game.h"
#include "events.h"
#include "renderer.h"
#if __has_include("resources.h")
#include "resources.h"
#endif

#include <fmt/format.h>

Game::~Game() = default;

void Game::load_assets(Renderer &renderer)
{
    m_font_ms_dos = renderer.load_font(resource_font_ms_dos, resource_font_ms_dos_size);
}

void Game::render(Renderer &renderer, const RenderEvent &event)
{
    renderer.set_color({ 0, 0, 255, 255 });
    renderer.clear();

    renderer.draw_text(m_font_ms_dos, "Hello, World!", 10, 10, { 255, 255, 255, 255 }, 24);

    fps_timer += event.seconds_elapsed;
    total_seconds_elapsed += event.seconds_elapsed;
    ++frame_counter;

    if (fps_timer >= 1.0F)
    {
        fmt::print("FPS: {}\n", frame_counter);
        fps_timer     = fps_timer - 1.0F;
        frame_counter = 0;
    }
}

void Game::on_window_shown()
{}

void Game::on_window_hidden()
{}

void Game::on_window_moved(const WindowMoveEvent &event)
{}

void Game::on_window_resized(const WindowResizeEvent &event)
{}

void Game::on_window_focus_gained()
{}

void Game::on_window_focus_lost()
{}

void Game::on_window_closed()
{}

void Game::on_window_mouse_entered()
{}

void Game::on_window_mouse_left()
{}

void Game::on_mouse_moved(const MouseMoveEvent &event)
{}

void Game::on_mouse_button_pressed(const MouseButtonPressEvent &event)
{}

void Game::on_mouse_button_released(const MouseButtonReleaseEvent &event)
{}

void Game::on_mouse_wheel(const MouseWheelEvent &event)
{}

void Game::on_key_pressed(const KeyPressEvent &event)
{}

void Game::on_key_released(const KeyReleaseEvent &event)
{}

bool Game::is_mouse_button_pressed(MouseButton button) const
{
    return m_mouse_button_states[static_cast<std::size_t>(button)] != 0;
}

bool Game::is_key_pressed(KeyCode key_code) const
{
    return m_keyboard_state[static_cast<std::size_t>(key_code)] != 0;
}
