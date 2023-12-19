#pragma once

#ifndef OGL_EVENTS_EVENT_HPP
#define OGL_EVENTS_EVENT_HPP

#include <string>
#include <sstream>


namespace ogl {

    enum class event_type {
        none = 0,
        // window events
        window_close, window_resize, window_focus, window_lost_focus, window_moved, 
        // app events
        app_tick, app_update, app_render,
        // key events
        key_pressed, key_released, key_typed,
        // mouse events
        mouse_button_pressed, mouse_button_released, mouse_moved, mouse_scrolled
    };

    enum event_category {
        none                        = 0,
        event_category_application  = 1 << 0,
        event_category_input        = 1 << 1,
        event_category_keyboard     = 1 << 2,
        event_category_mouse        = 1 << 3,
        event_category_mouse_button = 1 << 4
    };


    #define EVENT_CLASS_TYPE(type) \
        static event_type get_static_type() { return event_type::type; } \
        virtual event_type get_event_type() const override { return get_static_type(); } \
        virtual const char* get_name() const override { return #type; }
    
    #define EVENT_CLASS_CATEGORY(category) \
        virtual int get_category_flags() const override { return category; }

    struct event {
        public:
            bool handled = false;

            virtual event_type get_event_type() const = 0;
            virtual const char* get_name() const = 0;
            virtual int get_category_flags() const = 0;
            virtual std::string to_string() const { return get_name(); }

            inline bool is_in_category(event_category category) {
                return get_category_flags() & category;
            }

            inline friend std::ostream& operator<<(std::ostream& os, const event& e) {
                return os << e.to_string();
            }
    };

    struct event_dispatcher {
        public:
            event_dispatcher(event& event) : m_event(event) {}

            template<typename T, typename F>
            bool dispatch(const F& func) {
                if (m_event.get_event_type() == T::get_static_type()) {
                    m_event.handled |= func(static_cast<T&>(m_event));
                    return true;
                }
                return false;
            }
        private:
            event& m_event;
    };


}


#endif//OGL_EVENTS_EVENT_HPP