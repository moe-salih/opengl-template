
#include <imgui/imgui.h>
#include <imgui/imgui_impl_glfw.h>
#include <imgui/imgui_impl_opengl3.h>

#include "layer.hpp"
#include "application.hpp"

#include "events/event.hpp"
#include "events/mouse-event.hpp"
#include "log.hpp"

namespace ogl {

    struct imgui_layer : public ogl::layer{
        protected:
            float m_time = 0.0f;
        public:
            imgui_layer() : layer("ImGuiLayer") {}
            ~imgui_layer() = default;

            void on_attach() override {
                IMGUI_CHECKVERSION();
                ImGui::CreateContext();

                ImGuiIO& io = ImGui::GetIO();

                io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
                io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
                io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;

                ImGui::StyleColorsDark();

                ImGuiStyle& style = ImGui::GetStyle();
                if(io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable) {
                    style.WindowRounding = 1.0f;
                    style.Colors[ImGuiCol_WindowBg].w = 1.0f;
                }

                ogl::application& app = ogl::application::instance();
                GLFWwindow* window = static_cast<GLFWwindow*>(app.get_window().native_window());

                ImGui_ImplGlfw_InitForOpenGL(window, true);
                ImGui_ImplOpenGL3_Init("#version 460");
                LOG_INFO("ImGuiLayer::Attached");
            }

            void on_detach() override {
                ImGui_ImplOpenGL3_Shutdown();
                ImGui_ImplGlfw_Shutdown();
                ImGui::DestroyContext();
                LOG_INFO("ImGuiLayer::Detached");
            }

            void begin() {
                ImGui_ImplOpenGL3_NewFrame();
                ImGui_ImplGlfw_NewFrame();
                ImGui::NewFrame();
            }

            virtual void on_update(const float& dt) override {}

            void end() {
                ImGuiIO& io = ImGui::GetIO();
                ogl::application& app = ogl::application::instance();
                io.DisplaySize = ImVec2(app.get_window().size().x, app.get_window().size().y);

                ImGui::Render();
                ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

                if(io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable) {
                    GLFWwindow* backup_current_context = glfwGetCurrentContext();
                    ImGui::UpdatePlatformWindows();
                    ImGui::RenderPlatformWindowsDefault();
                    glfwMakeContextCurrent(backup_current_context);
                }
            }

            void on_event(ogl::event& e) override {
                ogl::event_dispatcher dispatcher(e);
                dispatcher.dispatch<ogl::mouse_button_pressed_event>(BIND_EVENT_FN(imgui_layer::on_mouse_button_pressed));
            }

            bool on_mouse_button_pressed(ogl::mouse_button_pressed_event& e) {
                ImGuiIO& io = ImGui::GetIO();
                return io.WantCaptureMouse;
            }      
    };

}