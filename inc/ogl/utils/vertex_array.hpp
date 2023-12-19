#pragma once
#ifndef VERTEX_ARRAY_HPP
#define VERTEX_ARRAY_HPP

#include "index_buffer.hpp"

namespace ogl {

    struct vertex_array {
        private:
            unsigned int id_;

        public:
            vertex_array() { glGenVertexArrays( 1, &id_ ); }
            ~vertex_array() { glDeleteVertexArrays( 1, &id_ ); }

            void bind() const { glBindVertexArray( id_ ); }
            void unbind() const { glBindVertexArray( 0 ); }

            void add_buffer( const vertex_buffer& vb, const vertex_buffer_layout& layout ) {
                bind(); vb.bind();
                const auto& elements = layout.elements();
                size_t offset = 0;
                for( unsigned int i = 0; i < elements.size(); ++i ) {
                    const auto& element = elements[i];
                    glEnableVertexAttribArray( i );
                    glVertexAttribPointer( i, element.count, element.type, element.normalized, layout.stride(), (const void*)offset );
                    offset += element.count * vertex_buffer_element::size_of_type( element.type );
                }
            }
    };

}

#endif//VERTEX_ARRAY_HPP
