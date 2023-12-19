#pragma once
#ifndef OGL_EVENTS_MOUSE_EVENT_HPP
#define OGL_EVENTS_MOUSE_EVENT_HPP

#include "event.hpp"
#include "../utils/vec.hpp"


namespace ogl {

    struct mouse_button_event : public event {
        public:
            inline const int& button() const { return m_button; }

            EVENT_CLASS_CATEGORY(event_category_mouse | event_category_input)
        protected:
            mouse_button_event(int button) : m_button(button) {}
            int m_button;
    };

    struct mouse_button_pressed_event : public mouse_button_event {
        public:
            mouse_button_pressed_event(int button) : mouse_button_event(button) {}

            std::string to_string() const override {
                std::stringstream ss;
                ss << "mouse_button_pressed_event: " << m_button;
                return ss.str();
            }

            EVENT_CLASS_TYPE(mouse_button_pressed)
    };

    struct mouse_button_released_event : public mouse_button_event {
        public:
            mouse_button_released_event(int button) : mouse_button_event(button) {}

            std::string to_string() const override {
                std::stringstream ss;
                ss << "mouse_button_released_event: " << m_button;
                return ss.str();
            }

            EVENT_CLASS_TYPE(mouse_button_released)
    };

    struct mouse_moved_event : public event {
        public:
            mouse_moved_event(const vec2f& p) : m_position(p) {}

            inline const vec2f& position() const { return m_position; }

            std::string to_string() const override {
                std::stringstream ss;
                ss << "mouse_moved_event: " << m_position.x << ", " << m_position.y;
                return ss.str();
            }

            EVENT_CLASS_TYPE(mouse_moved)
            EVENT_CLASS_CATEGORY(event_category_mouse | event_category_input)
        private:
            vec2f m_position;
    };

    struct mouse_scrolled_event : public event {
        public:
            mouse_scrolled_event(const vec2f& o) : m_offset(o) {}

            inline const vec2f& offset() const { return m_offset; }

            std::string to_string() const override {
                std::stringstream ss;
                ss << "mouse_scrolled_event: " << m_offset.x << ", " << m_offset.y;
                return ss.str();
            }

            EVENT_CLASS_TYPE(mouse_scrolled)
            EVENT_CLASS_CATEGORY(event_category_mouse | event_category_input)
        private:
            vec2f m_offset;
    };

}

#endif//OGL_EVENTS_MOUSE_EVENT_HPP