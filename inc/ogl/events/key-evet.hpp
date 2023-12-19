#pragma once
#ifndef OGL_EVENTS_KEY_EVENT_HPP
#define OGL_EVENTS_KEY_EVENT_HPP

#include "event.hpp"

namespace ogl {
    struct key_event : public event {
        public:
            inline const int& key_code() const { return m_key_code; }

            EVENT_CLASS_CATEGORY(event_category_keyboard | event_category_input)
        protected:
            key_event(int key_code) : m_key_code(key_code) {}
            int m_key_code;
    };

    struct key_pressed_event : public key_event {
        public:
            key_pressed_event(int key_code, int repeat_count) : key_event(key_code), m_repeat_count(repeat_count) {}

            inline const int& repeat_count() const { return m_repeat_count; }

            std::string to_string() const override {
                std::stringstream ss;
                ss << "key_pressed_event: " << m_key_code << " (" << m_repeat_count << " repeats)";
                return ss.str();
            }

            EVENT_CLASS_TYPE(key_pressed)
        private:
            int m_repeat_count;
    };

    struct key_released_event : public key_event {
        public:
            key_released_event(int key_code) : key_event(key_code) {}

            std::string to_string() const override {
                std::stringstream ss;
                ss << "key_released_event: " << m_key_code;
                return ss.str();
            }

            EVENT_CLASS_TYPE(key_released)
    };

    struct key_typed_event : public key_event {
        public:
            key_typed_event(int key_code) : key_event(key_code) {}

            std::string to_string() const override {
                std::stringstream ss;
                ss << "key_typed_event: " << m_key_code;
                return ss.str();
            }

            EVENT_CLASS_TYPE(key_typed)
    };
}

#endif//OGL_EVENTS_KEY_EVENT_HPP