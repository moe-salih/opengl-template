#pragma once
#ifndef OGL_CORE_WINDOW_HPP
#define OGL_CORE_WINDOW_HPP

#include "basic_window.hpp"

#include "../events/application-event.hpp"
#include "../events/mouse-event.hpp"
#include "../events/key-evet.hpp"

#include "../utils/ogl_debug.hpp"
#include <GLFW/glfw3.h>

#ifdef _WIN32
    #define GLFW_EXPOSE_NATIVE_WIN32
    #include <GLFW/glfw3native.h>
    #include <dwmapi.h>
#endif


#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

namespace ogl {

    struct callbacks {
        static void window_resize_callback(GLFWwindow* window, int width, int height);
        static void window_close_callback(GLFWwindow* window);
        static void window_focus_callback(GLFWwindow* window, int focused);
        static void window_pos_callback(GLFWwindow* window, int xpos, int ypos);
        
        static void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);
        static void cursor_pos_callback(GLFWwindow* window, double xpos, double ypos);
        static void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
        
        static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
        static void char_callback(GLFWwindow* window, unsigned int codepoint);  
    
        static void glfw_error_callback(int error, const char* description);
    };

    struct window_data {
        const char* title;
        
        vec2u size;
        vec2u viewport_size;

        vec2i position;

        bool vsync;
        bool fullscreen;
        
        basic_window::event_callback_fn event_callback;

        GLFWwindow*     handle;
        GLFWmonitor*    monitor;
    } m_data;

    struct window : public basic_window {
        public:
            window( const basic_window_props& props = basic_window_props() );
            virtual ~window();

            void on_update(const float&) override;

            inline vec2u size() const override { return m_data.size; }
            
            inline void set_event_callback(const event_callback_fn& callback) override { m_data.event_callback = callback; }
            
            void set_vsync(bool enabled) override;
            void set_icon(const char* path) override;

            inline bool is_vsync() const override { return m_data.vsync; }

            inline virtual void* native_window() const override { return m_data.handle; }
        private:
            virtual void init(const basic_window_props& props);
            virtual void shutdown();
        private:
            
    };

    static bool s_glfw_initialized = false;

    basic_window* basic_window::create( const basic_window_props& props ) {
        return new window(props);
    }

    window::window( const basic_window_props& props ) {
        init(props);
    }

    window::~window() {
        shutdown();
    }

    void window::init( const basic_window_props& props) {
        m_data.title = props.title;
        m_data.size = props.size;

        if(!s_glfw_initialized) {
            int success = glfwInit();
            OGL_ASSERT(success, "Failed to initialize GLFW!");
            glfwSetErrorCallback(callbacks::glfw_error_callback);
            s_glfw_initialized = true;
        }

        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

        m_data.handle = glfwCreateWindow(m_data.size.x, m_data.size.y, m_data.title, nullptr, nullptr);

        /*
            SETUP DARK THEME FOR THE TITLE BAR IN WINDOWS
        */
        #ifdef _WIN32
            DWORD value = TRUE;
            DwmSetWindowAttribute(::glfwGetWin32Window(m_data.handle), DWMWA_USE_IMMERSIVE_DARK_MODE, &value, sizeof(value));
        #endif


        glfwMakeContextCurrent(m_data.handle);

        int status = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
        OGL_ASSERT(status, "Failed to initialize Glad!");

        const char* vendor = (const char*)glGetString(GL_VENDOR);
        const char* renderer = (const char*)glGetString(GL_RENDERER);
        const char* version = (const char*)glGetString(GL_VERSION);

        LOG_INFO("OpenGL Info:");
        LOG_INFO("  Vendor: {0}", vendor);
        LOG_INFO("  Renderer: {0}", renderer);
        LOG_INFO("  Version: {0}", version);


        glfwSetWindowUserPointer(m_data.handle, &m_data);
        set_vsync(true);

        glfwSetWindowSizeCallback(m_data.handle, callbacks::window_resize_callback);
        glfwSetWindowCloseCallback(m_data.handle, callbacks::window_close_callback);
        glfwSetWindowFocusCallback(m_data.handle, callbacks::window_focus_callback);
        glfwSetWindowPosCallback(m_data.handle, callbacks::window_pos_callback);
        glfwSetKeyCallback(m_data.handle, callbacks::key_callback);
        glfwSetCharCallback(m_data.handle, callbacks::char_callback);
        glfwSetMouseButtonCallback(m_data.handle, callbacks::mouse_button_callback);
        glfwSetCursorPosCallback(m_data.handle, callbacks::cursor_pos_callback);
        glfwSetScrollCallback(m_data.handle, callbacks::scroll_callback);
    
        m_data.monitor = glfwGetPrimaryMonitor();
        glfwGetWindowSize(m_data.handle, (int*)&m_data.size.x, (int*)&m_data.size.y);
        glfwGetWindowPos(m_data.handle, (int*)&m_data.position.x, (int*)&m_data.position.y);
    }

    void window::shutdown() {
        glfwDestroyWindow(m_data.handle);
    }

    void window::on_update(const float& ) {
        glfwPollEvents();
        glfwSwapBuffers(m_data.handle);
    }

    void window::set_vsync(bool enabled) {
        glfwSwapInterval(m_data.vsync = enabled ? 1 : 0);
    }

    void window::set_icon(const char* path) {
        GLFWimage images[1];
        images[0].pixels = stbi_load(path, &images[0].width, &images[0].height, 0, 4);
        glfwSetWindowIcon(m_data.handle, 1, images);
    }

    void callbacks::window_resize_callback(GLFWwindow* window, int width, int height) {
        window_data& data = *(window_data*)glfwGetWindowUserPointer(window);
        data.size = { (unsigned)width, (unsigned)height };
        glViewport(0, 0, width, height);
        window_resize_event event(data.size);
        data.event_callback(event);
    }

    void callbacks::window_close_callback(GLFWwindow* window) {
        window_data& data = *(window_data*)glfwGetWindowUserPointer(window);
        window_close_event event;
        data.event_callback(event);
    }

    void callbacks::window_focus_callback(GLFWwindow* window, int focused) {
        window_data& data = *(window_data*)glfwGetWindowUserPointer(window);
        if(focused) {
            window_focus_event event;
            data.event_callback(event);
        } else {
            window_lost_focus_event event;
            data.event_callback(event);
        }
    }

    void callbacks::window_pos_callback(GLFWwindow* window, int xpos, int ypos) {
        window_data& data = *(window_data*)glfwGetWindowUserPointer(window);
        data.position = { xpos, ypos };

        window_moved_event event(data.position);
        data.event_callback(event);
    }

    void callbacks::mouse_button_callback(GLFWwindow* window, int button, int action, int mods) {
        (void)mods;
        window_data& data = *(window_data*)glfwGetWindowUserPointer(window);

        switch(action) {
            case GLFW_PRESS: {
                mouse_button_pressed_event event(button);
                data.event_callback(event);
                break;
            }
            case GLFW_RELEASE: {
                mouse_button_released_event event(button);
                data.event_callback(event);
                break;
            }
        }
    }

    void callbacks::cursor_pos_callback(GLFWwindow* window, double xpos, double ypos) {
        window_data& data = *(window_data*)glfwGetWindowUserPointer(window);
        vec2f mouse_pos = { (float)xpos, (float)ypos };

        mouse_moved_event event(mouse_pos);
        data.event_callback(event);
    }

    void callbacks::scroll_callback(GLFWwindow* window, double xoffset, double yoffset) {
        window_data& data = *(window_data*)glfwGetWindowUserPointer(window);
        vec2f offset = { (float)xoffset, (float)yoffset };

        mouse_scrolled_event event(offset);
        data.event_callback(event);
    }

    void callbacks::key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
        
        (void)scancode;
        (void)mods;

        window_data& data = *(window_data*)glfwGetWindowUserPointer(window);

        switch(action) {
            case GLFW_PRESS: {
                key_pressed_event event(key, 0);
                data.event_callback(event);
                break;
            }
            case GLFW_RELEASE: {
                key_released_event event(key);
                data.event_callback(event);
                break;
            }
            case GLFW_REPEAT: {
                key_pressed_event event(key, 1);
                data.event_callback(event);
                break;
            }
        }
    }

    void callbacks::char_callback(GLFWwindow* window, unsigned int codepoint) {
        window_data& data = *(window_data*)glfwGetWindowUserPointer(window);
        key_typed_event event(codepoint);
        data.event_callback(event);
    }

    void callbacks::glfw_error_callback(int error, const char* description) {
        LOG_CRITICAL("GLFW Error ({0}): {1}", error, description);
    }

}

#endif//OGL_CORE_WINDOW_HPP