#ifndef OGL_CORE_BASIC_WINDOW_HPP
#define OGL_CORE_BASIC_WINDOW_HPP

#include "../utils/vec.hpp"
#include "../events/event.hpp"

#include <functional>


namespace ogl {

    struct basic_window_props {
        const char* title;
        vec2u size;

        basic_window_props(
            const char* title = "ogl basic window",
            vec2u size = { 640, 480 }
        ) : title(title), size(size) {}
    };

    struct basic_window {
        public:
            using event_callback_fn = std::function<void(event&)>;
        
            virtual ~basic_window() = default;

            virtual void on_update(const float& delta_time ) = 0;

            virtual vec2u size() const = 0;

            // window attributes
            virtual void set_event_callback(const event_callback_fn& callback) = 0;
            virtual void set_vsync(bool enabled) = 0;
            virtual void set_icon(const char* path) = 0;
            virtual bool is_vsync() const = 0;

            virtual void* native_window() const = 0;
            static basic_window* create(const basic_window_props& props = basic_window_props());
    };  

}

#endif//OGL_CORE_BASIC_WINDOW_HPP