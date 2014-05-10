/**
 * Simple window renderer/manager
 * 
**/

#ifndef LIBRARY_WINDOW_HPP
#define LIBRARY_WINDOW_HPP

#include <string>
struct GLFWwindow;

namespace library
{
	struct WindowConfig
	{
	public:
		WindowConfig();
		WindowConfig(std::string title, int width, int height);
		
		// values used to create a GLFW window
		std::string title;
		bool fullscreen;
		bool vsync;
		int refreshrate;
		int SW, SH;
		int multisample;
	};
	
	class WindowClass;
	
	// RenderClass: inherit from your Renderer class, so that you can pass 'this'
	// to the startRenderingLoop function when you want to start rendering.
	class RenderClass
	{
	protected:
		// prototype for rendering function
		virtual bool render(WindowClass& wnd, double dtime, double timeElapsed) = 0;
		
		friend class WindowClass;
	};
	
	class WindowClass
	{
	private:
		static bool init;
		bool closing = false;
		GLFWwindow* wndHandle;
		
	public:
		int SW, SH; // window size
		float SA;   // window aspect
		bool fullscreen;
		
		// opens an OpenGL context window
		void open(const WindowConfig& wndconf);
		// closes an opened window (must be called, at some point)
		void close();
		// wait for window to close
		void waitClose();
		
		// returns glfw window handle, wide usage with glfw calls
		GLFWwindow* window();
		// set window title to <string>
		void setTitle(std::string);
		// set window position on screen to (x, y)
		void setPosition(int x, int y);
		// make this window the current context
		void setCurrent();
		// starts a running rendering loop, that executes renderFunc
		// each frame, until renderFunc returns false
		void startRenderingLoop(RenderClass* rclass);
		// Granularity: What we want delta-time to be measured in.
		// 1ms granularity, meaning 1.0 dtime is 1ms, 100.0 is 100ms etc.
		// Examples: 1/1000 = 1ms/frame, 1/60 = 16.7ms/frame (60fps) etc.
		// And finally, 1.0 means dtime is measured in (whole) seconds
		void startRenderingLoop(RenderClass* rclass, double granularity);
	};
	
}

#endif
