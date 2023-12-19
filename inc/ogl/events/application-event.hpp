#pragma once
#ifndef OGL_EVENTS_APPLICATION_EVENT_HPP
#define OGL_EVENTS_APPLICATION_EVENT_HPP

#include "event.hpp"
#include "../utils/vec.hpp"

namespace ogl {

    struct window_resize_event : public event {
        public:
            window_resize_event(const vec2u& size) : m_size(size) {}

            inline const vec2u& size() const { return m_size; }

            std::string to_string() const override {
                std::stringstream ss;
                ss << "window_resize_event: " << m_size.x << ", " << m_size.y;
                return ss.str();
            }

            EVENT_CLASS_TYPE(window_resize)
            EVENT_CLASS_CATEGORY(event_category_application)
        private:
            vec2u m_size;
    };

    struct window_close_event : public event {
        public:
            window_close_event() {}

            EVENT_CLASS_TYPE(window_close)
            EVENT_CLASS_CATEGORY(event_category_application)
    };

    struct window_focus_event : public event {
        public:
            window_focus_event() {}
            

            EVENT_CLASS_TYPE(window_focus)
            EVENT_CLASS_CATEGORY(event_category_application)
    };

    struct window_lost_focus_event : public event {
        public:
            window_lost_focus_event() {}

            EVENT_CLASS_TYPE(window_lost_focus)
            EVENT_CLASS_CATEGORY(event_category_application)
    };

    struct window_moved_event : public event {
        public:
            window_moved_event(const vec2i& p) : m_position(p) {}

            inline const vec2i& position() const { return m_position; }

            std::string to_string() const override {
                std::stringstream ss;
                ss << "window_moved_event: " << m_position.x << ", " << m_position.y;
                return ss.str();
            }

            EVENT_CLASS_TYPE(window_moved)
            EVENT_CLASS_CATEGORY(event_category_application)
        private:
            vec2i m_position;
    }; 


    struct app_tick_event : public event {
        public:
            app_tick_event() {}

            EVENT_CLASS_TYPE(app_tick)
            EVENT_CLASS_CATEGORY(event_category_application)
    };

    struct app_update_event : public event {
        public:
            app_update_event() {}

            EVENT_CLASS_TYPE(app_update)
            EVENT_CLASS_CATEGORY(event_category_application)
    };

    struct app_render_event : public event {
        public:
            app_render_event() {}

            EVENT_CLASS_TYPE(app_render)
            EVENT_CLASS_CATEGORY(event_category_application)
    };

}

#endif//OGL_EVENTS_APPLICATION_EVENT_HPP