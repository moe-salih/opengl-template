#pragma once
#ifndef OGL_CORE_LAYER_STACK_HPP
#define OGL_CORE_LAYER_STACK_HPP

#include "layer.hpp"

#include <vector>
#include <algorithm>

namespace ogl {

    struct layer_stack {
        public:
            using iterator = std::vector<layer*>::iterator;
        public:
            layer_stack() : m_layer_insert_index(0) { }
            ~layer_stack();

            void push_layer(layer* layer);
            void push_overlay(layer* overlay);
            void pop_layer(layer* layer);
            void pop_overlay(layer* overlay);

            iterator begin() { return m_layers.begin(); }
            iterator end() { return m_layers.end(); }

        private:    
            std::vector<layer*> m_layers;
            unsigned int m_layer_insert_index;
    };


    layer_stack::~layer_stack() {
        for (auto& layer : m_layers) {
            layer->on_detach();
            delete layer;
        }
    }

    void layer_stack::push_layer(layer* lyr) {
        m_layers.emplace(m_layers.begin() + m_layer_insert_index, lyr);
        m_layer_insert_index++;
        lyr->on_attach();
    }

    void layer_stack::push_overlay(layer* lyr) {
        m_layers.emplace_back(lyr);
        lyr->on_attach();
    }

    void layer_stack::pop_layer(layer* lyr) {
        auto it = std::find(m_layers.begin(), m_layers.begin() + m_layer_insert_index, lyr);
        if (it != m_layers.begin() + m_layer_insert_index) {
            lyr->on_detach();
            m_layers.erase(it);
            m_layer_insert_index--;
        }
    }

    void layer_stack::pop_overlay(layer* lyr) {
        auto it = std::find(m_layers.begin() + m_layer_insert_index, m_layers.end(), lyr);
        if (it != m_layers.end()) {
            lyr->on_detach();
            m_layers.erase(it);
        }
    }



}

#endif//OGL_CORE_LAYER_STACK_HPP