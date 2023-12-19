#pragma once
#ifndef OGL_UTILS_SHADER_HPP
#define OGL_UTILS_SHADER_HPP

#include "../core/application.hpp"

#include <sstream>
#include <fstream>


namespace ogl {


    struct shader {
        public:
            ~shader();

            unsigned int id() const { return m_id; }
            void reload( const char* vert_path, const char* frag_path) {
                glDeleteProgram(m_id);
                load_from_glsl_file(vert_path, frag_path);
            }
            static shader* from_glsl_file(const char* vert_path, const char* frag_path);
        
        private:
            shader() = default;
            void load_from_glsl_file(const char* vert_path, const char* frag_path);
            unsigned int compile_shader(unsigned int type, const char* source);
        private:
            unsigned int m_id;
    };

    shader::~shader() {
        glDeleteProgram(m_id);
    }

    static std::string read_file( const char* path) {
        std::stringstream ss;
        std::ifstream file(path, std::ios::in | std::ios::binary);

        if(file) {
            ss << file.rdbuf();
            file.close();
        } else {
            OGL_ASSERT(false, "Failed to read file: {0}", path);
        }

        return ss.str();
    }
    
    unsigned int shader::compile_shader(unsigned int type, const char* source) {
        unsigned int id = glCreateShader(type);
        glShaderSource(id, 1, &source, nullptr);
        glCompileShader(id);

        int result;
        glGetShaderiv(id, GL_COMPILE_STATUS, &result);
        if(!result) {
            int length;
            glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
            char* message = (char*)alloca(length * sizeof(char));
            glGetShaderInfoLog(id, length, &length, message);
            LOG_ERROR("Failed to compile {0} shader: {1}", (type == GL_VERTEX_SHADER ? "vertex" : "fragment"), message);
            glDeleteShader(id);
            return 0;
        }

        return id;
    }

    shader* shader::from_glsl_file(const char* vert_path, const char* frag_path) {
        shader* s = new shader();
        s->load_from_glsl_file(vert_path, frag_path);
        return s;
    }

    void shader::load_from_glsl_file(const char* vert_path, const char* frag_path) {
        std::string vert_source = read_file(vert_path);
        std::string frag_source = read_file(frag_path);

        unsigned int program = glCreateProgram();
        unsigned int vs = compile_shader(GL_VERTEX_SHADER, vert_source.c_str());
        unsigned int fs = compile_shader(GL_FRAGMENT_SHADER, frag_source.c_str());

        glAttachShader(program, vs);
        glAttachShader(program, fs);
        glLinkProgram(program);
        glValidateProgram(program);

        int result;
        glGetProgramiv(program, GL_LINK_STATUS, &result);
        if(!result) {
            int length;
            glGetProgramiv(program, GL_INFO_LOG_LENGTH, &length);
            char* message = (char*)alloca(length * sizeof(char));
            glGetProgramInfoLog(program, length, &length, message);

            glDeleteProgram(program);

            LOG_ERROR("Failed to link shader program: {0}", message);
        }
        
        glDetachShader(program, vs);
        glDetachShader(program, fs);
        glDeleteShader(vs);
        glDeleteShader(fs);

        m_id = program;
    }
}

#endif//OGL_UTILS_SHADER_HPP