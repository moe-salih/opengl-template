#pragma once

#ifndef OGL_CORE_LAYER_HPP
#define OGL_CORE_LAYER_HPP

#include "../events/event.hpp"

namespace ogl {
    struct layer {
        public:
            layer( const char* name = "Layer") : m_debug_name(name) {}
            virtual ~layer() = default;

            virtual void on_attach() {}
            virtual void on_detach() {}
            virtual void on_update(const float&) {}

            // virtual void on_imgui_render() {}            

            virtual void on_event(event&) {}

            inline const char* name() const { return m_debug_name; }

        private:
            const char* m_debug_name;
    };
}

#endif//OGL_CORE_LAYER_HPP