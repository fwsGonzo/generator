#ifndef LIBRARY_INPUT_HPP
#define LIBRARY_INPUT_HPP

#include <array>
#include <glm/vec2.hpp>
#include <string>

struct GLFWwindow;

namespace library
{
class Input
{
public:
    static const int MAX_KEYS = 512;
    static const int MAX_MOUSE = 6;

    enum keystate_t
    {
        KEY_RELEASED,
        KEY_PRESSED,
        KEY_LOCKED
    };

    typedef int key_t;

    struct input_t
    {
        input_t() : action(0), mods(0){};

        int action;
        int mods;
    };

    // initialize input using the current context
    void init(GLFWwindow*, bool keyboard, bool mouse);
    ~Input();

    // public keyboard functions
    input_t key_ex(int key) const { return this->m_keys.at(key); }
    key_t key(int key) const { return key_ex(key).action; }
    // "holds" a key, allowing code to see that a single-fire event has been executed already
    void key_hold(int key) { m_keys.at(key).action = KEY_LOCKED; }
    // releases the hold of a key, setting state from KEY_LOCKED back to KEY_PRESSED
    void key_release(int key)
    {
        if (m_keys.at(key).action == KEY_LOCKED) m_keys[key].action = KEY_PRESSED;
    }

    // public mouse functions
    // set mouse options
    void mouse_options(double speed, double sensitivity);
    // enable FPS-like mouse grabbing
    void mouse_grab(bool grab);
    // mouse position
    glm::vec2 mouse_xy() const { return this->m_mouse_xy; }
    input_t mouse_button_ex(int btn) const { return this->m_mouse.at(btn); }
    key_t mouse_button(int btn) const { return this->m_mouse.at(btn).action; }
    void mouse_hold(int btn) { this->m_mouse.at(btn).action = KEY_LOCKED; }
    // mouse rotation functions
    glm::vec2 rotation() const noexcept { return this->m_rot; }
    void set_rotation(glm::vec2 rot) { this->m_rot = rot; }
    void add_rotation(glm::vec2 rot) { this->m_rot += rot; }
    void rotate_degrees(glm::vec2 degrees);

    // returns mousewheel status _AND_ resets it internally
    int mouse_wheel() const; // 0 = no change, down < 0, up > 0

    void restore_keyboard();
    void restore_mouse();

    static void keyboard(GLFWwindow*, int key, int action, int a, int b);
    static void mouseMove(GLFWwindow*, double x, double y);
    static void mouseButton(GLFWwindow*, int button, int action, int mods);
    static void mouseWheel(GLFWwindow*, double x, double y);

protected:
    GLFWwindow* m_window = nullptr;
    double m_speed;
    double m_sensitivity;
    glm::vec2 m_mouse_xy;
    glm::vec2 m_last_mouse_xy;

    // pitch & yaw
    glm::vec2 m_rot;

    // keyboard keys
    std::array<input_t, MAX_KEYS> m_keys;
    // mouse buttons
    std::array<input_t, MAX_MOUSE> m_mouse;
    // mousewheel
    mutable int m_wheel = 0;
    // mouse grabbing mode
    bool m_mousegrab = false;

private:
    static Input* inputFromWindow(GLFWwindow*);
};
} // namespace library

#endif
