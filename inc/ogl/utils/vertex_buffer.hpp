#pragma once
#ifndef VERTEX_BUFFER_HPP
#define VERTEX_BUFFER_HPP

#include "vertex_buffer_layout.hpp"

namespace ogl{

    struct vertex_buffer {
        private:
            unsigned int id_;

        public:
            vertex_buffer() { glGenBuffers( 1, &id_ ); }
            ~vertex_buffer() { glDeleteBuffers( 1, &id_ ); }

            void set_data( const void* data, unsigned int size ) { bind(); glBufferData( GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW ); }

            void bind() const { glBindBuffer( GL_ARRAY_BUFFER, id_ ); }
            void unbind() const { glBindBuffer( GL_ARRAY_BUFFER, 0 ); }
    };

}

#endif//VERTEX_BUFFER_HPP