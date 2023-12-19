#pragma once

#ifndef OGL_CORE_APPLICATION_HPP
#define OGL_CORE_APPLICATION_HPP

#include "window.hpp"
#include "layer-stack.hpp"

#include "../events/application-event.hpp"

#include <memory>

namespace ogl {

    struct application {
        public:
            application( const char* title = "OGL Application", const vec2u& size = { 640, 480 } );
            virtual ~application() = default; 

            void set_icon(const char* path) { m_window->set_icon(path); }
            void run();

            void on_event(event& e);

            void push_layer(layer* layer);
            void push_overlay(layer* overlay);


            inline basic_window& get_window() { return *m_window; }
            inline static application& instance() { return *s_instance; }
        private:
            bool on_window_close(window_close_event& e);

        private:
            std::unique_ptr<basic_window> m_window;
            layer_stack m_layer_stack;
            bool m_running = true;

            float m_last_frame_time = 0.0f;
            static application* s_instance;
    };

    #define BIND_EVENT_FN(fn) std::bind(&fn, this, std::placeholders::_1)

    application* application::s_instance = nullptr;

    application::application( const char* title, const vec2u& size ) {
        
        if(!s_instance) {
            log::init();
        }
        

        OGL_ASSERT(!s_instance, "application already exists!");
        s_instance = this;

        m_window = std::unique_ptr<basic_window>(basic_window::create({title, size}));
        m_window->set_event_callback(BIND_EVENT_FN(application::on_event));
    }    


    void application::push_layer(layer* lyr) {
        m_layer_stack.push_layer(lyr);
    }

    void application::push_overlay(layer* overlay) {
        m_layer_stack.push_overlay(overlay);
    }

    void application::on_event(event& e) {
        event_dispatcher dispatcher(e);
        dispatcher.dispatch<window_close_event>(BIND_EVENT_FN(application::on_window_close));

        for(auto it = m_layer_stack.end(); it != m_layer_stack.begin(); ) {
            (*--it)->on_event(e);
            if(e.handled) break;
        }
    }

    void application::run() {
        float delta_time, time;
        while(m_running) {
            time = (float)glfwGetTime();
            delta_time = time - m_last_frame_time;
            m_last_frame_time = time;

            for(layer* lyr : m_layer_stack) {
                lyr->on_update(delta_time);
            }

            m_window->on_update(delta_time);
        }
    }

    bool application::on_window_close(window_close_event& e) {
        (void)e;
        m_running = false;
        return true;
    }


}

#endif//OGL_CORE_APPLICATION_HPP