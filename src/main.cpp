
#include "main.hpp"

struct layer : ogl::layer {
    public:
        layer( const ogl::vec2u& size) : ogl::layer("Example"), m_resolution(size){}
        ~layer(){}

        void on_attach() override {
            LOG_INFO("Layer::Attach");

            ogl::enable_debuging();
            ogl::set_debug_log_level(ogl::debug_log_level::medium);

            glEnable(GL_DEPTH_TEST);
            glEnable(GL_BLEND);
            glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

            m_shader = ogl::shader::from_glsl_file(
                "res/shaders/vert.glsl",
                "res/shaders/frag.glsl"
            );

           

            glCreateVertexArrays(1, &m_vao);
            glBindVertexArray(m_vao);

            float vertices[] = {
                -1.0f, -1.0f, 0.0f,
                 1.0f, -1.0f, 0.0f,
                 1.0f,  1.0f, 0.0f,
                -1.0f,  1.0f, 0.0f
            };

            glCreateBuffers(1, &m_vbo);
            glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
            glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

            glEnableVertexAttribArray(0);
            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, 0);

            uint32_t indices[] = { 0, 1, 2, 2, 3, 0 };
            glCreateBuffers(1, &m_ibo);
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ibo);
            glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
        }

        void on_detach() override {
            LOG_INFO("Layer::Detach");
        
            glDeleteVertexArrays(1, &m_vao);
            glDeleteBuffers(1, &m_vbo);
            glDeleteBuffers(1, &m_ibo);
        }

        void on_update(const float& delta_time) override {
            // LOG_INFO("Layer::Update: {0}s", delta_time);
            time += delta_time;

            glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

            glUseProgram(m_shader->id());

            int resolution_location = glGetUniformLocation(m_shader->id(), "u_resolution");
            glUniform2f(resolution_location, m_resolution.x, m_resolution.y);
            
            int time_location = glGetUniformLocation(m_shader->id(), "u_time");
            glUniform1f(time_location, time);

            int mouse_location = glGetUniformLocation(m_shader->id(), "u_mouse");
            glUniform2f(mouse_location, m_mouse.x, m_mouse.y);

            glBindVertexArray(m_vao);
            glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);     
       
        }

        void on_event(ogl::event& e) override {
            // LOG_INFO("Layer::Event {0}",e);

            if(e.get_event_type() == ogl::event_type::window_resize) {
                ogl::window_resize_event& we = (ogl::window_resize_event&)e;
                m_resolution = {we.size().x, we.size().y};
            }

            if(e.get_event_type() == ogl::event_type::key_pressed) {
                ogl::key_pressed_event& kpe = (ogl::key_pressed_event&)e;
                if(kpe.key_code() == GLFW_KEY_R) {
                    m_shader->reload("res/shaders/vert.glsl", "res/shaders/frag.glsl");
                }
            }

            if(e.get_event_type() == ogl::event_type::mouse_moved) {
                ogl::mouse_moved_event& mme = (ogl::mouse_moved_event&)e;
                m_mouse = mme.position();
            }

        }
    
    private:
        unsigned int m_vao, m_vbo, m_ibo;
        ogl::shader* m_shader;
        ogl::vec2u m_resolution;
        ogl::vec2f m_mouse = {0.0f, 0.0f};
        float time = 0.0f;
};

struct app : public ogl::application {
    app( const char* title="GLSL", const ogl::vec2u& size={512, 512}) : ogl::application(title, size) {
        set_icon("res/icons/icon-64_x_64.png");
        push_layer(new layer( size ));
    }
};

int main() {
    std::unique_ptr<app> a = std::make_unique<app>("Real-time Shader Viewer", ogl::vec2u(1024, 512));
    a->run();
}