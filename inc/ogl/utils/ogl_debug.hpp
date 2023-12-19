#pragma once
#ifndef OGL_UTILS_OGL_DEBUG_HPP
#define OGL_UTILS_OGL_DEBUG_HPP

#include <glad/glad.h>
#include "../core/log.hpp"


namespace ogl {

    enum class debug_log_level {
        none = 0,
        high_assertion = 1,
        high = 2,
        medium = 3,
        low = 4,
        notification = 5
    };

    static debug_log_level current_debug_log_level = debug_log_level::high_assertion;

    void set_debug_log_level(debug_log_level level) {
        current_debug_log_level = level;
    }

    void ogl_debug_callback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const void* user_param) {
        (void)source;
        (void)id;
        (void)length;
        (void)user_param;
        (void)type;
        
        switch (severity) {
            case GL_DEBUG_SEVERITY_HIGH:
                if((int)current_debug_log_level > (int)debug_log_level::none) {
                    LOG_ERROR("OpenGL Debug High Severity: {0}", message);
                    if(current_debug_log_level == debug_log_level::high_assertion) {
                        OGL_ASSERT(false, "OpenGL Debug High Severity");
                    }
                }
            break;
            case GL_DEBUG_SEVERITY_MEDIUM:
                if((int)current_debug_log_level > (int)debug_log_level::high) {
                    LOG_WARN("OpenGL Debug Medium Severity: {0}", message);
                }
            break;
            case GL_DEBUG_SEVERITY_LOW:
                if((int)current_debug_log_level > (int)debug_log_level::medium) {
                    LOG_INFO("OpenGL Debug Low Severity: {0}", message);
                }
            break;
            case GL_DEBUG_SEVERITY_NOTIFICATION:
                if((int)current_debug_log_level > (int)debug_log_level::low) {
                    LOG_TRACE("OpenGL Debug Notification: {0}", message);
                }
            break;
        }
    }

    void enable_debuging() {
        glDebugMessageCallback(ogl_debug_callback, nullptr);
        glEnable(GL_DEBUG_OUTPUT);
        glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
    }

}

#endif//OGL_UTILS_OGL_DEBUG_HPP