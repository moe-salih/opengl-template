
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
            
            float vertices[] = {
                -1.0f, -1.0f, 0.0f,
                 1.0f, -1.0f, 0.0f,
                 1.0f,  1.0f, 0.0f,
                -1.0f,  1.0f, 0.0f
            };
            uint32_t indices[] = { 0, 1, 2, 2, 3, 0 };

            m_vbo.set_data(vertices, sizeof(vertices));

            m_layout.push<float>(3);
            m_vao.add_buffer(m_vbo, m_layout);

            m_ibo.set_data(indices, sizeof(indices)/sizeof(uint32_t));

            m_shader = ogl::shader::from_glsl_file(
                "res/shaders/vert.glsl",
                "res/shaders/frag.glsl"
            );

        }

        void on_detach() override {
            LOG_INFO("Layer::Detach");
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

            m_vao.bind();
            m_ibo.bind();

            glDrawElements(GL_TRIANGLES, m_ibo.count(), GL_UNSIGNED_INT, nullptr);     
       
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
        // unsigned int m_vao, m_vbo, m_ibo;
        ogl::vertex_array m_vao;
        ogl::vertex_buffer m_vbo;
        ogl::vertex_buffer_layout m_layout;
        ogl::index_buffer m_ibo;

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