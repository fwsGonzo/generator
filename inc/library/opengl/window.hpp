/**
 * Simple window renderer/manager
 *
 **/

#ifndef LIBRARY_WINDOW_HPP
#define LIBRARY_WINDOW_HPP

#include <glm/vec2.hpp>
#include <string>
struct GLFWwindow;

namespace library
{
//! \brief class used in window creation.
//!        most values are initialized to sane defaults
class WindowConfig
{
public:
    WindowConfig();
    WindowConfig(std::string title, int width, int height);

    //! values used to create a GLFW window
    std::string title;
    bool fullscreen;
    bool vsync;
    int refreshrate;
    int SW, SH;
    int multisample;
    int depthbits;
    int stencbits;

    bool core_context;
    int ver_major;
    int ver_minor;
};

class WindowClass;

// RenderClass: inherit from your Renderer class, so that you can pass 'this'
// to the startRenderingLoop function when you want to start rendering.
class RenderClass
{
protected:
    //! \brief prototype for rendering callback function
    //! \param dtime is the time elapsed between two rendered frames
    //! \param timeElapsed is the total time since rendering began
    //! \warning averaging 1.0 / dtime can be used as a decent FPS timer
    virtual bool render(WindowClass& wnd, double dtime, double timeElapsed) = 0;

    friend class WindowClass;
};

class WindowClass
{
public:
    //! \brief opens an OpenGL context window
    void open(const WindowConfig& wndconf);
    //! \brief closes an opened window (must be called, at some point)
    void close();
    //! \brief wait for an opened window to close
    void waitClose();

    //! \brief returns true if the window is running in fullscreen
    //! \warning or at least, if it thinks it does
    bool isFullscreen() const { return fullscreen; }

    bool isCoreContext() const { return is_core_context; }

    //! \brief returns the width of the glfw window
    int getWidth() const noexcept;
    //! \brief returns the height of the glfw window
    int getHeight() const noexcept;
    //! \brief returns the width, height of the glfw window
    glm::vec2 getSize() const noexcept;
    //! \brief returns the window aspect, calculated as (width / height)
    //! \warning the commonly used inverse aspect is 1.0 / getAspect()
    float getAspect() const noexcept;

    //! \brief returns the glfw window handle to this window
    //! \warning the value is undefined if the window is not opened
    GLFWwindow* window();
    //! \brief set window title to <string>
    void setTitle(std::string const& title);
    //! \brief set window position on screen to (x, y)
    void setPosition(int x, int y);
    //! \brief make this window the current context
    void setCurrent();
    //! \brief starts a running rendering loop, that executes RenderClass::render()
    //!        each frame, until render() returns false
    void startRenderingLoop(RenderClass& rclass);

private:
    static bool init;
    bool closing = false;
    bool fullscreen;
    bool is_core_context;

    float SA; //! screen aspect
    GLFWwindow* wndHandle;
};

} // namespace library

#endif
